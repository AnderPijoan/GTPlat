#ifndef ADMINISTRATIVEAREA_H
#define ADMINISTRATIVEAREA_H

#include "agents/places/Place.h"

class AdministrativeArea : public Place
{
    Q_OBJECT

public:
    AdministrativeArea( GSSGeometry* geom = 0 , QObject* parent = 0 );
};

#endif // ADMINISTRATIVEAREA_H
