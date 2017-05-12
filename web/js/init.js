var app = angular.module('geosmartsim', ['ngRoute' , 'ui.bootstrap' , 'nvd3']);

app.config(['$routeProvider', function($routeProvider) {

    $routeProvider

        /* GLOBAL */
        .when('/', {
            templateUrl: 'static/views/intro.html',
            controller: 'IntroController'
        })

        /* MAP */
        .when('/map', {
        })

        /* ENVIRONMENT */
        .when('/environment', {
            templateUrl: 'static/views/environment.html',
            controller: 'EnvironmentController'
        })

        /* AGENTS */
        .when('/agents', {
            templateUrl: 'static/views/agentclasses.html',
            controller: 'AgentClassesController'
        })
        .when('/agents/:type', {
            templateUrl: 'static/views/agents.html',
            controller: 'AgentsController'
        })
        .when('/agents/:type/create', {
            templateUrl: 'static/views/agentscreate.html',
            controller: 'AgentsCreateController'
        })
        .when('/agent/:type/:id', {
            templateUrl: 'static/views/agent.html',
            controller: 'AgentController'
        })


        /* RESULTS */
        .when('/results', {
            templateUrl: 'static/views/results.html',
            controller: 'ResultsController'
        })
        
        /* ELSE */
        .otherwise({
            redirectTo: '/'
        });

}]);