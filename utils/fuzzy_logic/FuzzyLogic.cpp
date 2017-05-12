#include "FuzzyLogic.h"

#include <QtMath>

#include "utils/exporters/debugger/Debugger.h"

FuzzyLogic::FuzzyLogic( QObject *parent) : QObject( parent ){
    this->fuzzy_logic_engine = new fl::Engine();
}


FuzzyLogic::~FuzzyLogic(){
    delete this->fuzzy_logic_engine;
}

/**********************************************************************
  GETTERS
**********************************************************************/

double FuzzyLogic::getOutputValue(QString variable_name){
    if( !this->fuzzy_logic_engine->hasOutputVariable( variable_name.toStdString() ) ){
        Debugger::warning( this , Q_FUNC_INFO , QString("Variable %1 not in fuzzy logic").arg( variable_name ) );
    }
    return this->fuzzy_logic_engine->getOutputValue( variable_name.toStdString() );
}

QString FuzzyLogic::getOutputValueAsName(QString variable_name){

    double v = this->getOutputValue( variable_name );
    if( v > this->output_variable_names.size() || v < 0 ){
        return "nan";
    }
    if( v == v ){ // Avoid nan
        return this->output_variable_names.at( qFloor( v ) );
    }
    return "nan";
}

QString FuzzyLogic::getOutputValueWeights(QString variable_name){
    return QString::fromStdString( this->fuzzy_logic_engine->getOutputVariable( variable_name.toStdString() )->fuzzyOutputValue() );
}

double FuzzyLogic::getInputValue(QString variable_name){
    return this->fuzzy_logic_engine->getInputVariable( variable_name.toStdString() )->getInputValue();
}

bool FuzzyLogic::hasInputVariable(QString variable_name){
    return this->fuzzy_logic_engine->hasInputVariable( variable_name.toStdString() );
}

bool FuzzyLogic::isReady(){
    return this->fuzzy_logic_engine->isReady();
}

/**********************************************************************
  GETTERS
**********************************************************************/

void FuzzyLogic::setInputValue(QString input_name, double value){
    this->fuzzy_logic_engine->setInputValue( input_name.toStdString() , value );
    Debugger::debug( this , Q_FUNC_INFO , QString("VARIABLE %1 given value %2").arg( input_name ).arg( value ) );
}

/**********************************************************************
   METHODS
**********************************************************************/

void FuzzyLogic::setParameters(QList<FuzzyLogic::InputMemberShipFunction> inputs, QList<FuzzyLogic::Rule> rules, FuzzyLogic::OutputMembershipFunction output){

    this->fuzzy_logic_engine->restart();

    // Create all input membership functions
    foreach (InputMemberShipFunction i , inputs) {

        // Create variable
        fl::InputVariable* input_variable = new fl::InputVariable( i.input_name.toStdString() , 0 , 1 );
        this->addTermsToVariable( input_variable , i.terms );
        input_variable->setInputValue( -1 );
        this->fuzzy_logic_engine->addInputVariable( input_variable );
    }

    // Create output membership function
    fl::OutputVariable* output_variable = new fl::OutputVariable( output.output_name.toStdString() );
    this->addTermsToVariable( output_variable , output.terms );
    output_variable->setOutputValue( 10+output.terms.size() );
    foreach( MemberShipFunctionTerm t , output.terms ){
        this->output_variable_names.append( t.name );
    }
    this->fuzzy_logic_engine->addOutputVariable( output_variable );

    // Create rules
    fl::RuleBlock* ruleblock = new RuleBlock( "RULES" );
    foreach( Rule rule , rules ){
        QString str = QString("if ");

        for( int i = 0 ; i < rule.terms.size() ; i++){
            RuleTerm rt = rule.terms.at( i );
            if( i > 0 ){ str += "and "; }
            str += QString("%1 is %2 ").arg( rt.input_name ).arg( rt.term_name );
        }

        str += QString("then %1 is %2").arg( QString::fromStdString( output_variable->getName() ) ).arg( rule.output_value );
        ruleblock->addRule( fl::Rule::parse( str.toStdString() , this->fuzzy_logic_engine ));
    }
    this->fuzzy_logic_engine->addRuleBlock( ruleblock );
}

void FuzzyLogic::configure(QString conjunctionT, QString disjunctionS, QString activationT, QString accumulationS, QString defuzzifier, int resolution){

    this->fuzzy_logic_engine->configure( conjunctionT.toStdString() , disjunctionS.toStdString() , activationT.toStdString() , accumulationS.toStdString() , defuzzifier.toStdString() , resolution );

    std::string status;
    if( !this->fuzzy_logic_engine->isReady( &status ) ){
        Debugger::warning( this , Q_FUNC_INFO , QString("Fuzzy Logic engine not ready : %1").arg( QString::fromStdString( status ) ) );
    }
}

void FuzzyLogic::resetInputValues(){
    std::vector<InputVariable*> variables = this->fuzzy_logic_engine->inputVariables();
    foreach( InputVariable* v , variables ){
        v->setInputValue( fl::nan );
    }
}

void FuzzyLogic::process(){
    this->fuzzy_logic_engine->process();
}


/**********************************************************************
  PRIVATE METHODS
**********************************************************************/

void FuzzyLogic::addTermsToVariable(fl::Variable* var, QList< FuzzyLogic::MemberShipFunctionTerm > terms ){

    // Create shapes
    double max_value = -1;
    double min_value = 999999;

    foreach( MemberShipFunctionTerm t , terms ){

        max_value = qMax( max_value , t.shape_points.last() );
        min_value = qMin( min_value , t.shape_points.first() );

        // If 2 values give, shape is rectangle
        if( t.shape_points.size() == 2 ){
            fl::Rectangle* rec = new fl::Rectangle( t.name.toStdString() , t.shape_points.at(0) , t.shape_points.at(1) );
            var->addTerm( rec );
            Debugger::debug( this , Q_FUNC_INFO , QString("VARIABLE %1 : Added rectangle %2 (%3 , %4)").arg( QString::fromStdString( var->getName() ) ).arg( t.name ).arg( t.shape_points.at(0) ).arg( t.shape_points.at(1) ) );
        }

        // If 3 values given, shape is triangle
        if( t.shape_points.size() == 3 ){
            fl::Triangle* tri = new fl::Triangle( t.name.toStdString() , t.shape_points.at(0) , t.shape_points.at(1) , t.shape_points.at(2) );
            var->addTerm( tri );
            Debugger::debug( this , Q_FUNC_INFO , QString("VARIABLE %1 : Added triangle %2 (%3 , %4 , %5)").arg( QString::fromStdString( var->getName() ) ).arg( t.name ).arg( t.shape_points.at(0) ).arg( t.shape_points.at(1) ).arg( t.shape_points.at(2) ) );
        }
    }

    var->setMinimum( min_value );
    var->setMaximum( max_value );
}
