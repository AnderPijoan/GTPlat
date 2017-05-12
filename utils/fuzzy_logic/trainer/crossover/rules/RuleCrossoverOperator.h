#ifndef RULECROSSOVEROPERATOR_H
#define RULECROSSOVEROPERATOR_H

#include <QObject>
#include "utils/fuzzy_logic/FuzzyLogic.h"
#include "utils/fuzzy_logic/trainer/FuzzyLogicTrainer.h"

class RuleCrossoverOperator : public QObject
{
    Q_OBJECT
public:

    explicit RuleCrossoverOperator(FuzzyLogicTrainer *parent) : QObject(parent){};

    virtual QList< FuzzyLogic::Rule > crossover( QList< FuzzyLogic::Rule > rs1 , QList< FuzzyLogic::Rule > rs2 ) = 0;
};

#endif // RULECROSSOVEROPERATOR_H
