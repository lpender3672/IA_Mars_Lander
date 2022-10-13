// Mars lander simulator
// Version 1.11
// Mechanical simulation functions
// Gabor Csanyi and Andrew Gee, August 2019

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation, to make use of it
// for non-commercial purposes, provided that (a) its original authorship
// is acknowledged and (b) no modified versions of the source code are
// published. Restriction (b) is designed to protect the integrity of the
// exercise for future generations of students. The authors would be happy
// to receive any suggested modifications by private correspondence to
// ahg@eng.cam.ac.uk and gc121@eng.cam.ac.uk.

#include "lander.h"
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

void autopilot (void)
  // Autopilot to adjust the engine throttle, parachute and attitude control
{
  // INSERT YOUR CODE HERE
    double altitude, Kp, Ka, Kv, error, P_out, P_threshold;
	
    altitude = position.abs() - MARS_RADIUS;
    Kp = 0.4;
    Ka = 0.02;
    Kv = 1.0;
    P_threshold = 0.4;

    if (scenario == 7) {
        double lander_mass = UNLOADED_LANDER_MASS + fuel * FUEL_CAPACITY * FUEL_DENSITY;
        double Feq = GRAVITY * lander_mass * MARS_MASS / position.abs2();
        throttle = Feq / MAX_THRUST;
        return;
    }
	
    error = -(0.5 + Ka * altitude + Kv * velocity * position.norm());
    P_out = Kp * error;
	
    if (P_out <= - P_threshold) {
        throttle = 0;
    }
    else  if (P_out < 1 - P_threshold) {
        throttle = P_threshold + P_out;
    }
    else {
        throttle = 1;
    }

    if (save_data) {
        if (fout) { // file opened successfully
            fout << simulation_time << ' ' << altitude << ' ' << velocity * position.norm() << endl;
        }
        else { // file did not open successfully
            cout << "Could not open trajectory file for writing" << endl;
        }
    }
}

vector3d force(vector3d position, vector3d velocity) {
	
    vector3d force = thrust_wrt_world();

    double lander_area = 3.14159 * pow(LANDER_SIZE, 2);
    double lander_mass = UNLOADED_LANDER_MASS + fuel * FUEL_CAPACITY * FUEL_DENSITY;
	

    // drag
    vector3d lander_drag = - 0.5 * DRAG_COEF_LANDER * lander_area * atmospheric_density(position) * velocity.abs2() * velocity.norm();
    force += lander_drag;
	
    if (parachute_status == DEPLOYED) {
        double chute_area = 5 * 4 * pow(LANDER_SIZE, 2);
		vector3d chute_drag = - 0.5 * DRAG_COEF_CHUTE * chute_area * atmospheric_density(position) * velocity.abs2() * velocity.norm();
        force += chute_drag;
    }

    // gravity
    vector3d gravitational_force = - GRAVITY * lander_mass * MARS_MASS * position.norm() / position.abs2();
    force += gravitational_force;

    return force;
}

void verlet_update(void) {
    double lander_mass = UNLOADED_LANDER_MASS + fuel * FUEL_CAPACITY * FUEL_DENSITY;
    static vector3d prev_position;
    vector3d next_position;

    if (simulation_time == 0.0) {
        next_position = position + delta_t * velocity;
        velocity = velocity + delta_t * force(position, velocity) / lander_mass;
    }
    else {
        next_position = 2 * position - prev_position + delta_t * delta_t * force(position, velocity) / lander_mass;
        velocity = 1 / (2 * delta_t) * (next_position - prev_position);
    }
    prev_position = position;
    position = next_position;
}
void euler_update(void) {
    double lander_mass = UNLOADED_LANDER_MASS + fuel * FUEL_CAPACITY * FUEL_DENSITY;
	velocity = velocity + delta_t * force(position, velocity) / lander_mass;
	position = position + delta_t * velocity;
}

void numerical_dynamics (void)
  // This is the function that performs the numerical integration to update the
  // lander's pose. The time step is delta_t (global variable).
{
  // INSERT YOUR CODE HERE
  // append current state to trajectories

    verlet_update();

  // Here we can apply an autopilot to adjust the thrust, parachute and attitude
  if (autopilot_enabled) autopilot();

  // Here we can apply 3-axis stabilization to ensure the base is always pointing downwards
  if (stabilized_attitude) attitude_stabilization();
}

void initialize_simulation (void)
  // Lander pose initialization - selects one of 10 possible scenarios
{
  // The parameters to set are:
  // position - in Cartesian planetary coordinate system (m)
  // velocity - in Cartesian planetary coordinate system (m/s)
  // orientation - in lander coordinate system (xyz Euler angles, degrees)
  // delta_t - the simulation time step
  // boolean state variables - parachute_status, stabilized_attitude, autopilot_enabled
  // scenario_description - a descriptive string for the help screen

  scenario_description[0] = "circular orbit";
  scenario_description[1] = "descent from 10km";
  scenario_description[2] = "elliptical orbit, thrust changes orbital plane";
  scenario_description[3] = "polar launch at escape velocity (but drag prevents escape)";
  scenario_description[4] = "elliptical orbit that clips the atmosphere and decays";
  scenario_description[5] = "descent from 200km";
  scenario_description[6] = "areostationary orbit";
  scenario_description[7] = "";
  scenario_description[8] = "";
  scenario_description[9] = "";

  vector3d a, position_prev, position_current;
  vector<double> t_list;
  vector<vector3d> x_list, v_list;

  switch (scenario) {

  case 0:
    // a circular equatorial orbit
    position = vector3d(1.2*MARS_RADIUS, 0.0, 0.0);
    velocity = vector3d(0.0, -3247.087385863725, 0.0);
    orientation = vector3d(0.0, 90.0, 0.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = false;
    autopilot_enabled = false;
    break;

  case 1:
    // a descent from rest at 10km altitude
    position = vector3d(0.0, -(MARS_RADIUS + 10000.0), 0.0);
    velocity = vector3d(0.0, 0.0, 0.0);
    orientation = vector3d(0.0, 0.0, 90.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = true;
    autopilot_enabled = true;
    save_data = true;
    break;

  case 2:
    // an elliptical polar orbit
    position = vector3d(0.0, 0.0, 1.2*MARS_RADIUS);
    velocity = vector3d(3500.0, 0.0, 0.0);
    orientation = vector3d(0.0, 0.0, 90.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = false;
    autopilot_enabled = false;
    break;

  case 3:
    // polar surface launch at escape velocity (but drag prevents escape)
    position = vector3d(0.0, 0.0, MARS_RADIUS + LANDER_SIZE/2.0);
    velocity = vector3d(0.0, 0.0, 5027.0);
    orientation = vector3d(0.0, 0.0, 0.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = false;
    autopilot_enabled = false;
    break;

  case 4:
    // an elliptical orbit that clips the atmosphere each time round, losing energy
    position = vector3d(0.0, 0.0, MARS_RADIUS + 100000.0);
    velocity = vector3d(4000.0, 0.0, 0.0);
    orientation = vector3d(0.0, 90.0, 0.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = false;
    autopilot_enabled = false;
    break;

  case 5:
    // a descent from rest at the edge of the exosphere
    position = vector3d(0.0, -(MARS_RADIUS + EXOSPHERE), 0.0);
    velocity = vector3d(0.0, 0.0, 0.0);
    orientation = vector3d(0.0, 0.0, 90.0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = true;
    autopilot_enabled = true;
    save_data = false;
    break;

  case 6:
    // a descent from rest at the edge of the exosphere
    position = vector3d(0.0, 0.0, -20428000);
    velocity = vector3d(1447.992029, 0.0, 0.0);
    orientation = vector3d(0.0, 0.0, 0);
    delta_t = 0.1;
    parachute_status = NOT_DEPLOYED;
    stabilized_attitude = false;
    autopilot_enabled = false;
    break;

  case 7:
    // examples paper 1 q3 scenario
    position = vector3d(0.0, 0.0, MARS_RADIUS + 500);
    velocity = vector3d(0.0, 0.0, 0.0);
    autopilot_enabled = true;
    stabilized_attitude = true;
    break;

  case 8:
    break;

  case 9:
    break;

  }
}
