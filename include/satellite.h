#pragma once
#include "globals.h"
#include "z80.h"
//#include "commands.h"
using namespace glm;


struct Power_System {
    float BUS_VOLTAGE_A = 27;
    float BUS_CURRENT_A;
    float BUS_VOLTAGE_B = 29;
    float BUS_CURRENT_B;

    float SOLAR_ROTOR_ENCODER;
    float SOLAR_VOLTAGE;
    

    //Outputs from Z80
    //Only switch one bus high
    bool BUS_RELAY_A = true;
    bool BUS_RELAY_B = false;
    //PWM Controlled left/right outputs
    bool SOLAR_ROTATE_LEFT;
    bool SOLAR_ROTATE_RIGHT;

};

struct Thermistors {
    int body;    
};

struct Scanner {
    float body_temp;
    float patch_temp;

    float scanner_channel_1;
    float scanner_channel_2;
    float scanner_channel_3a;
    float scanner_channel_3b;
    float scanner_channel_4;  

    float mirror_encoder;
    bool mirror_power;
};


struct IMMSU_Data { //Contains data from the magnets too
    vec3 acceleration; // m/s
    vec3 angular_rate; // rad/s
    vec3 magnetic_strength; // Gauss
};



class Satellite {
public:
    Power_System power;

    void step_simulation();
    void init();
    //Power system worked on at home
private:
    Scanner scanner;


};