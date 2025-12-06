#include "../include/satellite.h"

using namespace std;


//Thermal Electrical CS
void TECS::step(Satellite* sat) {

}
void TECS::reset() {
    
}

//Scientific Data Processing Unit
void SDPU::step(Satellite* sat) {

}

//Attitude Determination & Control System
void ADCS::step(Satellite* sat) {

    dvec3 v = -normalize(sat->physics.VEL);
    dvec3 nadir = -normalize(sat->physics.POS);

    dvec3 up = normalize(nadir - v * dot(nadir, v));

    dquat targetq = quatLookAtRH(up,up);

    //targetq = dquat{-normalize(sat->physics.VEL)};    //45,45,45 global
    //dquat error = get_error(sat->physics.attitude); //Get error to target quat
    //cout << error.x << endl;
    sat->physics.attitude = targetq;
}
void ADCS::reset() {
    mode = INERTIAL_GUIDANCE_MODE;

}
dquat ADCS::get_error(dquat current) {
    return targetq * conjugate(current);
}


void COMM::run_command(Satellite* sat) {
    command_packet* cm = &queue[current_queue_position];

    //Packet registers
    double* x_down = &sat->tantenna.downlink.x;
    double* y_down = &sat->tantenna.downlink.y;
    double* z_down = &sat->tantenna.downlink.z;
    double* r_down = &sat->tantenna.downlink.result;
    
    
    switch (cm->data_verb) {
        case (VERB_NOOP): cout << "This shouldnt happen in this func\n";
        break;
        case (SET_TIME_BASE): 
        sat->rtc = cm->x;   //Set RTC and return result
        *r_down = sat->rtc;
        break;
        case (GET_TIME_BASE): 
        *r_down = sat->rtc;
        break;
        case (RESET_SYSTEM): 
        sat->init(); //This control flow might be buggywuggy
        break;
        case (SAFE_MODE): break;
        case (TELEMETRY_MODE_SET): break;

        case (ADCS_STEER_TO_TARGET): //Set angles
        sat->sys_adcs.t_roll = cm->x;
        sat->sys_adcs.t_pitch = cm->y;
        sat->sys_adcs.t_yaw = cm->z;
        
        break; //command target
        case (ADCS_SET_MODE_TARGET): sat->sys_adcs.mode = STEERING_GUIDANCE_MODE; break;
        case (ADCS_SET_MODE_INERTIAL):sat->sys_adcs.mode = INERTIAL_GUIDANCE_MODE; break;
        case (ADCS_SET_MODE_NORMAL): sat->sys_adcs.mode = STANDARD_GUIDANCE_MODE; break;
        case (ADCS_GET_MODE):
        {switch (sat->sys_adcs.mode) {
                case (INERTIAL_GUIDANCE_MODE):*r_down = 0; break;
                case (STANDARD_GUIDANCE_MODE):*r_down = 1; break;
                case (STEERING_GUIDANCE_MODE):*r_down = 2; break;
        }} break;

        case (ADCS_GET_QUATERNION): break;
        case (ADCS_RATE_LIMIT):
        sat->sys_adcs.rate_limit_roll = cm->x;
        sat->sys_adcs.rate_limit_pitch = cm->y;
        sat->sys_adcs.rate_limit_yaw = cm->z;
        break;
        case (ADCS_GET_RATES): 
        *x_down = sat->sys_adcs.rate_roll;
        *y_down = sat->sys_adcs.rate_pitch;
        *z_down = sat->sys_adcs.rate_yaw;
        *r_down = 1;
        break;
        case (ADCS_GET_MAG): break;
        case (ADCS_SET_RW_SPEED): break;
        case (ADCS_UNLOAD_MOMENTUM): break;
        

        case (COMM_SET_MODE): break;
        case (COMM_SET_POWER): break;
        case (COMM_APT_SET): break;
        case (COMM_SET_VHF_BEACON): break;
        case (TAPE_SET_DRIVE): break;
        case (TAPE_SET_MODE): break;
        
        case (SCANNER_ENABLE_POWER):  sat->scanner.mirror_power =  true; break;
        case (SCANNER_DISABLE_POWER): sat->scanner.mirror_power = false; break;
        case (SCANNER_SET_GAIN): break;
        case (SCANNER_SET_FRAME): break;
        case (SOLAR_GET_SENSORS): break;
        case (THERMAL_GET_SENSORS): break;
        case (SEM_SET_POWER): break;
        

        case (THERMAL_CONTROL_AUTO): break;
        case (THERMAL_CONTROL_OVERRIDE): break;

        
        case (BUS_SWITCH_A): break;
        case (BUS_SWITCH_B): break;
        case (GET_BUS_DATA): break;
        case (PYRO_SET_FIRE): break;
        case (PYRO_SET_SAFE): break;
        case (SOLAR_DEPLOY): break;
        case (CAMERA_COVER_DEPLOY): break;
        case (COMM_L_DEPLOY): break;
        case (COMM_VHF_DEPLOY): break;
        case (BURN_SET_DELTAS): break;  //NOAA-17 did not have any maneuvering but this is still here for if i wanna reuse this software
        case (BURN_EXECUTE): break;     //Starts the STAR-37 AKM
        case (BURN_ABORT): break;
        case (GET_TANK_PRESSURE): break;
        case (VERB_MAX): cout << "This shouldnt happen in this func 2\n"; break; //Unused
    }
}
//Telemetry processor
void COMM::step(Satellite* sat) {
    //COMMAND
    command_packet * up = &sat->tantenna.uplink;
    return_packet  * dn = &sat->tantenna.downlink;
    dn->x = dn->y = dn->z = dn->result = 0; //Reset downlink packet for run_command() modification

    //Fetch new data from antenna buffer and queue it up, as long as theres enough room
    if (up->data_verb != VERB_NOOP && queue_store_position < 255) {
        //Store
        queue[queue_store_position].data_verb = up->data_verb;
        queue[queue_store_position].x = up->x;
        queue[queue_store_position].y = up->y;
        queue[queue_store_position].z = up->z;
        queue[queue_store_position].TTG = up->TTG;
        //Advance stack
        queue_store_position++;
    }

    //QUEUE
    //Run command
    if (queue[current_queue_position].data_verb != VERB_NOOP) {
        //..But only if its time
        if (sat->rtc >= queue[current_queue_position].TTG) {
            //cout << "running cmd at ix " << current_queue_position << "." << queue_store_position << "V:" << queue[current_queue_position].data_verb << endl;
            run_command(sat); //Run and downlink result
            queue[current_queue_position].data_verb = VERB_NOOP; //Stale this
            queue_store_position--; //Pull back stack ptr
        }
    }
    current_queue_position++; //Increment qp
    if (current_queue_position > queue_store_position) current_queue_position = 0; //End of command stack
    
    //Make stale
    sat->tantenna.uplink.data_verb = VERB_NOOP; //Verb max is nil command
    //END COMMAND
    //TELEMETRY
    telemetry_timer += deltaTime;
    if (telemetry_timer >= 1) {
        telemetry_timer = 0;
        double altitude = sqrt((sat->physics.POS.x * sat->physics.POS.x) + (sat->physics.POS.y * sat->physics.POS.y) + (sat->physics.POS.z * sat->physics.POS.z));
        altitude -= sat->physics.planet.radius;
        //cout << altitude / 1000 << "km" << endl;
        
    }
    //END TELEMETRY


}
void COMM::clear() {
    for (int i = 0; i < 256; i++) {queue->data_verb = VERB_NOOP;}
    current_queue_position = queue_store_position = 0;
}

void Satellite::step_simulation() {
    rtc += deltaTime; //RTC has a backup battery
    
    //cout << (glm::dot(physics.POS,physics.POS)) << endl;

    //Check 28V power state
    bool haspower,bus_A_nominal,bus_B_nominal;
    bus_A_nominal = (power.BUS_VOLTAGE_A > 23.0f && power.BUS_VOLTAGE_A < 35) ? true : false;   // In Range
    bus_B_nominal = (power.BUS_VOLTAGE_B > 23.0f && power.BUS_VOLTAGE_B < 35) ? true : false;   
    haspower = (bus_A_nominal && power.BUS_RELAY_A) || (bus_B_nominal && power.BUS_RELAY_B); //If A good on A, or B good on B

    //Amps
    step_current = 2.750; //accumulator,add a little bit of compute power


    //needs 833 watts to survive(?)

    dvec3 panel_angle = {    0, cos(120.0f),sin(120.0f)    };
    power.SOLAR_INCIDENCE = physics.solar_panel_directivity(panel_angle);
    

    //physics.kick_motor(-0.001);


    //Run electrical systems
    if (haspower) {
        sim_mirror_motor();

        //Step computers
        sys_tecs.step(this);
        sys_sdpu.step(this);
        sys_adcs.step(this);
        sys_comm.step(this);


    } else {
        cout << "dead!\n";
    }

    //do something w power draw

    physics.step();
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
    sys_tecs.reset();
    sys_comm.clear();
    sys_adcs.reset();
    //sys_sdpu.reset();
}