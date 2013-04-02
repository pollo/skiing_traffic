#include "parameters.h"

#include <cmath>

const double settings::g = 9.81;
const double settings::average_mass = 85.0;
const int settings::persons_hour = 1;
const double settings::degree_to_radians = M_PI/180.0;
const double settings::air_drag_coefficient = 1.0;
const double settings::frontal_area = 0.6;
const double settings::air_density = 1.3163;
const double settings::sidecut_radius = 10.0;
const double settings::kinetic_friction_coefficient = 0.1;
const double settings::range_repulsion_right = 15.0;
const double settings::range_repulsion_left = 15.0;
const double settings::range_repulsion_skier = 2.0;
const double settings::edge_repulsion_strength = 100.0;
const double settings::skier_repulsion_strength = 100.0;
const double settings::waypoint_force_strength = 1.0;
//10 degree
const double settings::directional_deviation = 10.0*settings::degree_to_radians;
//180 degree
const double settings::angle_view = 180.0*settings::degree_to_radians;
const double settings::stop_turning = 2.0*settings::degree_to_radians;
const double settings::time_between_waypoints = 5.0;
//distance between the skier position and the waypoint that should be choosen
const double settings::distance_waypoint = 20.0;
//angle that is subtracted from the total angle that can be considered choosing
//the new waypoint
const double settings::limit_angle_waypoint = 1.0/4.0;
