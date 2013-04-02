/*
 * File:   parameters.h
 *
 * Declares the parameters for the simulation
 *
 */


#ifndef PARAMETERS_H
#define PARAMETERS_H

struct settings {
  static const double g;
  static const double average_mass;
  static const int persons_hour;
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
  static const double directional_deviation; //degree
  static const double angle_view; //degree
  static const double stop_turning;
  static const double time_between_waypoints;
  //distance between the skier position and the waypoint that should be choosen
  static const double distance_waypoint;
  static const double waypoint_force_strength;
  static const double limit_angle_waypoint;
};

#endif /* PARAMETERS_H */
