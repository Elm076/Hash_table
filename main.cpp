

#include <iostream>
#include <string>
#include <ctime>
#include "Dynamic_container.h"
#include "Airport.h"
#include "Linked_list.h"
#include "Iterator.h"
#include "AVL.h"
#include "VuelaFlight.h"

/**
 * @author Adrián González Pérez agp00118@red.ujaen.es
 */

int main() {
    std::cout << ".....Start of the program ....." << std::endl;


    VuelaFlight dataBase("..\\aeropuertos_v3.csv", "..\\rutas_v1.csv", "..\\aerolineas_v1.csv");
    std::cout << std::endl;



    std::cout << "----- End of the program -----" << std::endl;
}