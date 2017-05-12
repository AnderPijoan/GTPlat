#include "FuzzyLogicTrainer.h"

#include "utils/exporters/debugger/Debugger.h"

#include <QDateTime>
#include <QDebug>

#include "utils/fuzzy_logic/trainer/crossover/rules/RuleCrossoverOperator.h"
#include "utils/fuzzy_logic/trainer/crossover/rules/RuleCombineCrossover.h"
#include "utils/fuzzy_logic/trainer/crossover/rules/RuleSliceCrossover.h"
#include "utils/fuzzy_logic/trainer/crossover/rules/RuleSliceTermsCrossover.h"

#include "utils/fuzzy_logic/trainer/mutation/rules/RuleMutationOperator.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleAddMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleAddTermMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleOutputMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleRemoveMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleRemoveTermMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleTermInputMutation.h"
#include "utils/fuzzy_logic/trainer/mutation/rules/RuleTermVarableMutation.h"

#include "utils/sorters/NumberSorter.h"
#include "utils/exporters/logger/Logger.h"
#include "utils/statistics/Statistics.h"

FuzzyLogicTrainer::FuzzyLogicTrainer( QMap<QString , QStringList> input_values , QStringList outputs  , QObject *parent) : QObject(parent){

    this->distribution = new UniformDistribution( 0 );

    this->accumulators << "AlgebraicSum" << "BoundedSum" << "DrasticSum" << "EinsteinSum" << "HamacherSum" << "Maximum" << "NormalizedSum";
    this->conjunctions << "AlgebraicProduct" << "BoundedDifference" << "DrasticProduct" << "EinsteinProduct" << "HamacherProduct" << "Minimum";
    this->disjunctions << "AlgebraicSum" << "BoundedSum" << "DrasticSum" << "EinsteinSum" << "HamacherSum" << "Maximum" << "NormalizedSum";
    this->activators << "AlgebraicProduct" << "BoundedDifference" << "DrasticProduct" << "EinsteinProduct" << "HamacherProduct" << "Minimum";

    // Load data structures for fast retrieve of values
    foreach( QString input , input_values.keys() ){

        this->input_names.insert( input , QStringList() );

        foreach( QString value_name , input_values.value( input ) ){
            this->input_names[ input ].append( value_name );
        }
    }
    this->output_names = outputs;
}

FuzzyLogicTrainer::~FuzzyLogicTrainer(){
    this->distribution->deleteLater();
    qDeleteAll( this->rule_crossovers );
    qDeleteAll( this->rule_mutations );
}

/**********************************************************************
   GETTERS
**********************************************************************/

QStringList FuzzyLogicTrainer::getOutputNames(){
    return this->output_names;
}

QMap<QString , QStringList> FuzzyLogicTrainer::getInputNames(){
    return this->input_names;
}

/**********************************************************************
   GENERATORS
**********************************************************************/

QList< FuzzyLogic::InputMemberShipFunction > FuzzyLogicTrainer::generateInputset(){

    QList< FuzzyLogic::InputMemberShipFunction > inputs;

    foreach( QString input_name , this->input_names.keys() ){

        FuzzyLogic::InputMemberShipFunction input_mf;
        input_mf.input_name = input_name;

        Debugger::debug( this , Q_FUNC_INFO , QString("Variable %1").arg( input_name ) );

        // Create triangles randomly: calculate n-2 numbers (because the first will be 0 and last will be 1)
        QVector<double> randoms;
        srand( QDateTime::currentDateTime().toMSecsSinceEpoch() );
        randoms.append( 0 );
        /*for( int i = 0 ; i < this->value_names.value( input_name ).size() -2 ; i++){
            randoms.append( qrand() % 1000 / 1000.0 );
        }*/
        randoms.append( 0.25 );
        randoms.append( 0.5 );
        randoms.append( 0.75 );
        randoms.append( 1 );
        randoms = NumberSorter::sortAscending( randoms );

        for( int i = 0; i < this->input_names.value( input_name ).size() ; i++ ){

            QString value_name = this->input_names.value( input_name ).at( i );
            FuzzyLogic::MemberShipFunctionTerm term;
            term.name = value_name;

            // Center of the term will be at its position in randoms
            double center = randoms.at( i );

            // left point
            double left = i == 0 ? -0.25 : randoms.at( i-1 );
            // right point
            double right = i == randoms.size()-1 ? 1.25 : randoms.at( i+1 );

            term.shape_points.append( left );
            term.shape_points.append( center );
            term.shape_points.append( right );
            input_mf.terms.append( term );
            Debugger::debug( this , Q_FUNC_INFO , QString("%1 with Triangle %2 points (%3 %4 %5)").arg( input_name ).arg( value_name ).arg( left ).arg( center ).arg( right ) );

        }
        inputs.append( input_mf );
    }

    return inputs;
}

QList< FuzzyLogic::Rule > FuzzyLogicTrainer::generateRuleset(){
    QList< FuzzyLogic::Rule > ruleset;
    for( int k = 0 ; k < (qrand() % 50)+1 ; k++ ){
        ruleset.append( this->generateRule() );
    }
    return ruleset;
}

FuzzyLogic::Rule FuzzyLogicTrainer::generateRule(){

    FuzzyLogic::Rule rule;
    int terms_amount = (qrand() % 2)+1;
    for( int k = 0 ; k < terms_amount ; k++ ){
        rule.terms.append( this->generateRuleTerm() );
    }
    rule.output_value = this->output_names.at( qrand() % this->output_names.size() );
    return rule;
}

FuzzyLogic::RuleTerm FuzzyLogicTrainer::generateRuleTerm(){
    FuzzyLogic::RuleTerm term;
    term.input_name = this->input_names.keys().at( qrand() % this->input_names.keys().size() );
    term.term_name = this->input_names.value( term.input_name ).at( qrand() % this->input_names.value( term.input_name ).size() );
    return term;
}

FuzzyLogic::OutputMembershipFunction FuzzyLogicTrainer::generateOutput(){

    FuzzyLogic::OutputMembershipFunction output;

    output.output_name = "TRANSPORT";
    foreach(QString output_name , this->output_names ){
        FuzzyLogic::MemberShipFunctionTerm term;
        term.name = output_name;
        term.shape_points.append( this->output_names.indexOf( output_name ) );
        term.shape_points.append( this->output_names.indexOf( output_name )+1 );
        output.terms.append( term );
    }

    return output;
}

/**********************************************************************
   EXECUTION
**********************************************************************/

void FuzzyLogicTrainer::init(){

    // Create randomly best fuzzy experiment
    this->best_experiment.inputset = this->generateInputset();
    this->best_experiment.ruleset = this->generateRuleset();
    this->best_experiment.output = this->generateOutput();
    this->best_experiment.accumulator = this->accumulators.at( qrand() % this->accumulators.size() );
    this->best_experiment.conjunction = this->conjunctions.at( qrand() % this->conjunctions.size() );
    this->best_experiment.disjunction = this->disjunctions.at( qrand() % this->disjunctions.size() );
    this->best_experiment.activator = this->activators.at( qrand() % this->activators.size() );

    // Iterate rule terms and create rules
    int individual_amount = 100;

    // Generate population
    while( this->rule_population.size() < individual_amount ){
        this->rule_population.append( this->generateRuleset() );
    }

    while( this->input_population.size() < individual_amount ){
        this->input_population.append( this->generateInputset() );
    }

    // Add crossover operators
    this->rule_crossovers.append( new RuleCombineCrossover( this ) );
    this->rule_crossovers.append( new RuleSliceCrossover( this ) );
    this->rule_crossovers.append( new RuleSliceTermsCrossover( this ) );

    // Add mutation operators
    this->rule_mutations.append( new RuleAddMutation( this ) );
    this->rule_mutations.append( new RuleAddTermMutation( this ) );
    this->rule_mutations.append( new RuleOutputMutation( this ) );
    this->rule_mutations.append( new RuleRemoveMutation( this ) );
    this->rule_mutations.append( new RuleRemoveTermMutation( this ) );
    this->rule_mutations.append( new RuleTermInputMutation( this ) );
    this->rule_mutations.append( new RuleTermVarableMutation( this ) );

}

void FuzzyLogicTrainer::execute(){

    Debugger::setDebugLevel( 2 );

    // INIT
    Logger::log( QString("%1;%2;%3;%4;%5;%6;%7;%8")
                 .arg( "STEP" )
                 .arg( "GA" )
                 .arg( "FITNESS" )
                 .arg( "MEAN" )
                 .arg( "STD" )
                 .arg( "MAX")
                 .arg( "MIN")
                 .arg( "RULES" ));

    // LOOP
    for( int step = 0 ; step < 9999 ; step++ ){

        // Execute rule mutations for the best input and const output
        this->executeRules();

        this->executeAccumulators();

        this->executeConjunctions();

        this->executeDisjunctions();

        this->executeActivators();

        QVector<double> fitness;
        this->best_experiment.ruleset = this->getBestRuleset( this->rule_population , fitness );
        Logger::log( QString("%1;%2;%3;%4;%5;%6;%7;%8")
                     .arg( step )
                     .arg( "RULES" )
                     .arg( this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator) )
                     .arg( Statistics::mean( fitness ) )
                     .arg( Statistics::std( fitness ) )
                     .arg( Statistics::max( fitness ) )
                     .arg( Statistics::min( fitness ) )
                     .arg( this->best_experiment.toString() ));

    }

    Debugger::info( this , Q_FUNC_INFO , QString("EXECUTION FINISHED") );

    // Inputs
    foreach( FuzzyLogic::InputMemberShipFunction i , this->best_experiment.inputset ){
        foreach(FuzzyLogic::MemberShipFunctionTerm t , i.terms ){
            Debugger::info( this , Q_FUNC_INFO , QString("%1 : %2 (%3 , %4 , %5)").arg( i.input_name ).arg( t.name ).arg( t.shape_points.at(0) ).arg( t.shape_points.at(1) ).arg( t.shape_points.at(2) ) );
        }
    }

    // Rules
    foreach( FuzzyLogic::Rule r , this->best_experiment.ruleset ){
        QString str = QString("then %1").arg( r.output_value );
        foreach(FuzzyLogic::RuleTerm t , r.terms ){
            str.prepend( QString("%1 IS %2 ").arg( t.input_name ).arg( t.term_name) );
        }
        Debugger::info( this , Q_FUNC_INFO , str );
    }
}

void FuzzyLogicTrainer::executeRules(){

    QList< QList< FuzzyLogic::Rule> > new_population;
    new_population.append( this->best_experiment.ruleset );
    qsrand( QDateTime::currentDateTime().toMSecsSinceEpoch() );

    double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

    for( int step = 0 ; step < this->rule_population.size()/2 ; step++ ){

        // Select two elements to apply operators and generate the new candidate ruleset
        int rulesets_pos = qrand() % this->rule_population.size();
        int rule_operator_pos = qrand() % this->rule_crossovers.size();

        QList<FuzzyLogic::Rule> candidate_rules = this->rule_crossovers.at( rule_operator_pos )->crossover(
                    this->rule_population.at( rulesets_pos % this->rule_population.size() ),
                    this->rule_population.at( (rulesets_pos+1) % this->rule_population.size() ) );

        // Compare the new candidate ruleset with the best
        double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , candidate_rules , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

        if( candidate_fitness <= best_fitness && this->best_experiment.ruleset != candidate_rules ){
            this->best_experiment.ruleset = candidate_rules;
            new_population.append( candidate_rules );
            best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );
        }

    }

    for( int step = 0 ; step < this->rule_population.size()/2 ; step++ ){

        // Select two elements to apply operators and generate the new candidate ruleset
        int rulesets_pos = qrand() % this->rule_population.size();
        int rule_operator_pos = qrand() % this->rule_mutations.size();

        QList<FuzzyLogic::Rule> candidate_rules = this->rule_mutations.at( rule_operator_pos )->mutation(
                    this->rule_population.at( rulesets_pos % this->rule_population.size() ) );

        // Compare the new candidate ruleset with the best
        double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , candidate_rules , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );
        double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

        if( candidate_fitness <= best_fitness && this->best_experiment.ruleset != candidate_rules ){
            this->best_experiment.ruleset = candidate_rules;
            new_population.append( candidate_rules );
            best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );
        }

    }

    // Replace rule population
    while( new_population.size() < this->rule_population.size() ){
        new_population.append( this->generateRuleset() );
    }
    this->rule_population = new_population;
    qDebug() << this->rule_population.size();
}


void FuzzyLogicTrainer::executeInputs(){

    QVector<double> fitness;
    for( int step = 0 ; step < 10 ; step++ ){

        /*if( 1 ){

            QList< QList<FuzzyLogic::InputMemberShipFunction> > candidate_elements;

            // Select two elements to apply operators and take them out of population
            int membership_pos = qrand() % this->input_population.size();
            candidate_elements.append( this->input_population.takeAt( membership_pos % this->input_population.size() ) );
            candidate_elements.append( this->input_population.takeAt( (membership_pos+1) % this->input_population.size() ) );

            // Generate new two elements
            candidate_elements.append( this->combineShapes( candidate_elements.at(0) , candidate_elements.at(1) ) );
            candidate_elements.append( this->combineShapes( candidate_elements.at(1) , candidate_elements.at(0) ) );

            QList<FuzzyLogic::InputMemberShipFunction> first_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( first_best );
            candidate_elements.removeOne( first_best );
            QList<FuzzyLogic::InputMemberShipFunction> second_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( second_best );

        }

        if( 1 ){

            QList< QList<FuzzyLogic::InputMemberShipFunction> > candidate_elements;

            // Select two elements to apply operators and take them out of population
            int membership_pos = qrand() % this->input_population.size();
            candidate_elements.append( this->input_population.takeAt( membership_pos % this->input_population.size() ) );
            candidate_elements.append( this->input_population.takeAt( (membership_pos+1) % this->input_population.size() ) );

            // Generate new two elements
            candidate_elements.append( this->swapShapes( candidate_elements.at(0) , candidate_elements.at(1) ) );
            candidate_elements.append( this->swapShapes( candidate_elements.at(1) , candidate_elements.at(0) ) );

            QList<FuzzyLogic::InputMemberShipFunction> first_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( first_best );
            candidate_elements.removeOne( first_best );
            QList<FuzzyLogic::InputMemberShipFunction> second_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( second_best );

        }

        if( 1 ){

            QList< QList<FuzzyLogic::InputMemberShipFunction> > candidate_elements;

            // Select two elements to apply operators and take them out of population
            int membership_pos = qrand() % this->input_population.size();
            candidate_elements.append( this->input_population.takeAt( membership_pos % this->input_population.size() ) );
            candidate_elements.append( this->input_population.takeAt( (membership_pos+1) % this->input_population.size() ) );

            // Generate new two elements
            candidate_elements.append( this->changeTriangleShape( candidate_elements.at(0) ) );
            candidate_elements.append( this->changeTriangleShape( candidate_elements.at(1) ) );

            QList<FuzzyLogic::InputMemberShipFunction> first_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( first_best );
            candidate_elements.removeOne( first_best );
            QList<FuzzyLogic::InputMemberShipFunction> second_best = this->getBestInput( candidate_elements , ruleset , output , fitness );
            this->input_population.append( second_best );
        }*/

        Debugger::debug( this , Q_FUNC_INFO , QString("Step %1").arg( step ) );
    }
}

void FuzzyLogicTrainer::executeAccumulators(){

    QString candidate_accumulator = this->accumulators.at( qrand() % this->accumulators.size() );

    // Compare the new candidate ruleset with the best
    double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , candidate_accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );
    double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

    if( candidate_fitness < best_fitness ){
        Debugger::info( this , Q_FUNC_INFO , QString("Changing to better accumulator from %1 to %2").arg( this->best_experiment.accumulator ).arg( candidate_accumulator ) );
        this->best_experiment.accumulator = candidate_accumulator;
    }
}

void FuzzyLogicTrainer::executeConjunctions(){

    QString candidate_conjunction = this->conjunctions.at( qrand() % this->conjunctions.size() );

    // Compare the new candidate ruleset with the best
    double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , candidate_conjunction , this->best_experiment.disjunction , this->best_experiment.activator );
    double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

    if( candidate_fitness < best_fitness ){
        Debugger::info( this , Q_FUNC_INFO , QString("Changing to better conjunction from %1 to %2").arg( this->best_experiment.conjunction ).arg( candidate_conjunction ) );
        this->best_experiment.conjunction = candidate_conjunction;
    }
}

void FuzzyLogicTrainer::executeDisjunctions(){

    QString candidate_disjunction = this->disjunctions.at( qrand() % this->disjunctions.size() );

    // Compare the new candidate ruleset with the best
    double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , candidate_disjunction , this->best_experiment.activator );
    double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

    if( candidate_fitness < best_fitness ){
        Debugger::info( this , Q_FUNC_INFO , QString("Changing to better disjunction from %1 to %2").arg( this->best_experiment.disjunction ).arg( candidate_disjunction ) );
        this->best_experiment.disjunction = candidate_disjunction;
    }
}

void FuzzyLogicTrainer::executeActivators(){

    QString candidate_activator = this->activators.at( qrand() % this->activators.size() );

    // Compare the new candidate ruleset with the best
    double candidate_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , candidate_activator );
    double best_fitness = this->calculateFitness( this->best_experiment.inputset , this->best_experiment.ruleset , this->best_experiment.output , this->best_experiment.accumulator , this->best_experiment.conjunction , this->best_experiment.disjunction , this->best_experiment.activator );

    if( candidate_fitness < best_fitness ){
        Debugger::info( this , Q_FUNC_INFO , QString("Changing to better activator from %1 to %2").arg( this->best_experiment.activator ).arg( candidate_activator ) );
        this->best_experiment.activator = candidate_activator;
    }
}

/**********************************************************************
   METHODS
**********************************************************************/

QList<FuzzyLogic::Rule> FuzzyLogicTrainer::getBestRuleset(QList<QList<FuzzyLogic::Rule> > rulesets, QVector<double> &return_fitness ){

    QList<FuzzyLogic::Rule> best_ruleset;

    // Create FuzzyExperiments and calculate their fitness
    QList<FuzzyLogicTrainer::FuzzyExperiment> experiments;
    return_fitness.clear();
    foreach( QList<FuzzyLogic::Rule> rs , rulesets ){
        FuzzyLogicTrainer::FuzzyExperiment exp = this->best_experiment;
        exp.ruleset = rs;

        experiments.append( exp );
        return_fitness.append( this->calculateFitness( exp.inputset , exp.ruleset , exp.output , exp.accumulator , exp.conjunction , exp.disjunction , exp.activator ) );
    }

    // Get Best
    int best_experiment_index = 0;
    for( int i = 0 ; i < return_fitness.size() ; i++ ){
        if( return_fitness.at( i ) < return_fitness.at( best_experiment_index ) ){
            best_experiment_index = i;
        }
    }

    best_ruleset = experiments.at( best_experiment_index ).ruleset;
    return best_ruleset;
}

QList< FuzzyLogic::InputMemberShipFunction > FuzzyLogicTrainer::getBestInput(QList<QList<FuzzyLogic::InputMemberShipFunction> > inputs , QVector<double> &return_fitness ){

    QList<FuzzyLogic::InputMemberShipFunction> best_input;

    // Create FuzzyExperiments and calculate their fitness
    QList<FuzzyLogicTrainer::FuzzyExperiment> experiments;
    return_fitness.clear();
    foreach( QList<FuzzyLogic::InputMemberShipFunction> is , inputs ){
        FuzzyLogicTrainer::FuzzyExperiment exp = this->best_experiment;
        exp.inputset = is;

        experiments.append( exp );
        return_fitness.append( this->calculateFitness( exp.inputset , exp.ruleset , exp.output , exp.accumulator , exp.conjunction , exp.disjunction , exp.activator ) );
    }

    // Get Best
    int best_experiment_index = 0;
    for( int i = 0 ; i < return_fitness.size() ; i++ ){
        if( return_fitness.at( i ) < return_fitness.at( best_experiment_index ) ){
            best_experiment_index = i;
        }
    }

    best_input = experiments.at( best_experiment_index ).inputset;
    return best_input;
}

/**********************************************************************
   MEMBERSHOP FUNCTION OPERATORS
**********************************************************************/


QList< FuzzyLogic::InputMemberShipFunction > FuzzyLogicTrainer::changeTriangleShape(QList< FuzzyLogic::InputMemberShipFunction > rs){

    QList< FuzzyLogic::InputMemberShipFunction > result = rs;

    int input_pos = qrand() % rs.size();

    // AVOIG GETTING TERM 0 AND LAST
    int term_pos = qrand() % ( rs.at( input_pos ).terms.size()-2 ) + 1;

    // Get left triangle center point
    double left_bound = rs.at( input_pos ).terms.at( term_pos ).shape_points.at( 0 );
    // Get left triangle center point
    double right_bound = rs.at( input_pos ).terms.at( term_pos ).shape_points.at( 2 );

    // The term that is being changed's center must be between the two bounds
    double center = (qrand() % int((right_bound*100 ) - left_bound*100) + left_bound*100) / 100.0;

    rs[ input_pos ].terms[ term_pos-1 ].shape_points[ 2 ] = center;
    rs[ input_pos ].terms[ term_pos+1 ].shape_points[ 0 ] = center;
    rs[ input_pos ].terms[ term_pos ].shape_points[ 1 ] = center;

    return result;
}

QList< FuzzyLogic::InputMemberShipFunction > FuzzyLogicTrainer::swapShapes(QList< FuzzyLogic::InputMemberShipFunction > rs1, QList< FuzzyLogic::InputMemberShipFunction > rs2){

    QList< FuzzyLogic::InputMemberShipFunction > result = rs1;

    int input_pos = qrand() % qMin( rs1.size(), rs2.size() );
    int term_pos = qrand() % qMin( rs1.at( input_pos ).terms.size(), rs2.at( input_pos ).terms.size() );

    FuzzyLogic::MemberShipFunctionTerm take_out_rs2 = rs2.at( input_pos ).terms.at( term_pos );

    result[ input_pos ].terms[ term_pos ] = take_out_rs2;

    return result;
}

QList< FuzzyLogic::InputMemberShipFunction > FuzzyLogicTrainer::combineShapes(QList< FuzzyLogic::InputMemberShipFunction > rs1, QList< FuzzyLogic::InputMemberShipFunction > rs2){

    QList< FuzzyLogic::InputMemberShipFunction > result;

    int slice_position = qrand() % qMin( rs1.size() , rs2.size() );

    QList< FuzzyLogic::InputMemberShipFunction > take_out_rs1 = rs1.mid( 0 , slice_position );
    QList< FuzzyLogic::InputMemberShipFunction > take_out_rs2 = rs2.mid( slice_position );

    result.clear();
    result.append( take_out_rs1 );
    result.append( take_out_rs2 );

    return result;
}

