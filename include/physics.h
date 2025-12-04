#pragma once
#include "globals.h"

//Satellite motions
using namespace glm;

struct Earth {
    double radius = 6378137.0; //m
    double mu = 3.986004418e14; //m3 s-2
    double J2 = 1.08262668e-3; //oblateness coeff
    double rotation_rate = 7.2921159e-5; //rad/s
};

struct Phys {
    //Earth Centered Inertial
    vec3 POS;
    vec3 VEL;
    vec3 attitude; //should make quaternion......

    //Mass
    double dry_mass = 1479; //kg
    double wet_mass =  753; //kg

    //Drag
    double cross_section_meters; //used for drag calcs
    double Cd, Cr;  
};