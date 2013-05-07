/*
 * File:   parameters.h
 *
 * Declares the parameters for the simulation
 *
 */


#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>
#ifdef DEBUG
#include <fstream>
#endif

struct settings {
  static const double g;
  static const double average_mass;
  static const int persons_hour;
  static const int total_simulation_time; //seconds
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
  static const double space_between_waypoints;
  //distance between the skier position and the waypoint that should be choosen
  static const double distance_waypoint;
  static const double waypoint_force_strength;
  static const double limit_angle_waypoint;
  static const double dtime;
  static const double limit_edge_distance;
  static const int log_freq;
  static const double initial_vel;

#ifdef DEBUG
  static std::ofstream& rep() {
    static std::ofstream output;
    static bool t = true;
    if (t)
    {
      t = false;
      output.open("/tmp/repulsion.csv");
      output << "x y force norm " << std::endl;
      output.precision(13);
    }
    return output;
  }

  static std::ofstream& right() {
    static std::ofstream output;
    static bool t = true;
    if (t)
    {
      t = false;
      output.open("/tmp/right.csv");
      output << "x y force norm " << std::endl;
      output.precision(13);
    }
    return output;
  }

  static std::ofstream& left() {
    static std::ofstream output;
    static bool t = true;
    if (t)
    {
      t = false;
      output.open("/tmp/left.csv");
      output << "x y force norm ad ae " << std::endl;
      output.precision(13);
    }
    return output;
  }

  static std::ofstream& waypoints() {
    static std::ofstream output;
    static bool t = true;
    static int id = 1;
    static int c = 0;
    if (t)
    {
      t = false;
      output.open("/tmp/waypoints.csv");
      output << "id x y " << std::endl;
      output.precision(13);
    }
    output << id << " ";
    if (c==4)
    {
      id ++;
    }
    c = (c+1)%5;
    return output;
  }
#endif
};

#endif /* PARAMETERS_H */
