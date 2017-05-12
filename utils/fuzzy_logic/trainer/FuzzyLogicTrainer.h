#ifndef FUZZYLOGICTRAINER_H
#define FUZZYLOGICTRAINER_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QVector>

#include "usecases/gtplat/GTPlatItinerary.h"
#include "utils/geometry/GSSCoordinate.h"
#include "utils/random_generators/UniformDistribution.h"
#include "utils/fuzzy_logic/FuzzyLogic.h"

QT_FORWARD_DECLARE_CLASS(RuleCrossoverOperator)
QT_FORWARD_DECLARE_CLASS(RuleMutationOperator)

class FuzzyLogicTrainer : public QObject
{
    Q_OBJECT
public:
    explicit FuzzyLogicTrainer( QMap<QString , QStringList> input_values , QStringList outputs , QObject *parent = 0);
    ~FuzzyLogicTrainer();

    // STRUCTS
    struct FuzzyExperiment {
        // Fuzzy Logic data
        QList< FuzzyLogic::InputMemberShipFunction > inputset;
        QList< FuzzyLogic::Rule > ruleset;
        FuzzyLogic::OutputMembershipFunction output;
        // Fuzzy logic parameters
        QString accumulator;
        QString conjunction;
        QString disjunction;
        QString activator;
        inline QString toString() const {
            QString str = QString("[%1,%2,%3,%4] ").arg( this->accumulator ).arg( this->conjunction ).arg( this->disjunction ).arg( this->activator );
            foreach(FuzzyLogic::Rule r , this->ruleset ){
                str += QString("if ");
                foreach(FuzzyLogic::RuleTerm rt , r.terms ){ str += QString("(%1 is %2) ").arg( rt.input_name ).arg( rt.term_name ); }
                str += QString("then %1 ").arg( r.output_value );
            }
            return str;
        }
    };

    // GETTERS
    QStringList getOutputNames();
    QMap<QString , QStringList> getInputNames();

    // GENERATORS
    QList< FuzzyLogic::InputMemberShipFunction > generateInputset();
    QList< FuzzyLogic::Rule > generateRuleset();
    FuzzyLogic::Rule generateRule();
    FuzzyLogic::RuleTerm generateRuleTerm();
    FuzzyLogic::OutputMembershipFunction generateOutput();

    // METHODS
    virtual void init();
    virtual void execute();

    // FITNESS
    virtual double calculateFitness( QList< FuzzyLogic::InputMemberShipFunction > inputset , QList< FuzzyLogic::Rule > ruleset , FuzzyLogic::OutputMembershipFunction output , QString accumulator , QString conjunction , QString disjunction , QString activator ) = 0; // IMPORTANT!! The lower, the better

protected:

    // GENETIC ALGORITHM FUNCTIONS
    void executeRules();
    void executeInputs();
    void executeAccumulators();
    void executeConjunctions();
    void executeDisjunctions();
    void executeActivators();

    QList< FuzzyLogic::Rule > getBestRuleset( QList< QList< FuzzyLogic::Rule > > rules , QVector<double> &return_fitness );
    QList< FuzzyLogic::InputMemberShipFunction > getBestInput( QList< QList< FuzzyLogic::InputMemberShipFunction > > inputs , QVector<double> &return_fitness );

    // GENETIC ALGORITHM OPERATORS FOR RULES
    QList<RuleCrossoverOperator*> rule_crossovers;
    QList<RuleMutationOperator*> rule_mutations;

    // GENETIC ALGORITHM OPERATORS FOR INPUTS
    QList< FuzzyLogic::InputMemberShipFunction > changeTriangleShape( QList< FuzzyLogic::InputMemberShipFunction > rs );
    QList< FuzzyLogic::InputMemberShipFunction > swapShapes( QList< FuzzyLogic::InputMemberShipFunction > rs1 , QList< FuzzyLogic::InputMemberShipFunction > rs2 );
    QList< FuzzyLogic::InputMemberShipFunction > combineShapes( QList< FuzzyLogic::InputMemberShipFunction > rs1 , QList< FuzzyLogic::InputMemberShipFunction > rs2 );

    // GENETIC ALGORITHM POPULATION
    UniformDistribution* distribution;
    QList< QList< FuzzyLogic::Rule > > rule_population;
    QList< QList< FuzzyLogic::InputMemberShipFunction > > input_population;

    // BEST FUZZY EXPERIMENT FOUND SO FAR
    FuzzyLogicTrainer::FuzzyExperiment best_experiment;

    // CONSTRUCTION DATA FOR GENERATING POPULATION
    QStringList output_names; // CAR, BICYCLE, MOTORCYCLE, TRANSIT...
    QMap<QString , QStringList > input_names; // QMAP < PRICE < QSTRINGLIST( CHEAP, EXPENSIVE, ..) > , TIME < QSTRINGLIST( LOW, MEDIUM, HIGH,... ) > >
    QStringList accumulators;
    QStringList conjunctions;
    QStringList disjunctions;
    QStringList activators;
};

#endif // FUZZYLOGICTRAINER_H
