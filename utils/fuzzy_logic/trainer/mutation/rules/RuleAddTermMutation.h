#ifndef RULEADDTERMMUTATION
#define RULEADDTERMMUTATION

#include "utils/fuzzy_logic/trainer/mutation/rules/RuleMutationOperator.h"

class RuleAddTermMutation : public RuleMutationOperator
{
    Q_OBJECT

public:
    RuleAddTermMutation(FuzzyLogicTrainer *parent) : RuleMutationOperator(parent){};

    virtual QList< FuzzyLogic::Rule > mutation(QList<FuzzyLogic::Rule> rs){
        QList<FuzzyLogic::Rule> result = rs;

        int rule_pos = qrand() % rs.size();

        // Create new one
        FuzzyLogic::RuleTerm term = dynamic_cast<FuzzyLogicTrainer*>( this->parent() )->generateRuleTerm();
        result[ rule_pos ].terms.append( term );

        return result;
    }
};
#endif // RULEADDTERMMUTATION

