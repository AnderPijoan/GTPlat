#ifndef RULEREMOVEMUTATION
#define RULEREMOVEMUTATION

#include "utils/fuzzy_logic/trainer/mutation/rules/RuleMutationOperator.h"

class RuleRemoveMutation : public RuleMutationOperator
{
    Q_OBJECT

public:
    RuleRemoveMutation(FuzzyLogicTrainer *parent) : RuleMutationOperator(parent){};

    virtual QList< FuzzyLogic::Rule > mutation(QList<FuzzyLogic::Rule> rs){
        QList<FuzzyLogic::Rule> result = rs;

        result.removeAt( qrand() % result.size() );

        return result;
    }
};


#endif // RULEREMOVEMUTATION

