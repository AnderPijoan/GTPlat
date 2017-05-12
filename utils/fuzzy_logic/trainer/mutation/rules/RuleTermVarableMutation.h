#ifndef RULETERMVARABLEMUTATION
#define RULETERMVARABLEMUTATION

#include "utils/fuzzy_logic/trainer/mutation/rules/RuleMutationOperator.h"

class RuleTermVarableMutation : public RuleMutationOperator
{
    Q_OBJECT

public:
    RuleTermVarableMutation(FuzzyLogicTrainer *parent) : RuleMutationOperator(parent){};

    virtual QList< FuzzyLogic::Rule > mutation(QList<FuzzyLogic::Rule> rs){
        QList<FuzzyLogic::Rule> result = rs;

        int rule_pos = qrand() % result.size();
        // Select term pos to be changed
        int term_pos = qrand() % result[ rule_pos ].terms.size();

        QString input_name = rs[ rule_pos ].terms[ term_pos ].input_name;
        QStringList values = dynamic_cast<FuzzyLogicTrainer*>( this->parent() )->getInputNames().value( input_name );
        result[ rule_pos ].terms[ term_pos ].term_name = values.at( qrand() % values.size() );

        return result;
    }
};


#endif // RULETERMVARABLEMUTATION

