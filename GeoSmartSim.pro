#-------------------------------------------------
#
# Project created by QtCreator 2014-09-04T11:58:49
#
#-------------------------------------------------

QT          += core
QT          += gui
QT          += network
QT          += websockets # sudo apt-get install libqt5websockets5-dev
QT          += sql # sudo apt-get install libqt5sql5 libqt5sql5-psql
QT          += KUnitConversion # sudo apt-get install libkf5unitconversion-dev
QT          += xml
QT          += network
QT          += qml # For creating agents from QML

# ACTIVATE QT-COMPILER OPTIMIZATION
# march=native
# 02 -> Optimizations level 2
# pipe
QMAKE_CXXFLAGS += -march=native -O2 -pipe
QMAKE_CFLAGS += -march=native -O2 -pipe

# FORCE C++11
QMAKE_CXXFLAGS += -std=c++11

TARGET      = GeoSmartSim
CONFIG      += console
CONFIG      -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
        # AGENTS
    agents/Agent.cpp \
            ## PEOPLE
    agents/people/Person.cpp \
                ### VEHICLE DRIVERS
    agents/people/vehicle_drivers/VehicleDriver.cpp \
    agents/people/vehicle_drivers/ItineraryVehicleDriver.cpp \
    agents/people/vehicle_drivers/PublicTransportVehicleDriver.cpp \
    agents/people/public_transport_users/PublicTransportUserPerson.cpp \
            ## VEHICLES
    agents/vehicles/Vehicle.cpp \
    agents/vehicles/Car.cpp \
    agents/vehicles/Motorcycle.cpp \
    agents/vehicles/Bicycle.cpp \
    agents/vehicles/ECar.cpp \
    agents/vehicles/Truck.cpp \
    agents/vehicles/public_transport/PublicTransportVehicle.cpp \
    agents/vehicles/public_transport/Bus.cpp \
    agents/vehicles/public_transport/Train.cpp \
    agents/vehicles/public_transport/Metro.cpp \
    agents/vehicles/public_transport/Tram.cpp \
    agents/vehicles/public_transport/Ferry.cpp \
    agents/vehicles/public_transport/Funicular.cpp \
            ## PLACES
    agents/places/Place.cpp \
    agents/places/administrative_areas/AdministrativeArea.cpp \
    agents/places/constructions/Construction.cpp \
                ### ACCOMMODATIONS
    agents/places/constructions/accommodations/Accommodation.cpp \
                ### CIVIL STRUCTURES
    agents/places/constructions/civil_structures/CivilStructure.cpp \
    agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.cpp \
    agents/places/constructions/civil_structures/public_transport_stops/BusStop.cpp \
    agents/places/constructions/civil_structures/public_transport_stops/RailwayStop.cpp \
    agents/places/constructions/civil_structures/parkings/Parking.cpp \
    agents/places/constructions/civil_structures/vehicle_rentals/BicycleRental.cpp \
    agents/places/constructions/civil_structures/vehicle_rentals/VehicleRental.cpp \
    agents/places/constructions/civil_structures/charging_stations/ChargingStation.cpp \
            ## TRANSPORT NETWORKS
    agents/transport_networks/TransportLine.cpp \
    agents/transport_networks/TransportNode.cpp \
    agents/transport_networks/elements/TrafficLight.cpp \
                ### ROADS
    agents/transport_networks/roads/Road.cpp \
                ### RAILWAYS
    agents/transport_networks/railways/Railway.cpp \
                ### WALK PATHS
    agents/transport_networks/walk_paths/WalkPath.cpp \
                ### PUBLIC TRANSPORT
    agents/transport_networks/public_transport/PublicTransportStretch.cpp \
        # UTILS
            ## PARALLELISM CONTROLLER
    utils/paralelism_controller/ParallelismController.cpp \
            ## GEOMETRY
    utils/geometry/GSSGeometryController.cpp \
    utils/geometry/GSSGeometry.cpp \
    utils/geometry/GSSEnvelope.cpp \
    utils/geometry/GSSCoordinate.cpp \
    utils/geometry/GSSPoint.cpp \
    utils/geometry/GSSLineString.cpp \
    utils/geometry/GSSPolygon.cpp \
    utils/geometry/GSSGeometryUtils.cpp \
    utils/geometry/GSSQuadtree.cpp \
            ## GRID
    utils/grid/GSSGrid.cpp \
    utils/grid/InterpolationGrid.cpp \
            ## VORONOI
    utils/voronoi/Voronoi.cpp \
            ## HTTP SERVER
    utils/http_server/HttpServer.cpp \
    utils/http_server/HttpRequestHandler.cpp \
            ## WEBSOCKET SERVER
    utils/websocket_server/WebSocketServer.cpp \
            ## TILE SERVER
    utils/tile_server/TileServer.cpp \
    utils/tile_server/TileRequestHandler.cpp \
            ## CONVERSORS
    utils/conversors/projection_conversor/ProjectionConversor.cpp \
    utils/conversors/encoded_polyline_conversor/EncodedPolylineConversor.cpp \
    utils/conversors/tile_coordinates_conversor/TileCoordinatesConversor.cpp \
    utils/conversors/grid_coordinates_conversor/GridCoordinatesConversor.cpp \
    utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.cpp \
            ## EXPORTERS
    utils/exporters/custom_json_exporter/CustomJsonExporter.cpp \
#    utils/exporters/agent_to_shp/AgentToShp.cpp \
    utils/exporters/logger/Logger.cpp \
    utils/exporters/debugger/Debugger.cpp \
            ## IMPORTERS
    utils/importers/Importer.cpp \
    #utils/importers/shp_importer/ShpImporter.cpp \
    utils/importers/json_importer/JsonImporter.cpp \
    utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.cpp \
    utils/importers/gtfs_importer/GtfsImporter.cpp \
    utils/importers/csv_importer/CsvImporter.cpp \
    utils/importers/gpx_importer/GpxImporter.cpp \
    utils/importers/ine_importer/IneMicroDataImporter.cpp \
    utils/importers/osm_importer/OsmImporter.cpp \
    utils/importers/tiff_importer/TiffImporter.cpp \
            ## SORTERS
    utils/sorters/NumberSorter.cpp \
            ## SERVICE AREA
    utils/service_area/ServiceAreaCalculator.cpp \
    utils/service_area/graph/GraphServiceAreaCalculator.cpp \
            ## UI STYLE
    utils/uistyle/UiStyle.cpp \
            ## DB
    utils/db/PostgresDriver.cpp \
            ## API
    utils/api/APIDriver.cpp \
    utils/api/SyncAPIDriver.cpp \
    utils/api/ASyncAPIDriver.cpp \
    utils/api/NoReplyAPIDriver.cpp \
    utils/api/openstreetmap/OsmAPIDriver.cpp \
    utils/api/nominatim/NominatimAPIDriver.cpp \
            ## IMAGE DOANLOADER
    utils/image_downloader/ImageDownloader.cpp \
            ## GRAPHS
    utils/graph/GSSGraph.cpp \
    utils/graph/GSSGraphUtils.cpp \
    utils/graph/GSSGraphEdge.cpp \
    utils/graph/GSSGraphNode.cpp \
            ## HEATMAP
    utils/heatmap/GSSHeatMap.cpp \
            ## ROUTING
    utils/routing/GSSRoutingGraph.cpp \
    utils/routing/GSSDijkstraRoutingGraph.cpp \
    utils/routing/DynamicArcMap.cpp \
    utils/routing/TSPRoutingGraph.cpp \
            ## FUZZY LOGIC
    utils/fuzzy_logic/FuzzyLogic.cpp \
            ## SVM
    include/svm/SVMClassifier.cpp \
            ## NEURAL NETWORK
    utils/neural_network/NeuralNetwork.cpp \
            ## ALGORITHMS
    utils/linear_algebra/LeastSquaresSolving.cpp \
            ## STORAGE
    utils/storage/SharedObjectStorage.cpp \
    utils/storage/ObjectStorage.cpp \
    utils/storage/AgentStorage.cpp \
            ## RANDOM GENERATORS
    utils/random_generators/NormalDistribution.cpp \
    utils/random_generators/UniformDistribution.cpp \
    utils/random_generators/RouletteWheel.cpp \
            ## PERFORMANCE
    utils/performance/PerformanceMonitor.cpp \
        # ENVIRONMENT
    environment/Environment.cpp \
    environment/physical_environment/PhysicalEnvironment.cpp \
    environment/social_environment/SocialEnvironment.cpp \
    environment/social_environment/Message.cpp \
    environment/time_environment/TimeEnvironment.cpp \
    environment/climate_environment/ClimateEnvironment.cpp \
    environment/network_environment/NetworkEnvironment.cpp \
    environment/execution_environment/ExecutionEnvironment.cpp \
        # SKILLS
    skills/Skill.cpp \
            ## MOVE
    skills/move/MoveSkill.cpp \
    skills/move/drive/DriveSkill.cpp \
            ## CONSUME
            ## PRODUCE
    skills/produce/ProduceSkill.cpp \
    skills/produce/power/PowerProduceSkill.cpp \
    skills/produce/power/SolarPowerProduceSkill.cpp \
    skills/produce/light/LightProduceSkill.cpp \
    skills/produce/image/ImageProduceSkill.cpp \
    skills/produce/sound/SoundProduceSkill.cpp \
    skills/produce/pollution/PollutionProduceSkill.cpp \
    skills/produce/pollution/vehicle/linear/VehicleLinearPollutionProduceSkill.cpp \
    skills/produce/pollution/vehicle/exp/VehicleExpPollutionProduceSkill.cpp \
    skills/produce/pollution/vehicle/sumo/VehicleSumoPollutionProduceSkill.cpp \
    skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.cpp \
    skills/produce/pollution/vehicle/svm/VehicleSVMPollutionProduceSkill.cpp \
            ## TRANSPORT
    skills/route/RoutingSkill.cpp \
    skills/route/PublicTransportUseRoutingSkill.cpp \
    skills/route/WalkPathRoutingSkill.cpp \
    skills/route/TransportLineRoutingSkill.cpp \
            ## VIEW
    skills/view/ViewRouteAheadSkill.cpp \
            ## STORE
    skills/store/AgentStoreSkill.cpp \
    skills/store/power/PowerStoreSkill.cpp \
            ## MEMORIZE
    skills/memorize/MemorizeSkill.cpp \
    skills/memorize/MemorizeAgentsSkill.cpp \
        # USE CASE - ANDER
    #usecases/gtplat/test_fuzzy_logic.cpp \
    usecases/gtplat/GTPlatScenarioCreator.cpp \
    usecases/gtplat/GTPlatAgent.cpp \
    usecases/gtplat/GTPlatItinerary.cpp \
    usecases/gtplat/GTPlatIncentive.cpp \
    usecases/gtplat/GTPlatJourney.cpp \
    usecases/gtplat/GTPlatProfile.cpp 


HEADERS += \
        # AGENTS
    agents/Agent.h \
            ## PEOPLE
    agents/people/Person.h \
    agents/people/PersonConstants.h \
                ### VEHICLE DRIVERS
    agents/people/vehicle_drivers/VehicleDriver.h \
    agents/people/vehicle_drivers/ItineraryVehicleDriver.h \
    agents/people/vehicle_drivers/PublicTransportVehicleDriver.h \
    agents/people/public_transport_users/PublicTransportUserPerson.h \
                ### PUBLIC TRANSPORT USERS
            ## VEHICLES
    agents/vehicles/Vehicle.h \
    agents/vehicles/Car.h \
    agents/vehicles/Motorcycle.h \
    agents/vehicles/Bicycle.h \
    agents/vehicles/ECar.h \
    agents/vehicles/Truck.h \
    agents/vehicles/public_transport/PublicTransportVehicle.h \
    agents/vehicles/public_transport/Bus.h \
    agents/vehicles/public_transport/Train.h \
    agents/vehicles/public_transport/Metro.h \
    agents/vehicles/public_transport/Tram.h \
    agents/vehicles/public_transport/Ferry.h \
    agents/vehicles/public_transport/Funicular.h \
            ## PLACES
    agents/places/Place.h \
    agents/places/administrative_areas/AdministrativeArea.h \
                ### ACCOMMODATIONS
    agents/places/constructions/Construction.h \
    agents/places/constructions/accommodations/Accommodation.h \
                ### CIVIL STRUCTURES
    agents/places/constructions/civil_structures/CivilStructure.h \
    agents/places/constructions/civil_structures/public_transport_stops/PublicTransportStop.h \
    agents/places/constructions/civil_structures/public_transport_stops/BusStop.h \
    agents/places/constructions/civil_structures/public_transport_stops/RailwayStop.h \
    agents/places/constructions/civil_structures/vehicle_rentals/BicycleRental.h \
    agents/places/constructions/civil_structures/parkings/Parking.h \
    agents/places/constructions/civil_structures/vehicle_rentals/VehicleRental.h \
    agents/places/constructions/civil_structures/charging_stations/ChargingStation.h \
            ## TRANSPORT NETWORKS
    agents/transport_networks/TransportLine.h \
    agents/transport_networks/TransportNode.h \
    agents/transport_networks/elements/TrafficLight.h \
                ### ROADS
    agents/transport_networks/roads/Road.h \
    agents/transport_networks/roads/RoadConstants.h \
                ### RAILWAYS
    agents/transport_networks/railways/RailwayConstants.h \
    agents/transport_networks/railways/Railway.h \
                ### WALK PATHS
    agents/transport_networks/walk_paths/WalkPathConstants.h \
    agents/transport_networks/walk_paths/WalkPath.h \
                ### PUBLIC TRANSPORT
    agents/transport_networks/public_transport/PublicTransportStretch.h \
        # UTILS
            ## PARALLELISM CONTROLLER
    utils/paralelism_controller/ParallelismController.h \
            ## GEOMETRY
    utils/geometry/GSSGeometryController.h \
    utils/geometry/GSSGeometry.h \
    utils/geometry/GSSEnvelope.h \
    utils/geometry/GSSCoordinate.h \
    utils/geometry/GSSPoint.h \
    utils/geometry/GSSLineString.h \
    utils/geometry/GSSPolygon.h \
    utils/geometry/GSSGeometryUtils.h \
    utils/geometry/GSSQuadtree.h \
            ## GRAPHS
    utils/graph/GSSGraph.h \
    utils/graph/GSSGraphUtils.h \
    utils/graph/GSSGraphEdge.h \
    utils/graph/GSSGraphNode.h \
            ## HEATMAP
    utils/heatmap/GSSHeatMap.h \
    utils/heatmap/GSSHeatMapPalette.h \
            ## GRID
    utils/grid/GSSGrid.h \
    utils/grid/InterpolationGrid.h \
            ## VORONOI
    utils/voronoi/Voronoi.h \
            ## UNITS
    utils/units/UnitConstants.h \
            ## HTTP SERVER
    utils/http_server/HttpServer.h \
    utils/http_server/HttpRequestHandler.h \
            ## WEBSOCKET SERVER
    utils/websocket_server/WebSocketServer.h \
            ## TILE SERVER
    utils/tile_server/TileServer.h \
    utils/tile_server/TileRequestHandler.h \
            ## CONVERSORS
    utils/conversors/projection_conversor/ProjectionConversor.h \
    utils/conversors/encoded_polyline_conversor/EncodedPolylineConversor.h \
    utils/conversors/tile_coordinates_conversor/TileCoordinatesConversor.h \
    utils/conversors/grid_coordinates_conversor/GridCoordinatesConversor.h \
    utils/conversors/image_coordinates_conversor/ImageCoordinatesConversor.h \
            ## EXPORTERS
    utils/exporters/custom_json_exporter/CustomJsonExporter.h \
#    utils/exporters/agent_to_shp/AgentToShp.h \
    utils/exporters/logger/Logger.h \
    utils/exporters/debugger/Debugger.h \
            ## IMPORTERS
    utils/importers/Importer.h \
    utils/importers/json_importer/JsonImporter.h \
    utils/importers/geojson_to_geometry_importer/GeoJsonToGeometry.h \
    utils/importers/gtfs_importer/GtfsImporter.h \
    utils/importers/csv_importer/CsvImporter.h \
    utils/importers/gpx_importer/GpxImporter.h \
    utils/importers/ine_importer/IneMicroDataImporter.h \
    utils/importers/osm_importer/OsmImporter.h \
    utils/importers/tiff_importer/TiffImporter.h \
            ## SORTERS
    utils/sorters/NumberSorter.h \
            ## SERVICE AREA
    utils/service_area/ServiceAreaCalculator.h \
    utils/service_area/graph/GraphServiceAreaCalculator.h \
            ## UI STYLE
    utils/uistyle/UiStyle.h \
            ## DB
    utils/db/PostgresDriver.h \
            ## API
    utils/api/APIDriver.h \
    utils/api/SyncAPIDriver.h \
    utils/api/ASyncAPIDriver.h \
    utils/api/NoReplyAPIDriver.h \
    utils/api/openstreetmap/OsmAPIDriver.h \
    utils/api/nominatim/NominatimAPIDriver.h \
            ## IMAGE DOWNLOADER
    utils/image_downloader/ImageDownloader.h \
            ## ROUTING
    utils/routing/GSSRoutingGraph.h \
    utils/routing/GSSDijkstraRoutingGraph.h \
    utils/routing/DynamicArcMap.h \
    utils/routing/TSPRoutingGraph.h \
            ## FUZZY LOGIC
    utils/fuzzy_logic/FuzzyLogic.h \
            ## SVM
    include/svm/SVMClassifier.h \
            ## NEURAL NETWORK
    utils/neural_network/NeuralNetwork.h \
            ## ALGORITHMS
    utils/linear_algebra/LeastSquaresSolving.h \
            ## STORAGE
    utils/storage/SharedObjectStorage.h \
    utils/storage/ObjectStorage.h \
    utils/storage/AgentStorage.h \
            ## RANDOM GENERATORS
    utils/random_generators/NormalDistribution.h \
    utils/random_generators/UniformDistribution.h \
    utils/random_generators/RouletteWheel.h \
            ## PERFORMANCE
    utils/performance/PerformanceMonitor.h \
        # ENVIRONMENT
    environment/Environment.h \
    environment/physical_environment/PhysicalEnvironment.h \
    environment/social_environment/SocialEnvironment.h \
    environment/social_environment/Message.h \
    environment/time_environment/TimeEnvironment.h \
    environment/climate_environment/ClimateEnvironment.h \
    environment/network_environment/NetworkEnvironment.h \
    environment/execution_environment/ExecutionEnvironment.h \
        # SKILLS
    skills/Skill.h \
            ## MOVE
    skills/move/MoveSkill.h \
    skills/move/drive/DriveSkill.h \
            ## CONSUME
            ## PRODUCE
    skills/produce/ProduceSkill.h \
    skills/produce/power/PowerProduceSkill.h \
    skills/produce/power/SolarPowerProduceSkill.h \
    skills/produce/light/LightProduceSkill.h \
    skills/produce/image/ImageProduceSkill.h \
    skills/produce/sound/SoundProduceSkill.h \
    skills/produce/pollution/PollutionConstants.h \
    skills/produce/pollution/PollutionProduceSkill.h \
    skills/produce/pollution/vehicle/linear/VehicleLinearPollutionProduceSkill.h \
    skills/produce/pollution/vehicle/exp/VehicleExpPollutionProduceSkill.h \
    skills/produce/pollution/vehicle/sumo/VehicleSumoPollutionProduceSkill.h \
    skills/produce/pollution/vehicle/VehiclePollutionProduceSkill.h \
    skills/produce/pollution/vehicle/svm/VehicleSVMPollutionProduceSkill.h \
            ## TRANSPORT
    skills/route/RoutingSkill.h \
    skills/route/PublicTransportUseRoutingSkill.h \
    skills/route/WalkPathRoutingSkill.h \
    skills/route/TransportLineRoutingSkill.h \
            ## VIEW
    skills/view/ViewRouteAheadSkill.h \
            ## STORE
    skills/store/AgentStoreSkill.h \
    skills/store/power/PowerStoreSkill.h \
            ## MEMORIZE
    skills/memorize/MemorizeSkill.h \
    skills/memorize/MemorizeAgentsSkill.h \
        # USE CASE - ANDER
    usecases/gtplat/GTPlatScenarioCreator.h \
    usecases/gtplat/GTPlatAgent.h \
    usecases/gtplat/GTPlatItinerary.h \
    usecases/gtplat/GTPlatIncentive.h \
    usecases/gtplat/GTPlatJourney.h \
    usecases/gtplat/GTPlatProfile.h 


#INCLUDE QHTTP COMPILED LIBRARY
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/qhttp-master/xbin/release/ -lqhttp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/qhttp-master/xbin/debug/ -lqhttp
else:unix: LIBS += -L$$PWD/libs/qhttp-master/xbin/ -lqhttp

INCLUDEPATH += $$PWD/libs/qhttp-master/xbin
DEPENDPATH += $$PWD/libs/qhttp-master/xbin

#INCLUDE LIBPQ SYSTEM LIBRARY (sudo apt-get install libpq-dev)
unix|win32: LIBS += -lpq

#INCLUDE LIBGEOS SYSTEM LIBRARY (sudo apt-get install libgeos-dev libgeos++-dev)
unix|win32: LIBS += -lgeos

#INCLUDE LIBPROJ SYSTEM LIBRARY (sudo apt-get install libproj-dev)
unix|win32: LIBS += -lproj

#INCLUDE LIBPROJ SYSTEM LIBRARY (sudo apt-get install libgdal-dev)
unix|win32: LIBS += -lgdal

#INCLUDE LISVM SYSTEM LIBRARY (sudo apt-get install libsvm-dev)
unix|win32: LIBS += -lsvm

#INCLUDE GRASS SYSTEM LIBRARY (sudo apt-get install grass-dev)

#INCLUDE LIBFUZZYLITE SYSTEM LIBRARY (sudo apt-get install libfuzzylite-dev) USE DFL_CPP11 FLAG
unix|win32: LIBS += -lfuzzylite
QMAKE_CXXFLAGS += -DFL_CPP11=ON

#INCLUDE EIGEN3 SYSTEM LIBRARY (sudo apt-get install libeigen3-dev)

#INCLUDE SHAPELIB SYSTEM LIBRARY (sudo apt-get install libshp-dev)
unix|win32: LIBS += -lshp

#INCLUDE LEMON SYSTEM LIBRARY
unix|win32: LIBS += -lemon

#INCLUDE FANN SYSTEM LIBRARY (sudo apt-get install libfann-dev)
unix|win32: LIBS += -lfann

#INCLUDE OPENBEAGLE REALATED LIBRARIES
#unix|win32: LIBS += -lpacc
#unix|win32: LIBS += -lopenbeagle
#unix|win32: LIBS += -lopenbeagle-GA


