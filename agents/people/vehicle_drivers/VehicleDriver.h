#ifndef VEHICLEDRIVER_H
#define VEHICLEDRIVER_H

#include "agents/people/Person.h"
#include "agents/vehicles/Vehicle.h"
#include "agents/transport_networks/TransportLine.h"

#include "skills/view/ViewRouteAheadSkill.h"
#include "skills/move/drive/DriveSkill.h"
#include "skills/route/TransportLineRoutingSkill.h"

class VehicleDriver : public Person
{
    Q_OBJECT

public:
    VehicleDriver(Vehicle* vehicle = 0, PersonConstants::gender gender = PersonConstants::MALE , GSSGeometry* geom = 0 , QObject* parent = 0);
    ~VehicleDriver();
    virtual QJsonObject toJSON();

    // GETTERS
    Vehicle* getVehicle();
    double getSpeedFactor();
    double getSpeedDeviationFactor();

    // SETTERS
    virtual void setVehicle(Vehicle* vehicle);
    void setDestination( GSSCoordinate destination);
    void setSpeedFactor(double speed_factor);
    void setSpeedDeviationFactor(double speed_deviation_factor);
    void setViewDistance(double view_distance);
    void setStopDistance(double stop_distance);

private slots:
    // Agents behaviour
    virtual void behave();

protected slots:
    virtual void calculateRouteToVehicle();
    virtual void followWalkRoute();
    virtual void getInsideVehicleAndDrive();
    virtual void followDriveRoute();
    virtual void getOutsideVehicleAndWalk();
    virtual void routeFinished();

protected:

    Vehicle* vehicle;
    GSSCoordinate destination;

    int status = 0;

};

#endif // VEHICLEDRIVER_H
