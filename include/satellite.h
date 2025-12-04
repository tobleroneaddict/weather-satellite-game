#pragma once
#include "globals.h"
#include "physics.h"
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
    float SOLAR_ROTATE_LEFT;
    float SOLAR_ROTATE_RIGHT;

};

struct Thermistors {
    int body;    
};

struct Scanner {
    float body_temp;
    float patch_temp;

    float scanner_channel_1;    //0
    float scanner_channel_2;    //1
    float scanner_channel_3a;   //2
    float scanner_channel_3b;   //3
    float scanner_channel_4;    //4
    float scanner_channel_5;    //5
    float scanner_channel_6;    //6
    

    float mirror_tgt_velocity = 37.6991118; //rad/s
    float mirror_position;
    bool mirror_power;
};


struct IMMSU_Data { //Contains data from the magnets too
    vec3 acceleration; // m/s
    vec3 angular_rate; // rad/s
    vec3 magnetic_strength; // Gauss
};



class Satellite {
public:
    Phys physics;

    Power_System power;

    void step_simulation();
    void init();
    //Power system worked on at home
private:
    Scanner scanner;

    float step_current; //power draw of this current step
    void sim_mirror_motor();
};