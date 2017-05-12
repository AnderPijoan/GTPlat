#include "ViewRouteAheadSkill.h"

#include "agents/transport_networks/TransportLine.h"

ViewRouteAheadSkill::ViewRouteAheadSkill(Agent *agent) : Skill(agent){
    this->view_distance = LengthUnit( 100 ); // Can see 100meters ahead

    // Route
    this->current_traffic_level = 1 ;
    this->current_free_distance = this->view_distance;

}

/**********************************************************************
 GETTERS
**********************************************************************/

double ViewRouteAheadSkill::getTrafficLevel(){
    return this->current_traffic_level;
}

LengthUnit ViewRouteAheadSkill::getFreeDistance(){
    return this->current_free_distance;
}

LengthUnit ViewRouteAheadSkill::getViewDistance(){
    return this->view_distance;
}

const QList<GSSGraphEdge*> ViewRouteAheadSkill::getViewingEdges(){
    return this->viewing_edges;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void ViewRouteAheadSkill::setViewDistance(LengthUnit view_distance){
    this->view_distance = view_distance;
}

/**********************************************************************
 METHODS
**********************************************************************/

void ViewRouteAheadSkill::updateViewingRoads(QList<GSSGraphEdge*> remaining_route){

    // Reset
    this->current_free_distance = LengthUnit( 0.0 );
    this->current_traffic_level = 1.0;

    // Calculate how many occupied roads we see and where it is the next stop
    double occupied_roads_amount = 0;
    this->current_free_distance = this->view_distance;
    LengthUnit viewed_distance = LengthUnit( 0 );
    this->viewing_edges.clear();

    int edge_index = 0;

    while( viewed_distance < this->view_distance && edge_index < remaining_route.size() ){
        GSSGraphEdge* e = remaining_route.at( edge_index );
        viewed_distance = viewed_distance + e->getLength();
        this->viewing_edges.append( e );

        TransportLine* t = dynamic_cast<TransportLine*>( e );
        if( t->isPermanentlyClosed() || t->isTemporarilyClosed() || t->isFull() ){
            this->current_free_distance = this->current_free_distance < viewed_distance ? this->current_free_distance : viewed_distance;
            occupied_roads_amount++;
        }
        edge_index++;
    }

    this->current_traffic_level = this->viewing_edges.isEmpty() ? 0.0 :  1.0 - ( occupied_roads_amount / this->viewing_edges.size() );
}
