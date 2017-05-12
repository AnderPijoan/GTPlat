#ifndef RANDOMVEHICLEDRIVER_H
#define RANDOMVEHICLEDRIVER_H

#include "agents/people/vehicle_drivers/VehicleDriver.h"

/**
 * @brief The RandomVehicleDriver class
 * It will go adding random roads to the
 * viewing_roads and follow them
 */
class RandomVehicleDriver : public VehicleDriver
{
    Q_OBJECT
public:
    RandomVehicleDriver(Vehicle* vehicle = 0, PersonConstants::gender gender = PersonConstants::MALE);

    // METHODS
    virtual void updateViewingRoads();

protected slots:
    // Start the agents behaviour
    virtual void agentStarted();
    virtual void moveToNextEdge();
    virtual void vehicleCantEnterRoad();

};

#endif // RANDOMVEHICLEDRIVER_H
