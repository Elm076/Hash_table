cmake_minimum_required(VERSION 3.26)
project(Hash_table)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Hash_table
        Airline.cpp
        Airline.h
        Airport.cpp
        Airport.h
        AVL.h
        Dynamic_container.h
        Iterator.cpp
        Iterator.h
        Linked_list.h
        main.cpp
        NodeLinkedList.h
        Route.cpp
        Route.h
        UTM.cpp
        UTM.h
        VuelaFlight.cpp
        VuelaFlight.h
        ThashAirp.h
        ThashAirp.cpp
        fecha.cpp
        fecha.h
        Flight.cpp
        Flight.h)
