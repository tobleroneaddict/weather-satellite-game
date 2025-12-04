#pragma once
#include "globals.h"

enum VERB { //All these can be delayed by the delay in the command packet (added to the queue)
    //System
    SET_TIME_BASE, //Sets internal clock to (x << 32) | (y << 16) | (z)
    GET_TIME_BASE,
    RESET_SYSTEM, //X: select system
    

    ACDS_STEER_TARGET,    //Target P,R,Y 
    ACDS_MODE_INERTIAL, //Hold target
    ACDS_MODE_NORMAL,
    ACDS_GET_IMU, // Returns IMU Angle if X is false, IMU rot rate if X is true



    //Communication
    COMM_SET_MODE, //X: VHF, Y: L Band, Z: SAR    enable/disable
    COMM_SET_POWER, //X: VHF hi/lo Y: Lb hi/lo
    
    //Recording of scientific data to tape
    TAPE_SET_DRIVE, //X: Start,stop     Y: forward/rewind       Z: high/low speed
    TAPE_SET_MODE, //X: Record/Play     Y: VHF,L
    //Tape will yap to S band

    //Sensors
    SCANNER_ENABLE_POWER,
    SCANNER_DISABLE_POWER,
    


    
    //Power & Thermals
    THERMAL_CONTROL_AUTO,
    THERMAL_CONTROL_OVERRIDE,
    SOLAR_TRACK_AUTO,
    SOLAR_TRACK_OVERRIDE,
    BUS_SWITCH_A,
    BUS_SWITCH_B,
    GET_BUS_DATA, //Based on X being true or false, returns V << 16 | A of selected Bus


    //Thruster, releasing of pyro/latches
    PYRO_SET_FIRE,
    PYRO_SET_SAFE,
    SOLAR_DEPLOY,
    CAMERA_COVER_DEPLOY,
    COMM_L_DEPLOY,
    COMM_VHF_DEPLOY,

    BURN_SET_DV,
    //Set angle by IMU
    BURN_START,   //Takes no XYZ inputs
    VERB_MAX

};

//Command packets happen over S band communication
struct command_packet {
    VERB data_verb;
    uint16_t x, y, z;
    uint64_t delay; //millseconds from Time Base
};

struct return_packet {
    uint16_t x, y, z;
    uint32_t result;
};