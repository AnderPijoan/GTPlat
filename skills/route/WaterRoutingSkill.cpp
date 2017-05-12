#include "WaterRoutingSkill.h"

WaterRoutingSkill::WaterRoutingSkill( Water *skilled_agent, QString graph_name ) : RoutingSkill( skilled_agent, graph_name ){

}

/**********************************************************************
 METHODS
**********************************************************************/

void WaterRoutingSkill::calculateRoute(GSSCoordinate from, GSSCoordinate to){
    RoutingSkill::calculateRoute( from, to );
    emit this->routeCalculatedSignal();
}
