
// ENVIRONMENT

app.factory('Environment', ['$http', function($http)
{
    return {
        'get': function()
        {
            return $http({
                method: 'GET',
                url: 'environment'

            });
        },
        'update': function(data){
            return $http({
                method: 'PUT',
                url: 'environment',
                data: data
            });
        },
        'run': function()
        {
            return $http({
                method: 'POST',
                url: 'environment/run'
            });
        },
    }
}]);

// SCENARIO

app.factory('Scenario', ['$http', function($http)
{
    return {
        'run': function(data){
            return $http({
                method: 'POST',
                url: 'scenario',
                data: data
            });
        }
    }
}]);


// AGENTS

app.factory('Agent', ['$http', function($http)
{
    return {
        'create': function(class_name, data)
        {
            return $http({
                method: 'POST',
                url: 'agent/' + class_name,
                data: data
            });
        },
        'update': function(class_name, id, data)
        {
            return $http({
                method: 'PUT',
                url: 'agent/' + class_name + '/' + id,
                data : data
            });
        },
        'deleteAll': function(class_name, id)
        {
            return $http({
                method: 'DELETE',
                url: 'agents/' + class_name
            });
        },
        'deleteOne': function(class_name, id)
        {
            return $http({
                method: 'DELETE',
                url: 'agent/' + class_name + '/' + id
            });
        },
        'getAll': function(class_name , from , limit)
        {
            return $http({
                method: 'GET',
                url: 'agents/' + class_name + '/from/' + from + '/limit/' + limit
            });
        },
        'getOne': function(class_name, id)
        {
            return $http({
                method: 'GET',
                url: 'agent/' + class_name + '/' + id
            });
        },
        'runAll': function(class_name)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/run'
            });
        },
        'runOne': function(class_name, id)
        {
            return $http({
                method: 'POST',
                url: 'agent/' + class_name + '/' + id + '/run'
            });
        },
        'stopAll': function(class_name)
        {
            return $http({
                method: 'POST',
                url: 'agents/' + class_name + '/stop'
            });
        },
        'stopOne': function(class_name, id)
        {
            return $http({
                method: 'POST',
                url: 'agent/' + class_name + '/' + id + '/stop'
            });
        }
    }
}]);

// RESULTS

app.factory('Result', ['$http', function($http)
{
    return {
        'get': function(data){
            return $http({
                method: 'GET',
                url: 'result'
            });
        }
    }
}]);