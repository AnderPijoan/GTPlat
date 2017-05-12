#ifndef RULEADDMUTATION
#define RULEADDMUTATION

#include "utils/fuzzy_logic/trainer/mutation/rules/RuleMutationOperator.h"

class RuleAddMutation : public RuleMutationOperator
{
    Q_OBJECT

public:
    RuleAddMutation(FuzzyLogicTrainer *parent) : RuleMutationOperator(parent){};

    virtual QList< FuzzyLogic::Rule > mutation(QList<FuzzyLogic::Rule> rs){
        QList<FuzzyLogic::Rule> result = rs;

        FuzzyLogic::Rule rule = dynamic_cast<FuzzyLogicTrainer*>( this->parent() )->generateRule();
        result.append( rule );

        return result;
    }
};

#endif // RULEADDMUTATION

