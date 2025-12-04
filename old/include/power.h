#pragma once

#include "globals.h"

//Main bus, power, etc.

struct Solar_Panel {
    float angle; //rotator
    float area;
};

struct Battery_Bus { //Combined battery banks
    float energy = 400; //Watt hours,       right now
    float energy_capacity = 450; //Watt hours, max
};


struct Power_Bus {
    float target_voltage = 28.0f; //Goldilocks voltage

    //HV Busses
    Battery_Bus BUS_A;
    Battery_Bus BUS_B;
    
    float BUS_A_Voltage;
    float BUS_B_Voltage;

    float BUS_A_Current;
    float BUS_B_Current;
    
};