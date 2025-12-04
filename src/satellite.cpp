#include "../include/satellite.h"

using namespace std;

void Satellite::step_simulation() {
    physics.step();
    //cout << (glm::dot(physics.POS,physics.POS)) << endl;

    //Check 28V power state
    bool haspower,bus_A_nominal,bus_B_nominal;
    bus_A_nominal = (power.BUS_VOLTAGE_A > 23.0f && power.BUS_VOLTAGE_A < 35) ? true : false;   // In Range
    bus_B_nominal = (power.BUS_VOLTAGE_B > 23.0f && power.BUS_VOLTAGE_B < 35) ? true : false;   
    haspower = (bus_A_nominal && power.BUS_RELAY_A) || (bus_B_nominal && power.BUS_RELAY_B); //If A good on A, or B good on B

    //Amps
    step_current = 2.750; //accumulator

    //Run electrical systems
    if (haspower) {
        sim_mirror_motor();



    } else {
        cout << "dead!\n";
    }

    //do something w power draw
}

void Satellite::sim_mirror_motor() {
    if (scanner.mirror_power) {
        step_current += 0.500;
        scanner.mirror_position += scanner.mirror_tgt_velocity * deltaTime;
    }
    scanner.mirror_position = fmod(scanner.mirror_position,M_PI * 2);
}


void Satellite::init() {
    scanner.mirror_power = true;
}