#ifndef GTPLATFUZZYLOGICTRAINER_H
#define GTPLATFUZZYLOGICTRAINER_H

#include "utils/fuzzy_logic/trainer/FuzzyLogicTrainer.h"
#include "usecases/gtplat/GTPlatAgent.h"

class GTPlatFuzzyLogicTrainer : public FuzzyLogicTrainer
{
    Q_OBJECT
public:
    explicit GTPlatFuzzyLogicTrainer( QList<GTPlatAgent*> agents , QMap<QString , QStringList> input_values , QStringList outputs , QObject *parent = 0 );

    // FITNESS
    virtual double calculateFitness( QList< FuzzyLogic::InputMemberShipFunction > input , QList< FuzzyLogic::Rule > ruleset , FuzzyLogic::OutputMembershipFunction output , QString accumulator , QString conjunction , QString disjunction , QString activator ); // IMPORTANT!! The lower, the better

    QList<GTPlatAgent*> agents;
    QMap<QString , int> agent_options;
    double max_duration = 0;
    double max_price = 0;
    double max_pollution = 0;
    double max_length = 0;
};

#endif // GTPLATFUZZYLOGICTRAINER_H
