#include "../include/physics.h"


void Phys::step() {
    //Run VEL
    leap_frog();
    //Run ROT, since its a feedback loop i shouldnt need to accurately integrate it like orbits (?)
    //**   sctiuffff */

}





//  Calculate gravity force
glm::dvec3 Phys::grav_f() {
    double r2, r3;

    r2 = glm::dot(POS,POS); //distance**
    r3 = r2 * sqrt(r2);
    return -planet.mu * POS / r3;
}

//  Step forward
void Phys::leap_frog() {
    dvec3 acc_curr;
    double ddt = (double)deltaTime / 100;

    //Current time sep acceleration
    acc_curr = grav_f() += T_ACC; 
    VEL += 0.5 * ddt* acc_curr; //Kick
    POS += ddt*VEL; //Drift
    acc_curr = grav_f() + T_ACC; //New Kick
    //Propagate
    VEL += 0.5 * ddt* acc_curr;

    return;
}