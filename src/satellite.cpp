#include "../include/satellite.h"

using namespace std;


//Thermal Electrical CS
void TECS::step(Satellite* sat) {

}

//Scientific Data Processing Unit
void SDPU::step(Satellite* sat) {

}

//Attitude Determination CS
void ADCS::step(Satellite* sat) {

}

void COMM::run_command(Satellite* sat) {
    command_packet* cm = &queue[current_queue_position];
    switch (cm->data_verb) {
        case (VERB_NOOP): cout << "This shouldnt happen in this func\n";
        break;
        case (SET_TIME_BASE): 
        sat->rtc = cm->x;   //Set RTC and return result
        sat->tantenna.downlink.result = sat->rtc;
        break;
        case (GET_TIME_BASE): 
        sat->tantenna.downlink.result = sat->rtc;
        break;
        case (RESET_SYSTEM): 
        sat->init(); //This control flow might be buggywuggy
        break;
        case (SAFE_MODE): break;
        case (TELEMETRY_MODE_SET): break;
        case (ADCS_STEER_TARGET): break;
        case (ADCS_MODE_INERTIAL): break;
        case (ADCS_MODE_NORMAL): break;
        case (ADCS_GET_MODE): break;
        case (ADCS_GET_QUATERNION): break;
        case (ADCS_RATE_LIMIT): break;
        case (ADCS_GET_RATES): break;
        case (ADCS_GET_MAG): break;
        case (ADCS_SET_RW_SPEED): break;
        case (ADCS_UNLOAD_MOMENTUM): break;
        

        case (COMM_SET_MODE): break;
        case (COMM_SET_POWER): break;
        case (COMM_APT_SET): break;
        case (COMM_SET_VHF_BEACON): break;
        case (TAPE_SET_DRIVE): break;
        case (TAPE_SET_MODE): break;
        case (SCANNER_ENABLE_POWER): break;
        case (SCANNER_DISABLE_POWER): break;
        case (SCANNER_SET_GAIN): break;
        case (SCANNER_SET_FRAME): break;
        case (SOLAR_GET_SENSORS): break;
        case (THERMAL_GET_SENSORS): break;
        case (SEM_SET_POWER): break;
        

        case (THERMAL_CONTROL_AUTO): break;
        case (THERMAL_CONTROL_OVERRIDE): break;
        case (SOLAR_TRACK_AUTO): break;
        case (SOLAR_TRACK_OVERRIDE): break;
        
        case (BUS_SWITCH_A): break;
        case (BUS_SWITCH_B): break;
        case (GET_BUS_DATA): break;
        case (PYRO_SET_FIRE): break;
        case (PYRO_SET_SAFE): break;
        case (SOLAR_DEPLOY): break;
        case (CAMERA_COVER_DEPLOY): break;
        case (COMM_L_DEPLOY): break;
        case (COMM_VHF_DEPLOY): break;
        case (BURN_SET_DELTAS): break;
        case (BURN_EXECUTE): break;
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
        cout << altitude / 1000 << "km" << endl;
        
    }
    //END TELEMETRY


}
void COMM::clear() {
    for (int i = 0; i < 256; i++) {queue->data_verb = VERB_NOOP;}
    current_queue_position = queue_store_position = 0;
}

void Satellite::step_simulation() {
    rtc += deltaTime; //RTC has a backup battery
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

        //Step computers
        sys_tecs.step(this);
        sys_sdpu.step(this);
        sys_adcs.step(this);
        sys_comm.step(this);


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
    sys_comm.clear();
}