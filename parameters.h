/*
 *
 * Declares the parameters for the simulation
 * Comments on .cpp file
 *
 */


#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>

struct settings {
  static const double g;
  static const double average_mass;
  static const int persons_hour;
  static const int total_simulation_time;
  static const double degree_to_radians;
  static const double air_drag_coefficient;
  static const double frontal_area;
  static const double air_density;
  static const double sidecut_radius;
  static const double kinetic_friction_coefficient;
  static const double range_repulsion_right;
  static const double range_repulsion_left;
  static const double range_repulsion_skier;
  static const double edge_repulsion_strength;
  static const double skier_repulsion_strength;
  static const double directional_deviation;
  static const double angle_view;
  static const double stop_turning;
  static const double space_between_waypoints;
  static const double distance_waypoint;
  static const double waypoint_force_strength;
  static const double dtime;
  static const double limit_edge_distance;
  static const int log_freq;
  static const double initial_vel;
  static const double small_slope;
  static const double distance_to_check_slope;
  static const double vel_high;
  static const double slow_simulation;
  static const double skidding_factor;
  static const char* output_file_path;
};

#endif /* PARAMETERS_H */
