#pragma once
#include "globals.h"
#include "z80.h"
//#include "commands.h"
using namespace glm;

/*
Silly plan:
Z80 for processing

SDP:
responsible for reading scientific data from sensors, combining them into a packet for transmit.


EPP Monitor
Solar, battteries, all that stuff

NAP


Will compile C into z80 binaries



Power system monitoring






*/

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

class SDP { //Scientific Data Processor Unit
public:
    Z80Context chip;


    //INPUTS

    //Scanner
    uint8_t BODY_TEMP;
    uint8_t PATCH_TEMP;
    uint16_t scanner_channel_1;
    uint16_t scanner_channel_2;
    uint16_t scanner_channel_3a;
    uint16_t scanner_channel_3b;
    uint16_t scanner_channel_4;   
    uint16_t MIRROR_ROTOR_ENCODER;

    



    //OUTPUTS
    bool MIRROR_POWER;




    void init();
};

class EPP { //Electrical Power Processor
public:
    Z80Context chip;
    
    //MMIO pins

    //INPUT INPUT INPUT Analog inputs from sensors,  0-65536
    //0 is 0V, 65536 is 100V
    uint8_t BUS_VOLTAGE_A;
    uint8_t BUS_CURRENT_A;
    uint8_t BUS_VOLTAGE_B;
    uint8_t BUS_CURRENT_B;

    uint16_t SOLAR_ROTOR_ENCODER;
    uint8_t SOLAR_VOLTAGE;
    

    //OUTPUT OUTPUT OUTPUT outputs from MMIO
    //Only switch one bus high
    bool BUS_RELAY_A;
    bool BUS_RELAY_B;
    //PWM Controlled left/right outputs
    bool SOLAR_ROTATE_LEFT;
    bool SOLAR_ROTATE_RIGHT;






    void init();
};

class TCP { //Thermal Control Processor
public:
    Z80Context chip;

    //INPUTS


    void init();
};

//I'm not going to simulate any IMU torquing lol 
struct IMMSU_Data { //Contains data from the magnets too
    vec3 acceleration; // m/s
    vec3 angular_rate; // rad/s
    vec3 magnetic_strength; // Gauss
};

class NAP { //Navigation & Attitude Processor
public:
    Z80Context chip;

    uint16_t accX;
    uint16_t accY;
    uint16_t accZ;
    uint16_t angrateX;
    uint16_t angrateY;
    uint16_t angrateZ;
    uint16_t magX;
    uint16_t magY;
    uint16_t magZ;

    
    void init();
};



//Recieve commands from S band antenna and feed command data into this z80
class Command_Handler_System {
    Z80Context chip;
    

    //INPUTS
    
    
    void init();
};

class Satellite {
public:
    //Power system worked on at home

    //Processing units
    SDP dp;
    TCP thermal;
    NAP att;
    EPP power;
    

    Command_Handler_System ch;

    Scanner scanner;

};