#ifndef ITINERARYVEHICLEDRIVER_H
#define ITINERARYVEHICLEDRIVER_H

#include "agents/people/vehicle_drivers/VehicleDriver.h"

class ItineraryVehicleDriver : public VehicleDriver
{
    Q_OBJECT

public:
    ItineraryVehicleDriver(Vehicle* vehicle = 0, PersonConstants::gender gender = PersonConstants::MALE , GSSGeometry* geom = 0 , QObject* parent = 0);

    struct ScheduledStop {
        Agent* stop;  // The stop it arrives at
        QDateTime arrival;
        QDateTime departure;
    };

    // GETTERS
    virtual QList<ItineraryVehicleDriver::ScheduledStop> getItinerary();

    // SETTERS
    virtual void setItinerary(QList<ItineraryVehicleDriver::ScheduledStop> itinerary);

protected slots:
    virtual void behave();
    virtual void getInsideVehicleAndDrive();
    virtual void calculateRouteToNextPoint();
    virtual void stopAtItineraryPoint();
    virtual void departFromItineraryPoint();

protected:

    QList<ItineraryVehicleDriver::ScheduledStop> itinerary; // Itinerary
    QList<ItineraryVehicleDriver::ScheduledStop> remaining_stops; // Copy of the itinerary that goes popping visited stops

private:

};

#endif // ITINERARYVEHICLEDRIVER_H
