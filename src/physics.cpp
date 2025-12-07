#include "../include/physics.h"


void Phys::step() {
    //Add mass
    TOTAL_MASS = dry_mass + STAR_37_XFB_FUEL + HYDRAZINE_FUEL + COLD_GAS_FUEL;
    

    //Run VEL
    leap_frog();
    
    //Run ANG
    dquat av = {0.0f, rate.x,rate.y,rate.z};

    attitude += 0.5 * attitude * av * deltaTime;

    attitude = normalize(attitude);
    //attitude * rate;

    //Zero acc before stepping next
    T_ACC = dvec3 { 0,0,0 };


    //Reset if below earth for testing
    if (sqrt( glm::dot(POS,POS)) < planet.radius) {
        std::cout << "Crash!\n";
        POS = {1799.209 * 1000, -3960.856 * 1000, 5797.431 * 1000};
        VEL = {-4.08207 * 1000, 4.3215701 * 1000, 4.413379 * 1000};
    }
}

//Used for star37
void Phys::kick_motor(float newtons) {
    vec3 forward = attitude * dvec3(0,0,1); //forward
    T_ACC += forward * (newtons / TOTAL_MASS);
}


//Calculate how direct the solar panel is
float Phys::solar_panel_directivity(dvec3 local_panel_normal) {   //Directly forward is  0 0 1
    vec3 sun_direction = normalize(SUN - POS);
    vec3 global_panel_normal = normalize(attitude * local_panel_normal); //transform panel into world space

    return max(dot(global_panel_normal,sun_direction),0.0f);    //Incidence angle
}

// TODO: add oblateness
//  Calculate gravity force
glm::dvec3 Phys::grav_f() {
    double r2, r3;

    r2 = glm::dot(POS,POS); //distance**
    r3 = r2 * sqrt(r2);
    auto pull_earth =  -planet.mu * POS / r3;

    //For this simulation, its negligible so we just use this one value
    r2 = SUN.x;
    r3 = r2 * sqrt(r2);

    auto pull_sun = -home.mu * POS / r3;
    //shit
    return pull_earth;
}

//  Step forward
void Phys::leap_frog() {
    dvec3 acc_curr;
    double ddt = (double) deltaTime;

    //Current time sep acceleration
    acc_curr = grav_f() += T_ACC; 
    VEL += 0.5 * ddt* acc_curr; //Kick
    POS += ddt*VEL; //Drift
    acc_curr = grav_f() + T_ACC; //New Kick
    //Propagate
    VEL += 0.5 * ddt* acc_curr;

    return;
}