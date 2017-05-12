#include "ExecutionEnvironment.h"

#include "environment/Environment.h"


ExecutionEnvironment::ExecutionEnvironment(Environment *parent) : QObject(parent) , cycle_frequency_milliseconds(1000) {
}

ExecutionEnvironment::~ExecutionEnvironment(){
}

/**********************************************************************
 GETTERS
**********************************************************************/

double ExecutionEnvironment::getCycleFrequency() const{
    return this->cycle_frequency_milliseconds;
}

/**********************************************************************
 SETTERS
**********************************************************************/

void ExecutionEnvironment::addAgent(Agent *agent){
    this->running_agents.insert( agent->getId() , agent );
    agent->run();
}

void ExecutionEnvironment::removeAgent(Agent *agent){
    agent->stop();
    this->running_agents.remove( agent->getId() );
}

int ExecutionEnvironment::getRunningAgents(){
    return this->running_agents.keys().size();
}

void ExecutionEnvironment::setCycleFrequency(double cycle_frequency_milliseconds){
    this->cycle_frequency_milliseconds = cycle_frequency_milliseconds;
}

/**********************************************************************
 METHODS
**********************************************************************/

void ExecutionEnvironment::executeAgents(TimeEnvironment* time_environment){

    // Wait for agents that are delayed.
    // Get min tick time and add some threshold to execute the agents that are more delayed.
    QDateTime min_tick = time_environment->getCurrentDateTime();
    QList<Agent*> currently_running_agents = this->running_agents.values();

    foreach( Agent* agent , currently_running_agents ){
        if( agent && agent->running && !agent->busy && agent->next_tick_datetime.isValid() ){
            min_tick = qMin( min_tick , agent->next_tick_datetime );
        }
    }

    Debugger::info( this , Q_FUNC_INFO , QString("Environment datetime: %1 , Executing tick: %2 , Running agents: %3")
                                        .arg( time_environment->getCurrentDateTime().time().toString( "hh:mm:ss" ) )
                                        .arg( min_tick.time().toString( "hh:mm:ss" ) )
                                        .arg( currently_running_agents.size() ) );

    QDateTime min_tick_threshold = min_tick.addSecs( 5 ); // Add some threshold
    int ticked_agents = 0;

    // Update TimeEnvironment datetime
    time_environment->setDatetime( min_tick.addMSecs( this->cycle_frequency_milliseconds * time_environment->getTimeSpeed() ) );

    // Execute lower than min_tick agents
    foreach( Agent* agent , currently_running_agents ){

        if( agent && agent->running && !agent->busy && agent->environment == this->parent() && ( !agent->next_tick_datetime.isValid() || agent->next_tick_datetime <= min_tick_threshold ) ){

            agent->busy = true;
            ticked_agents++;

            // Call behave through behaveWrapper for it to be executed in the agents thread (avoid msec = 0)
            agent->timer->singleShot( 1 + (qrand() * agent->getId()) % (int)this->cycle_frequency_milliseconds , Qt::PreciseTimer , agent , &Agent::behaveWrapper );
        }
    }

    Debugger::info( this , Q_FUNC_INFO ,    QString("Tick end with %1 executed agents").arg( ticked_agents ) );
}
