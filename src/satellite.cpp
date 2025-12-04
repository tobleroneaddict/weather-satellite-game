#include "../include/satellite.h"

using namespace std;

void Satellite::step_simulation() {
    //Check 28V power state
    bool haspower,bus_A_nominal,bus_B_nominal;
    bus_A_nominal = (power.BUS_VOLTAGE_A > 23.0f && power.BUS_VOLTAGE_A < 35) ? true : false;   // In Range
    bus_B_nominal = (power.BUS_VOLTAGE_B > 23.0f && power.BUS_VOLTAGE_B < 35) ? true : false;   
    haspower = (bus_A_nominal && power.BUS_RELAY_A) || (bus_B_nominal && power.BUS_RELAY_B); //If A good on A, or B good on B


    //Run electrical systems
    if (haspower) {
        Z80Execute(&dataproc.chip);   //Scientific Data Processor Unit
        Z80Execute(&attitude.chip);   //Navigation & Attitude Processor
        Z80Execute(&powchip.chip);   //Electrical Power Processor
        Z80Execute(&CMDhandler.chip);  //Ground command handler
        cout << "step!\n";
    } else {
        cout << "dead!\n";
    }
}





//Start Z80s
void Satellite::init() {
    dataproc.init();
    attitude.init();
    powchip.init();
    CMDhandler.init();
}




void SDP::init() {clear_ram(ram); Z80RESET(&chip);}
void NAP::init() {clear_ram(ram); Z80RESET(&chip);}
void EPP::init() {clear_ram(ram); Z80RESET(&chip);}
void CHS::init() {clear_ram(ram); Z80RESET(&chip);}