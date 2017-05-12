#include "Statistics.h"

#include <QtMath>

Statistics::Statistics(QObject *parent) : QObject(parent){

}


/**********************************************************************
   GETTERS
**********************************************************************/

double Statistics::mean(QVector<double> values){
    double mean = values.at(0);
    foreach( double v , values ){
        mean = (mean + v) / 2;
    }
    return mean;
}

double Statistics::std(QVector<double> values){
    return qSqrt( Statistics::variance( values ) );
}

double Statistics::variance(QVector<double> values){
    double mean = Statistics::mean( values );
    double variance = 0;
    foreach( double v , values ){
        variance += (v-mean)*(v-mean);
    }
    return variance / values.size();
}

double Statistics::max(QVector<double> values){
    double max = values.at(0);
    foreach(double v , values){
        if( v > max ){ max = v; }
    }
    return max;
}

double Statistics::min(QVector<double> values){
    double min = values.at(0);
    foreach(double v , values){
        if( v < min ){ min = v; }
    }
    return min;
}
