// GLOBAL CONTROLLER

app.controller('GeoSmartSimController', ['$scope', '$rootScope', '$compile', 'Agent', function($scope, $rootScope, $compile, Agent){

    // Variables
    $rootScope.selected_agent_id = false;
    $rootScope.socket_port = 3009;
    $rootScope.tiles_port = 3010;
    $rootScope.hostname = window.location.hostname;
    
    // Map elements
    $rootScope.map = null;
    $rootScope.layerSwitcher = null;

    // Storage
    $rootScope.environment = {};
    $rootScope.agents = {}; /* {CLASS : { ID : Agent JSON }} */
    $rootScope.layers = {}; /* {CLASS : { ID : GeoJSONLayer }} */

    // Overlay Layers
    $rootScope.overlays = {}; /* {CLASSNAME : FeatureLayerGroup} */
    
    // Download all existing agents
    $rootScope.recursiveDownload = function( class_name , from , limit ){
        
        setTimeout(function(){
            
            var f = from || 0 ;
            var l = limit || 100;
            Agent.getAll( class_name , f , l ).success(function(res){
            $rootScope.parseResponse( res );
            if( res.count > f+l ){
                // Download next page
                setTimeout( function(){ $rootScope.recursiveDownload( class_name , f+l , 100 ) } , 5000 );
            }
            else if( res.count > 0 ){
                // Start again downloading
                setTimeout( function(){ $rootScope.recursiveDownload( class_name , 0 , 100 ) } , 5000 );
            }
            })
            .error(function(res){
                setTimeout( function(){ $rootScope.recursiveDownload( class_name , 0 , limit ); } , 10000 );
            });
            
        } , 1000 );
    };
    
    // Download all existing agents
    $rootScope.autoDownloadLayer = function( class_name , layer_ref ){
        
        setTimeout(function(){

            var l  = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/'+ class_name +'/timestamp/'+ new Date().getTime() +'/tile/{z}/{x}/{y}.png', { maxZoom: 25, maxNativeZoom: 25 });
            l.setZIndex( 10 );
            $rootScope.map.addLayer( l , true );
            l.on('load', function () {
                if( layer_ref ){
                    $rootScope.map.removeLayer( layer_ref );
                    delete layer_ref;
                }
                $rootScope.autoDownloadLayer( class_name , this );
            });

        } , 5000);
    };
    
    
    // Parse each agent
    $rootScope.parseAgent = function(agent , operation){

        if( !agent ){
            return;
        }                        
                
        var id = agent.id;
        var type = agent.type;
        
        if(agent.properties){
            // Retrieve info about current activity to show in bottomright box
            if( agent.properties.current_activity ){
                var date = new Date(agent.properties.current_time.value);
                if( agent.properties.current_action ){
                    $rootScope.map_info.update( '<div class="panel-body"><h3>' + date.getHours() +':'+ date.getMinutes() +':'+ date.getSeconds() + ' - ' + agent.properties.current_activity.value + ' - ' + agent.properties.current_action.value  + '</h3></div>' );
                } else {
                    $rootScope.map_info.update( '<div class="panel-body"><h3>' + date.getHours() +':'+ date.getMinutes() +':'+ date.getSeconds() + ' - ' + agent.properties.current_activity.value + '</h3></div>' );
                }
            }            
        }

        if( operation == 'CREATE' || operation == 'UPDATE' ){
            /* If new agent class */
            if( !$rootScope.agents[type] ){
                $rootScope.createLayerGroup(type);
            }
            // Delete if existing layer
            if( $rootScope.agents[type][id] ){
                $rootScope.deleteLayer(type , id);
            }
            
            // Create geojson layer and add it to overlays 
            $rootScope.createLayer(type, id , agent);
        }
        
        if( operation == 'DELETE' ){
            /* Delete from agentsLayer and memory */
            if ( $rootScope.agents[ type ] && $rootScope.agents[ type ][ id ] ){
                $rootScope.deleteLayer(type , id);
            }
                
        }
    };

    // Create new layergroup
    $rootScope.createLayerGroup = function(type){
        $rootScope.agents[type] = {};
        $rootScope.layers[ type ] = {};
        $rootScope.overlays[ type ] = L.featureGroup();
        $rootScope.overlays[ type ].addTo( $rootScope.map );
        $rootScope.layerSwitcher.addOverlay( $rootScope.overlays[ type ] , type );
    };

    // Create new layer
    $rootScope.createLayer = function(type, id , agent){
        $rootScope.agents[ type ][ id ] = agent;
        if( $rootScope.selected_agent_id == id ){

            // Apply changes
            //$rootScope.$apply();

            // Fit map to agent
            //$rootScope.map.fitBounds(L.geoJson( agent ).getBounds(), {minZoom : 15 , maxZoom : 20});
        }
        if( agent.geometry ){
            
            var geojson = { "type": "Feature" , "properties": {} , "geometry": agent.geometry.value };
            for(var p in agent){ // Set properties
                if( p != 'geometry' ){
                    geojson.properties[p] = agent[p];
                }
            }
            
            var layer = L.geoJson( geojson, {style: $rootScope.getStyle , pointToLayer: $rootScope.pointToLayer, onEachFeature: $rootScope.onEachHighlightFeature} );
            $rootScope.layers[ type ][ id ] = layer;
            $rootScope.overlays[ type ].addLayer( layer );
            $rootScope.map.indexLayer( layer );
        }
    };

    // Delete layer
    $rootScope.deleteLayer = function(type, id){
            $rootScope.overlays[ type ].removeLayer( $rootScope.layers[ type ][ id ] );
            delete $rootScope.agents[ type ][ id ];
            delete $rootScope.layers[ type ][ id ];
    };

    // Parse response
    $rootScope.parseResponse = function(res){
        var json_data = res.data,
            operation = res.operation || 0;

        if( !$rootScope.map ){ return; }
            
        // If operation >= 0 -> Agents notification
        if( operation == 'CREATE' || operation == 'UPDATE' || operation == 'DELETE' ){
            // data is an array, execute the same operation for all the elements inside it
            if( Object.prototype.toString.call( json_data ) === '[object Array]' ) {
                for(var i in json_data){
                    $rootScope.parseAgent(json_data[i], operation );
                }
                // Else, data will only contain one element
            } else {
                $rootScope.parseAgent(json_data, operation );
            }
        }
        
        // If operation < 0 -> Results notification
        if( operation == 'RESULTS' ){
            
            console.log( json_data );
            
            $scope.results = {};
            
            var linechart = { type: 'lineChart',
                          height: 150,
                          x: function(d){ return d[0]; },
                          y: function(d){ return d[1]; },       
			  xScale: d3.time.scale(), // <-- explicitly set time scale
			  xAxis: {
			      axisLabel: "Timestamp",
			      //rotateLabels: 90,
			      ticks: 15 , // <-- add formatter for the ticks
			      tickFormat: function(d) {
				  return d3.time.format('%H:%M')(new Date(d))				
			      }
			  },
                          yAxis: {
			      tickFormat: function(d) {
				  return d3.format('.4s')(d)				
			      }
			  },
                          color: [
                            "#1f77b4",
                            "#ff7f0e",
                            "#2ca02c",
                            "#d62728",
                            "#9467bd",
                            "#8c564b",
                            "#e377c2",
                            "#7f7f7f",
                            "#bcbd22",
                            "#17becf"
                            ],
			  margin: {
			    "top": 16,
			    "right": 16,
			    "bottom": 16,
			    "left": 64
			  },	       
                          showValues: true,
                          useInteractiveGuideline: true,
                          valueFormat: function(d){ return d3.format('.0f')(d); },
                          duration: 0,
	    };
            
            var barchart = {
                        type : "discreteBarChart",
                        height : 150,
                        showValues : true,
                        duration : 400,
                        x: function(d){ return d.key; },
                        y: function(d){ return d.value; },
                        valueFormat: function(d){ return d3.format('d')(d); },
            };
                                
            // CURRENT_MODES
            $scope.results.current_modes = {};
            $scope.results.current_modes = {       options : { chart: barchart },
                                                data : json_data.current_modes.value.data
                                        }
                                        
            // CHOSEN_MODES
            $scope.results.chosen_modes = {};
            $scope.results.chosen_modes = {       options : { chart: barchart },
                                                data : json_data.chosen_modes.value.data
                                        }
                                                       
            $scope.$apply();
        }
    };


    // Locate agent
    $rootScope.locateAgent = function(type , id, text){
        $rootScope.map.fitBounds( $rootScope.layers[type][id].getBounds(), {minZoom : 15 , maxZoom : 20});
        $rootScope.layers[type][id].bindPopup('<h3>' + text + '</h3><a class="btn btn-primary" href="#/agent/' + type + '/' + id + '">View more</a>');
        $rootScope.layers[type][id].openPopup();
    };
    
    // Run agent
    $rootScope.runAgent = function(type , id){
        Agent.runOne( type , id );
    }
    
    // Stop agent
    $rootScope.stopAgent = function(type , id){
        Agent.stopOne( type , id );
    }
    
    $rootScope.deleteAgent = function( type , id ){
        Agent.deleteOne( type , id );
    }

    // Get agent style
    $rootScope.getStyle = function(geojson) {
        var style = geojson.properties || {};
        return {
            fillColor: style.color ? style.color.value : 'gray',
            weight: style.weight ? style.weight.value : 6,
            opacity: style.border_opacity ? style.border_opacity.value : 0.8,
            color: style.border_color ? style.border_color.value : 'black',
            fillOpacity: style.fill_opacity ? style.fill_opacity.value : 0.6,
            dashArray: style.dash_array ? style.dash_array.value : 1
        };
    };


    // Highlight
    $rootScope.onEachHighlightFeature = function(geojson, layer) {
        if( geojson.properties ){
            var html = '<div><h3>' + geojson.properties.type + ' ' + geojson.properties.id + '</h3>\
                                <button class="btn btn-primary btn-block" ng-click="$root.runAgent(\'' + geojson.properties.type + '\',' + geojson.properties.id + ')">Run agent</button>\
                                <button class="btn btn-primary btn-block" ng-click="$root.stopAgent(\'' + geojson.properties.type + '\',' + geojson.properties.id + ')">Stop agent</button>\
                                <a class="btn btn-secondary btn-block" href="#/agent/' + geojson.properties.type + '/' + geojson.properties.id + '">View agent</a></div>';
            var linkFunction = $compile(angular.element(html));
            layer.bindPopup( linkFunction($scope)[0] );
        }
    };


    // Point geojson, set marker icon and if draggable
    $rootScope.pointToLayer = function(geojson, latlng){
        var style = geojson.properties || {};
        var icon = L.icon({
            iconUrl: style.icon_url ? style.icon_url.value : 'http://cdn.flaticon.com/png/128/33622.png',
            iconSize: style.icon_size ? style.icon_size : [32, 32],
            iconAnchor: style.icon_anchor ? style.icon_anchor.value : [16, 32],
            popupAnchor: style.popup_anchor ? style.popup_anchor.value : [0, -24]
        });
        return L.marker(latlng, {icon: icon});
    };


    // Create socket and bind events
    $scope.socket = new WebSocket("ws://" + $rootScope.hostname + ":" + $rootScope.socket_port);
    $scope.socket.onopen = function (event) {};
    $scope.socket.onmessage = function (event) {
        $rootScope.parseResponse( JSON.parse(event.data) );
    }
    
}]);

 /* MAP CONTROLLER */

app.controller('MapController', ['$scope', '$rootScope', '$http', 'Environment', 'Agent', function($scope, $rootScope,  $http, Environment, Agent){

    // Reset flags
    $rootScope.selected_agent_id = false;

    var locate_class = false;
    var locate_id = false;

    if( !$rootScope.map ){

        // Get environment info
        Environment.get().success(function(res){

            $rootScope.environment = res.data;

            // Tell map to have spatial index
            L.Map.include(L.LayerIndexMixin);
            
            // Create map
            $rootScope.map = L.map('map-canvas' , {fullscreenControl: true}).setView([43.27166, -2.93959], 20);

            // Create base layers
            var baselayers = {
                Wikimedia: L.tileLayer('https://maps.wikimedia.org/osm-intl/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }).addTo($rootScope.map),
                OpenStreetMap: L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }),
                Toner: L.tileLayer('http://{s}.tile.stamen.com/toner-background/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }),
                Hyda: L.tileLayer('http://{s}.tile.openstreetmap.se/hydda/base/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }),
                //WaterColor: L.tileLayer('http://{s}.tile.stamen.com/watercolor/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 18 }),
                Satelite: L.tileLayer('http://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}.jpg', { maxZoom: 22, maxNativeZoom: 18 }),
            };
            
            $rootScope.overlays.Elevation = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/grids/Elevation/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
            
            $rootScope.overlays.WalkPaths = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/graphs/WalkPath/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
            $rootScope.overlays.House = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/Place/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
            $rootScope.overlays.Roads = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/graphs/Road/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
            $rootScope.overlays.Railways = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/graphs/Railway/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });             $rootScope.overlays.PublicTransportStretchs = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/graphs/PublicTransportStretch/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
             $rootScope.overlays.Accommodations = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/Accommodation/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
             $rootScope.overlays.TrafficLights = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/TrafficLight/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
             $rootScope.overlays.BusStops = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/BusStop/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
             $rootScope.overlays.RailwayStops = L.tileLayer('http://' + $rootScope.hostname + ':' + $rootScope.tiles_port + '/agents/RailwayStop/timestamp/0/tile/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 22 });
            
            
            // Add scale
            L.control.scale().addTo( $rootScope.map );
            
            // Add info control
            $rootScope.map_info = L.control({position: 'bottomright'});
            $rootScope.map_info.onAdd = function (map) {
                this._div = L.DomUtil.create('div', 'leaflet-control-layers panel panel-default');
                this._div.innerHTML = '';
                return this._div;
            };
            $rootScope.map_info.update = function(content){ this._div.innerHTML = content; };
            $rootScope.map_info.addTo( $rootScope.map );

            // Fit bounds
            if( $rootScope.environment.geometry ){
                try {
                    var geojson = { "type": "Feature" , "properties" : $rootScope.environment , "geometry" : $rootScope.environment.geometry.value };
                    var env_geom = L.geoJson( geojson , {style: $rootScope.getStyle} );//.addTo( $rootScope.map );
                    $rootScope.map.fitBounds( env_geom.getBounds(), {minZoom : 15 , maxZoom : 20});
                } catch(e) {}
            }

            // Layer switcher
            $rootScope.layerSwitcher = L.control.layers(baselayers , $rootScope.overlays , {position : 'bottomleft'}).addTo($rootScope.map);

        }).error(function(res){ alert('Error creating map') });
    }

}]);

 /* INTRO CONTROLLER */

app.controller('IntroController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', 'Agent', function($scope, $rootScope, $routeParams, $location, Environment, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;
    
    $scope.runEnvironment = function(){ 
        Environment.run();
            
        // START AUTO DOWNLOAD
        //$rootScope.autoDownloadLayer( 'GTPlatAgent' );
        //$rootScope.autoDownloadLayer( 'Car' );
        //$rootScope.recursiveDownload( 'GTPlatAgent' , 0 , 100 );
        //$rootScope.recursiveDownload( 'Car' , 0 , 100 );
        
        /*var agent_layer = false;
        setInterval(function(){
            
            Agent.getOne( 'GTPlatAgent' , 6196)
            .success(function(res){
                
                var temp = L.heatLayer( res.data.values , { max : res.data.maximum / 10 , radius : 25 } );
                $rootScope.map.addLayer( temp , true );
                if( agent_layer ){ 
                    $rootScope.map.removeLayer( agent_layer );
                    delete agent_layer;
                }
                agent_layer = temp;
            });
        } , 3000);
        
        var heatLayer = false;
        setInterval(function(){
            
            Agent.getOne( 'PetronorHeatMap' , 6196)
            .success(function(res){
                
                var temp = L.heatLayer( res.data.values , { max : res.data.maximum / 10 , radius : 25 } );
                $rootScope.map.addLayer( temp , true );
                if( heatLayer ){ 
                    $rootScope.map.removeLayer( heatLayer );
                    delete heatLayer;
                }
                heatLayer = temp;
            });
        } , 3000);*/
    };
}]);

 /* ENVIRONMENT CONTROLLER */

app.controller('EnvironmentController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', function($scope, $rootScope, $routeParams, $location, Environment)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    /* Get environment */
    $scope.reloadEnvironment = function(){ 
        Environment.get().success(function(res){
            
            $rootScope.environment = res.data;

            // Create environment edit map
            $scope.map = L.map('agent-map-canvas' , {editable: true, fullscreenControl: true}).setView([43.27166, -2.93959], 10);
            OpenStreetMap: L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo( $scope.map );
            
            if( $rootScope.environment.geometry ){
                // Clone the geometry to the geometry edit map
                var geojson = { "type": "Feature" , "properties" : $rootScope.environment , "geometry" : $rootScope.environment.geometry.value };
                $scope.environment_geometry = L.geoJson( geojson , {style: $rootScope.getStyle , pointToLayer: $rootScope.pointToLayer} ).addTo( $scope.map );
                
                // Fit map to environment
                $scope.map.fitBounds( $scope.environment_geometry.getBounds(), {minZoom : 15 , maxZoom : 20});
            }
            
        }).error(function(res){ $location.path('/') }); };
        
    $scope.updateEnvironment = function(){ 
        Environment.update( $rootScope.environment ).success(function(res){ $rootScope.environment = res.data; }).error(function(res){ $location.path('/environment') }) 
    };
    $scope.reloadEnvironment();
}]);

/* AGENT CLASSES CONTROLLER */

app.controller('AgentClassesController', ['$scope', '$rootScope', '$routeParams', '$location', 'Environment', 'Agent', function($scope, $rootScope, $routeParams, $location, Environment, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    Environment.get().success(function(res){
        $rootScope.environment = res.data;
        $scope.agent_types = $rootScope.environment.agent_types;
    });

    $scope.runAll = function( type ){
        Agent.runAll( type );
    }
    
}]);

 /* AGENTS CONTROLLER */

app.controller('AgentsController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    $scope.type = $routeParams.type;
    $scope.paginated = 0;
    $scope.page_size = 100;
    
    $scope.getNextPaginated = function() {
    /* Get all agents for class with pagination */
        Agent.getAll( $scope.type , $scope.paginated++ * $scope.page_size , $scope.page_size ).success(function(res){
            $rootScope.parseResponse( res );
            $scope.agents = $rootScope.agents[ $scope.type ];
        }).error(function(res){ $location.path('/') });
    }
    $scope.getNextPaginated();

    $scope.runAll = function(){
        Agent.runAll( $scope.type );
    }
    
    $scope.stopAll = function(){
        Agent.stopAll( $scope.type );
    }
    
    $scope.deleteAll = function(){
        Agent.deleteAll( $scope.type );
    }
    
}]);

 /* AGENT CONTROLLER */

app.controller('AgentController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    $scope.type = $routeParams.type;
    $scope.id = $rootScope.selected_agent_id = $routeParams.id;
    $scope.map = false;
    $scope.map_geometry = false;

    /* Get agent */
    
    $scope.getAgent = function(){
        Agent.getOne( $scope.type , $scope.id ).success(function(res){
            
            $scope.agent = res.data;
           
            // Remove old geometry
            if( $scope.map_geometry ){
                $scope.map.removeLayer( $scope.map_geometry );
            }
            
            // Check if agent has geometry
            if( $scope.agent.geometry ){
                
                // Clone the geometry to the geometry edit map               
                
                // Marker
                if( $scope.agent.geometry.value.type == 'Point' ){
                    var coors = $scope.agent.geometry.value.coordinates;
                    $scope.map_geometry = L.marker( [coors[1] , coors[0]] , $rootScope.getStyle( $scope.agent ) );
                    $scope.map_geometry.enableEdit();              
                }
                
                // LineString
                if( $scope.agent.geometry.value.type == 'LineString' ){
                    var coors = [];
                    var _coord = $scope.agent.geometry.value.coordinates;
                    _coord.forEach(function(item) {
                        coors.push( [item[1], item[0]] );
                    });
                    $scope.map_geometry = L.polyline( coors , $rootScope.getStyle( $scope.agent ) );
                    $scope.map_geometry.enableEdit();
                }
                
                // Polygon
                if( $scope.agent.geometry.value.type == 'Polygon' ){
                }
                
                // GeometryCollection
                if( $scope.agent.geometry.value.type == 'GeometryCollection' ){
                    $scope.map_geometry = L.geoJson( $scope.agent.geometry.value , {style: $rootScope.getStyle} );
                    // Not editable
                }
                
                // Create agent edit map
                if( !$scope.map ){
                    $scope.map = L.map('agent-map-canvas' , {editable: true, fullscreenControl: true}).setView([43.27166, -2.93959], 15);
                    OpenStreetMap: L.tileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom: 22, maxNativeZoom: 19 }).addTo( $scope.map );
                    $scope.map.fitBounds( $scope.map_geometry.getBounds() );
                }
                $scope.map_geometry.addTo( $scope.map );
    
            }
        }).error(function(res){ $location.path('/' + $scope.type ) });
    }
    $scope.getAgent();
    
    $scope.saveAgent = function(){
        Agent.update( $scope.agent.type , $scope.agent.id , $scope.agent );
    }
    
    $scope.deleteAgent = function(){
        Agent.deleteOne( $scope.type , $scope.id );
    }

}]);

 /* NEW AGENT CONTROLLER */

app.controller('AgentsCreateController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;

    $scope.type = $routeParams.type;
    $scope.agent_amount = 1;
    $scope.agent_properties = {};
    $scope.agent_style = {};
    
    
}]);

 /* RESULTS CONTROLLER */

app.controller('ResultsController', ['$scope', '$rootScope', '$routeParams', '$location', 'Agent', function($scope, $rootScope, $routeParams, $location, Agent)
{
    // Reset flags
    $rootScope.selected_agent_id = false;
    
}]);

