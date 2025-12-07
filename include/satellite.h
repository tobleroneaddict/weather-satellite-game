#pragma once
#include "globals.h"
#include "physics.h"
#include "commands.h"

using namespace glm;


struct Power_System {
    float BUS_VOLTAGE_A = 27;
    float BUS_CURRENT_A;
    float BUS_VOLTAGE_B = 29;
    float BUS_CURRENT_B;

    float SOLAR_INCIDENCE;
    float SOLAR_AREA;
    float SOLAR_VOLTAGE;
    

    //Outputs from Z80
    //Only switch one bus high
    bool BUS_RELAY_A = true;
    bool BUS_RELAY_B = false;
};

struct Thermistors {
    int body;    
};

struct Command_antenna {
    command_packet   uplink = {VERB_MAX,0,0,0,0};
    return_packet  downlink;
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

class Satellite; //Forward dec

enum ADCS_MODES {
    INERTIAL_GUIDANCE_MODE, //look to target/ pause rates
    STANDARD_GUIDANCE_MODE, //Prograde facing
    STEERING_GUIDANCE_MODE  //Steer to target    

};
//Thermal / Power
class TECS {
    public:
    void reset();
    void step(Satellite* sat);
    private:      
};

//Sci
class SDPU {
    public:
    void step(Satellite* sat);
    private:
};
//Attitude
class ADCS {
    public:
    //RPY
    PID pid;
    dquat targetq;
    double t_roll,t_pitch,t_yaw; //transmit these values
    double rate_roll,rate_pitch,rate_yaw;
    double rate_limit_roll, rate_limit_pitch, rate_limit_yaw;

    ADCS_MODES mode;
    void reset();
    dquat get_error(dquat current);
    void step(Satellite* sat);
    private:
    
};
//Comms
class COMM {
    public:
    void step(Satellite* sat);
    void clear();
    private:
    float telemetry_timer;
    void run_command(Satellite* sat);
    command_packet queue[256] = {}; int queue_store_position = 0; //Stack, will grow UP! >:3
    int current_queue_position = 0; //Advances every tick, no commands run until RTC >= TTG.
    //If an instant (<256 steps) command is desired, TTG 0 may be used:
    //Uplinked commands will go here waiting for RTC >= TTG.
};

class Satellite {
public:
    Phys physics;

    //Physical stuff
    Scanner scanner;
    Power_System power;
    double rtc;
    //Send L band packets to satellite
    Command_antenna tantenna;

    
    
    //Compute
    TECS sys_tecs;
    SDPU sys_sdpu;
    ADCS sys_adcs;
    COMM sys_comm;
    
    void step_simulation();

    void init();
    //Power system worked on at home
private:
    

    

    float step_current; //power draw of this current step
    void sim_mirror_motor();

};