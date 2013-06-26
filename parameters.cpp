/*
 *
 * Model paramaters values
 *
 */

#include "parameters.h"
#include <cmath>

//PHYSICAL FORCES PARAMETERS
//\rho_{air}
const double settings::air_density = 1.3163;
//m
const double settings::average_mass = 85.0;
//g
const double settings::g = 9.81;
//R_{SC}
const double settings::sidecut_radius = 10.0;
//\mu
const double settings::kinetic_friction_coefficient = 0.1;
//C_d
const double settings::air_drag_coefficient = 1.0;
//A
const double settings::frontal_area = 0.6;
//\eta
const double settings::skidding_factor = 0.2;

//SOCIAL FORCES PARAMETERS
//A_0
const double settings::waypoint_force_strength = 1;
//U_0
const double settings::edge_repulsion_strength = 10.0;
//V_0
const double settings::skier_repulsion_strength = 8.0;
//R_R
const double settings::range_repulsion_right = 10.0;
//R_L
const double settings::range_repulsion_left = 10.0;
//R_A
const double settings::range_repulsion_skier = 8.0;
//2\varphi = 180 degree
const double settings::angle_view = 180.0*settings::degree_to_radians;
//\delta = 10 degree
const double settings::directional_deviation = 10.0*settings::degree_to_radians;
//D (after D meters a new waypoint is chosen)
const double settings::space_between_waypoints = 15.0;
//s_{lim} (if the slope at waypoint_distance is less than this value the skier
//tends to choose waypoints on a straight line to increase velocity)
const double settings::small_slope = 15*settings::degree_to_radians;
const double settings::distance_to_check_slope = 50;
//v_{high}
const double settings::vel_high = 25;
//distance between the skier position and the waypoint that should be choosen
const double settings::distance_waypoint = 30.0;

//SLOPE RELATED PARAMETERS
//\lambda
const int settings::persons_hour = 300;
const int settings::total_simulation_time = 1 * 60; //seconds
//distance under which narrow the angle where waypoints should be choosen
const double settings::limit_edge_distance = settings::sidecut_radius; //meters
const double settings::initial_vel = 1;

//IMPLEMENTATION RELATED PARAMETERS
//whean angle between desired direction and actual direction less than
//stop_turning -> stop turning
const double settings::stop_turning = 2.0*settings::degree_to_radians;
const double settings::degree_to_radians = M_PI/180.0;
//integration time step
const double settings::dtime = 0.01; //seconds
//log one point after
const int settings::log_freq = 10;
//slow computation to view simulation in qgis
const double settings::slow_simulation = 5;
const char* settings::output_file_path = "/tmp/simulation.csv";
