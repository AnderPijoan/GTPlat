#ifndef INTERPOLATIONGRID_H
#define INTERPOLATIONGRID_H

#include "utils/grid/GSSGrid.h"

class InterpolationGrid : public GSSGrid
{
    Q_OBJECT

public:
    explicit InterpolationGrid(double left , double right , double top , double bottom , unsigned int x_size = 100 , unsigned int y_size = 100 , double min_value = NAN, double max_value = NAN , QObject *parent = 0);
    InterpolationGrid(const InterpolationGrid&);
    InterpolationGrid(const GSSGrid&);
    InterpolationGrid();

    // GETTERS
    virtual double getValue(GSSCoordinate coor) const;

};

#endif // INTERPOLATIONGRID_H
