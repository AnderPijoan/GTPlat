#ifndef GSSGRID_H
#define GSSGRID_H

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QImage>

#include "utils/geometry/GSSCoordinate.h"
#include "utils/geometry/GSSEnvelope.h"

QT_FORWARD_DECLARE_CLASS( GeometryController )

class GSSGrid : public QObject
{
    Q_OBJECT

    //[xy]
    //[03 , 13 , 23 , 33]
    //[02 , 12 , 22 , 32]
    //[01 , 11 , 21 , 31]
    //[00 , 10 , 20 , 30]

public:
    explicit GSSGrid(double left , double right , double top , double bottom , unsigned int x_size = 100 , unsigned int y_size = 100 , double min_value = NAN, double max_value = NAN , QObject *parent = 0);
    GSSGrid(const GSSGrid&);
    GSSGrid();
    ~GSSGrid();

    // EXPORTERS
    QImage toImage( const GSSEnvelope image_bounds , int image_width = 1024, int image_height = 1024 ) const;

    // GETTERS
    bool isEmpty() const;
    const GSSEnvelope getBounds() const;
    unsigned int getXSize() const;
    unsigned int getYSize() const;
    double getMaxValue() const;
    double getMinValue() const;
    virtual double getValue( GSSCoordinate coor ) const;
    const GSSEnvelope getCellEnvelope( unsigned int grid_x , unsigned int grid_y ) const;

    // SETTERS
    void setBounds( double left , double right , double top , double bottom );
    void setBounds( GSSEnvelope bounds );
    void setSize( unsigned int x_size = 100 , unsigned int y_size = 100 );
    void setValue( GSSCoordinate coor , double v );

    // OPERATORS
    GSSGrid& operator=(const GSSGrid&);

    // METHODS
    GSSGrid getSubGrid( GSSEnvelope bounds );
    QList<GSSEnvelope> getSurroundingCells( GSSCoordinate coor );

protected:
    // GETTERS
    double getValue( unsigned int grid_x , unsigned int grid_y ) const;
    // SETTERS
    void setValue( unsigned int grid_x , unsigned int grid_y , double v);

private:
    GSSEnvelope bounds;
    QVector< QVector<double> > values;
    double min_value;
    double max_value;
};

#endif // GSSGRID_H
