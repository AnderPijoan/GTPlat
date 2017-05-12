#ifndef ACCOMMODATION_H
#define ACCOMMODATION_H

#include "agents/places/constructions/Construction.h"

class Accommodation : public Construction
{
    Q_OBJECT

public:
    Accommodation( GSSGeometry* geom , QObject* parent = 0);

};

#endif // ACCOMMODATION_H
