#ifndef CIVILSTRUCTURE_H
#define CIVILSTRUCTURE_H

#include "agents/places/constructions/Construction.h"

class CivilStructure : public Construction
{
    Q_OBJECT

public:
    CivilStructure(GSSGeometry* geom = 0 , QObject* parent = 0);
};

#endif // CIVILSTRUCTURE_H
