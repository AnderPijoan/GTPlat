#include "InterpolationGrid.h"

#include "utils/conversors/grid_coordinates_conversor/GridCoordinatesConversor.h"

InterpolationGrid::InterpolationGrid(double left, double right, double top, double bottom, unsigned int x_size, unsigned int y_size, double min_value, double max_value, QObject *parent) : GSSGrid( left , right , top , bottom , x_size , y_size , min_value , max_value , parent){
}

InterpolationGrid::InterpolationGrid(const InterpolationGrid &other) : GSSGrid( other ){
}

InterpolationGrid::InterpolationGrid(const GSSGrid &other) : GSSGrid( other ){
}

InterpolationGrid::InterpolationGrid(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

double InterpolationGrid::getValue(GSSCoordinate coor) const{

    // Get value
    double value = GSSGrid::getValue( coor );
    if( value <= this->getMinValue() - 1 ){
        return value;
    }

    // TODO

     return value;
}

