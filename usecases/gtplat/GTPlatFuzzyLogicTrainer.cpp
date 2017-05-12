#include "GTPlatFuzzyLogicTrainer.h"

#include "utils/statistics/ConfusionMatrix.h"

GTPlatFuzzyLogicTrainer::GTPlatFuzzyLogicTrainer( QList<GTPlatAgent*> agents , QMap<QString , QStringList> input_values , QStringList outputs , QObject *parent) : FuzzyLogicTrainer( input_values , outputs , parent ){

    if( agents.isEmpty() ){
        Debugger::critical( this , Q_FUNC_INFO , "EMPTY AGENTS" );
    }

    // Store agents and their census mode
    this->agents = agents;
    foreach( GTPlatAgent* agent , this->agents){

        // Store options
        this->agent_options.insert( agent->profile->options.join("") , this->agent_options.value( agent->profile->options.join("") , 0 ) +1 );

        // Store max values
        foreach( GTPlatItinerary* it , agent->itineraries ){
            if( !it->route_found ){ continue; }
            max_duration = qMax( max_duration , it->getExpectedDuration().number() );
            max_price = qMax( max_price , it->getExpectedPrice().number() );
            max_pollution = qMax( max_pollution , it->getExpectedPollutionCC() );
            max_length = qMax( max_length , it->getExpectedLength().number() );
        }
    }

    // Add extra GRZEGORZ RULES
    QList<FuzzyLogic::Rule> grzegorz_rules;

    // CAR
    { // if CAR_PRICE is VERY_LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is VERY_LOW and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is VERY_LOW and TRANSIT_TIME is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is LOW and (TRANSIT_TIME is VERY_HIGH ) then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is MEDIUM and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is HIGH and TRANSIT_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_TIME is LOW and MOTORCYCLE_TIME is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is VERY_LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_LOW and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is LOW then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is LOW and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is MEDIUM then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is MEDIUM and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is HIGH and TRANSIT_PRICE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is HIGH and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_PRICE is VERY_HIGH and TRANSIT_PRICE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_DISTANCE is HIGH and BICYCLE_DISTANCE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_DISTANCE is MEDIUM and BICYCLE_DISTANCE is HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if CAR_DISTANCE is MEDIUM and BICYCLE_DISTANCE is VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_DISTANCE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_DISTANCE is not VERY_LOW and TRANSIT_DISTANCE is not LOW and TRANSIT_DISTANCE is not MEDIUM and TRANSIT_DISTANCE is not HIGH and TRANSIT_DISTANCE is not VERY_HIGH then TRANSPORT is CAR
        FuzzyLogic::Rule rule;
        rule.output_value = "CAR";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_DISTANCE" , "not VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }


    // TRANSIT
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is VERY_LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is MEDIUM and CAR_PRICE is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is MEDIUM and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_PRICE is HIGH and CAR_PRICE is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is VERY_LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is VERY_LOW and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is LOW then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is LOW and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is MEDIUM then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "MEDIUM" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is MEDIUM and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is HIGH and CAR_TIME is HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if TRANSIT_TIME is HIGH and CAR_TIME is VERY_HIGH then TRANSPORT is TRANSIT
        FuzzyLogic::Rule rule;
        rule.output_value = "TRANSIT";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "TRANSIT_TIME" , "HIGH" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }

    // MOTORCYCLE
    { // if MOTORCYCLE_DISTANCE is VERY_LOW and CAR_PRICE is HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_DISTANCE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if MOTORCYCLE_DISTANCE is VERY_LOW and CAR_PRICE is VERY_HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_DISTANCE" , "VERY_LOW" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if MOTORCYCLE_PRICE is MEDIUM and CAR_PRICE is HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_PRICE" , "HIGH" } );
        grzegorz_rules.append( rule );
    }
    { // if MOTORCYCLE_PRICE is MEDIUM and CAR_PRICE is VERY_HIGH then TRANSPORT is MOTORCYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "MOTORCYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "MOTORCYCLE_PRICE" , "MEDIUM" } );
        rule.terms.append( FuzzyLogic::RuleTerm() = { "CAR_TIME" , "VERY_HIGH" } );
        grzegorz_rules.append( rule );
    }

    // WALK
    { // if WALK_DISTANCE is VERY_LOW then TRANSPORT is WALK
        FuzzyLogic::Rule rule;
        rule.output_value = "WALK";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "WALK_DISTANCE" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if WALK_DISTANCE is LOW then TRANSPORT is WALK
        FuzzyLogic::Rule rule;
        rule.output_value = "WALK";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "WALK_DISTANCE" , "LOW" } );
        grzegorz_rules.append( rule );
    }

    // BICYCLE
    { // if BICYCLE_DISTANCE is VERY_LOW then TRANSPORT is BICYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "BICYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_DISTANCE" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }
    { // if BICYCLE_TIME is VERY_LOW then TRANSPORT is BICYCLE
        FuzzyLogic::Rule rule;
        rule.output_value = "BICYCLE";
        rule.terms.append( FuzzyLogic::RuleTerm() = { "BICYCLE_TIME" , "VERY_LOW" } );
        grzegorz_rules.append( rule );
    }

    this->rule_population.append( grzegorz_rules );

}

double GTPlatFuzzyLogicTrainer::calculateFitness( QList< FuzzyLogic::InputMemberShipFunction > input , QList< FuzzyLogic::Rule > ruleset , FuzzyLogic::OutputMembershipFunction output , QString accumulator , QString conjunction , QString disjunction , QString activator ){

    // Create success matrix
    ConfusionMatrix travel_matrix;

    // Create fuzzy logic
    FuzzyLogic* fuzzy_logic = new FuzzyLogic();
    fuzzy_logic->setParameters( input , ruleset , output );
    fuzzy_logic->configure( conjunction , disjunction , activator , accumulator , "LargestOfMaximum");

    // Iterate agents
    foreach( GTPlatAgent* agent , this->agents ){

        foreach( GTPlatItinerary* it , agent->itineraries ){
            QString option = it->requested_mode;

            // Remove ;WALK from TRANSIT;WALK
            option = option.replace( ",WALK" , "" );

            QString price_name = option + "_PRICE";
            if( it->route_found ){
                fuzzy_logic->setInputValue( price_name , max_price == 0 ? 0 : it->getExpectedPrice().number() / max_price );
            } else { fuzzy_logic->setInputValue( price_name , fl::nan ); }

            QString time_name = option + "_TIME";
            if( it->route_found ){
                fuzzy_logic->setInputValue( time_name , max_duration == 0 ? 0 : it->getExpectedDuration().number() / max_duration );
            } else { fuzzy_logic->setInputValue( time_name , fl::nan ); }

            QString length_name = option + "_DISTANCE";
            if( it->route_found ){
                fuzzy_logic->setInputValue( length_name , max_length == 0 ? 0 : it->getExpectedLength().number() / max_length );
            } else { fuzzy_logic->setInputValue( length_name , fl::nan ); }

            QString green_name = option + "_EMISSIONS";
            if( it->route_found ){
                fuzzy_logic->setInputValue( green_name , max_pollution == 0 ? 0 : it->getExpectedPollutionCC() / max_pollution );
            } else { fuzzy_logic->setInputValue( green_name , fl::nan ); }
        }

        fuzzy_logic->process();

        QString r = fuzzy_logic->getOutputValueAsName( "TRANSPORT" );
        travel_matrix.sumValue( agent->profile->options.join("") , r , 1 );
    }

    // Get matrix diagonal
    QMap<QString , double> diagonal = travel_matrix.getDiagonal();
    double error_percentage = 0;
    foreach( QString key , diagonal.keys() ){
        double v = diagonal.value( key );
        error_percentage += 1 - ( v / this->agent_options.value( key ) );
    }

    delete fuzzy_logic;

    qDebug() << error_percentage << diagonal;

    return error_percentage;
}

