//
// Created by fotop on 24/10/2023.
//


#include "VuelaFlight.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>

VuelaFlight::VuelaFlight(std::string airports_file, std::string routes_file, std::string airlines_file) {

    std::ifstream airports_stream;

    airports_stream.open(airports_file); //project folder
    if (airports_stream.good()) {
        std::string id;
        std::string iata;
        std::string type;
        std::string name;
        std::string strLatitude;
        std::string strLongitude;
        std::string continent;
        std::string country_iso;
        std::stringstream columns_airports;
        std::string line;

        clock_t t_ini = clock();

        while (getline(airports_stream, line)) {

            //¿Exists a new line on the file?
            if (!line.empty()) {

                columns_airports.str(line);

                //Line format: id;iata;type;size;fecha;country_iso

                getline(columns_airports, id, ';'); //we readAVL the line till ';' and omit the caracter
                getline(columns_airports, iata, ';');
                getline(columns_airports, type, ';');
                getline(columns_airports, name, ';');
                getline(columns_airports, strLatitude, ';');
                getline(columns_airports, strLongitude, ';');
                getline(columns_airports, continent, ';');
                getline(columns_airports, country_iso, ';');

                line = "";
                columns_airports.clear();
                UTM location = UTM(strLatitude, strLongitude);
                Airport airp(std::stoi(id), iata, type, name, location, continent, country_iso);
                airports.push(iata,airp);
            }
        }

        airports_stream.close();

        std::cout << " Hash Table with airports initialized." << std::endl << " Reading Time: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " secs." << std::endl;
    } else {
        std::cout << "Fatal error opening the file" << std::endl;
    }



    std::ifstream airlines_stream;
    airlines_stream.open(airlines_file); //project folder
    if (airlines_stream.good()) {
        std::string id;
        std::string icao;
        std::string name;
        std::string country;
        std::string active;
        bool real_active;
        std::stringstream columns_airlines;
        std::string line;

        clock_t t_ini = clock();

        while (getline(airlines_stream, line)) {

            //¿Exists a new line on the file?
            if (!line.empty()) {

                columns_airlines.str(line);


                getline(columns_airlines, id, ';'); //we readAVL the line till ';' and omit the caracter
                getline(columns_airlines, icao, ';');
                getline(columns_airlines, name, ';');
                getline(columns_airlines, country, ';');
                getline(columns_airlines, active, ';');

                if (active == "Y")
                    real_active = true;
                else
                    real_active = false;

                line = "";
                columns_airlines.clear();

                Airline line_airline(std::stoul(id),icao,name,country,real_active);
                airlines.insert(std::pair<std::string,Airline>(icao, line_airline));

            }
        }

        airports_stream.close();

        std::cout << " AVL with airlines initialized" << std::endl << "Reading Time: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " secs." << std::endl;
    } else {
        std::cout << "Fatal error opening the file" << std::endl;
    }



    std::ifstream routes_stream;
    routes_stream.open(routes_file); //project folder
    if (routes_stream.good()) {
        std::string airline;
        std::string orig_airport;
        std::string dest_airport;
        std::stringstream columns_routes;
        std::string line;


        clock_t t_ini = clock();

        while (getline(routes_stream, line)) {

            //¿Exists a new line on the file?
            if (!line.empty()) {

                columns_routes.str(line);


                getline(columns_routes, airline, ';'); //we readAVL the line till ';' and omit the caracter
                getline(columns_routes, orig_airport, ';');
                getline(columns_routes, dest_airport, ';');

                line = "";
                columns_routes.clear();

                //build the "key" objects to find them
                Airport orig = Airport(orig_airport);
                Airport dest = Airport(dest_airport);
                Airline airlineObject(airline);

                //search the airline needed to build the route
                Airline* realAirline = &airlines.find(airlineObject.getIcao())->second;

                //search the airports in the data base and build the route to push it after.
                Airport *real_orig = airports.search(orig_airport);
                Airport *real_dest = airports.search(dest_airport);
                //build the new route
                Route line_route(realAirline,real_orig,real_dest);
                //push the new route on the linkedList
                origRoutes.insert(std::pair<std::string,Route>(orig_airport,line_route));
                destRoutes.insert(std::pair<std::string,Route*>(dest_airport,&line_route));
                //link recent route to his airline.
                realAirline->linkAirRoute(&origRoutes.end()->second);
            }
        }

        airports_stream.close();

        std::cout << " Linked_list with routes initialized" << std::endl <<"Reading Time: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " secs." << std::endl;
    } else {
        std::cout << "Fatal error opening the file" << std::endl;
    }

}

Airport& VuelaFlight::searchAirport(std::string iataAirport) {
    return *airports.search(iataAirport);
}

unsigned int VuelaFlight::getNumAirports() {
    return airports.getSize();
}

const std::vector<Airport> VuelaFlight::getAirports() {
    return airports.getAirports();
}

void VuelaFlight::deleteAirport(std::string iataAirport) {
    if (airports.pop(iataAirport)){
        std::cout << "Airport " << iataAirport << " deleted" << std::endl;
    }
    else{
        std::cout << "Airport " << iataAirport << " NOT deleted, it wasn't stored in the database" << std::endl;
    }
}
/*
Route& VuelaFlight::origDestRoutesSearch(const std::string& airportIataOrig, const std::string& airportIataDest) {
    std::list<Route>::iterator iterator = routes.begin();
    unsigned int count = 0;

    while(count < routes.size() and iterator != routes.end()){
        if (iterator->getOrigin()->getIata() == airportIataOrig and iterator->getDestination()->getIata() == airportIataDest){
            return *iterator;
        }
        iterator++;
        count++;
    }
    throw std::invalid_argument("---FATAL ERROR-- Doesn't exist a route in the database specified with the airports given");
}
*/
/*
std::vector<Route> VuelaFlight::origRoutesSearch(std::string airportIataOrig) {
    std::list<Route>::iterator iterator = routes.begin();
    unsigned int count = 0;
    unsigned int found_count = 0;
    std::vector<Route> found_routes;
    while(count < routes.size() and iterator != routes.end()){
        if (iterator->getOrigin()->getIata() == airportIataOrig){
            found_routes.push_back(*iterator);
            found_count++;
        }
        iterator++;
        count++;
    }
    if(found_count == 0){
        throw std::invalid_argument("---FATAL ERROR-- Doesn't exist a route in the database specified with the origin airport given");
    }
    return found_routes;
}
*/
/*
std::vector<Airport> VuelaFlight::countryAirportSearch(std::string country) {
    std::list<Route>::iterator iterator = routes.begin();
    unsigned int count = 0;
    unsigned int found_count = 0;
    std::vector<Airport> found_airports;
    while(count < routes.size() and iterator != routes.end()){
        if (iterator->getOrigin()->getCountryIso() == country or iterator->getDestination()->getCountryIso() == country){
            Airport* airport_to_push = iterator->getOrigin();
            found_airports.push_back(*airport_to_push);
            found_count++;
        }
        iterator++;
        count++;
    }
    if(found_count == 0){
        throw std::invalid_argument("---FATAL ERROR-- Doesn't exist a route in the database specified with the origin airport given");
    }
    return found_airports;
}
*/
/*
void VuelaFlight::addNewRoute(std::string origAirportIata, std::string destAirportIata, std::string airline) {
    Airport* orig = airports.search(destAirportIata);
    if (orig == nullptr){
        throw std::invalid_argument("---FATAL ERROR-- Doesn't exist a route in the database specified with the origin airport IATA given");
    }
    Airport* dest = airports.search(destAirportIata);
    if (dest == nullptr){
        throw std::invalid_argument("---FATAL ERROR-- Doesn't exist a route in the database specified with the destination airport IATA given");
    }
    Airline auxAirline(airline);
    Route new_route(&airlines.find(auxAirline.getIcao())->second, orig, dest);
    routes.push_back(new_route);

}
*/
/*
const std::list<Route> &VuelaFlight::getRoutes() const {
    return routes;
}

Airline& VuelaFlight::searchAirline(std::string airlineIcao) {
    Airline result = airlines.find(airlineIcao)->second;

    std::cout << "Data of airline: Id " << result.getId() << ", Name " << result.getName() << ", country " << result.getCountry() << " and is active? " << result.isActive();
    return result;
}
*/
/*
//FIX THIS
std::vector<Airline> VuelaFlight::searchActiveAirline() {
    std::vector<Airline*> data = airlines.readAVL();
    std::vector<Airline> result;
    for (unsigned int i=0; i < data.used_tam();i++){
        if(data[i]->isActive())
            result.push(*data[i]);
    }
    return result;
}
*/

