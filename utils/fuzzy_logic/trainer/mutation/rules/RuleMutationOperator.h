#ifndef RULEMUTATIONOPERATOR_H
#define RULEMUTATIONOPERATOR_H

#include <QObject>
#include "utils/fuzzy_logic/FuzzyLogic.h"
#include "utils/fuzzy_logic/trainer/FuzzyLogicTrainer.h"

class RuleMutationOperator : public QObject
{
    Q_OBJECT
public:

    explicit RuleMutationOperator(FuzzyLogicTrainer *parent) : QObject(parent){};

    virtual QList< FuzzyLogic::Rule > mutation( QList< FuzzyLogic::Rule > r ) = 0;
};

#endif // RULEMUTATIONOPERATOR_H
