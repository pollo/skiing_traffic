/*
 * File:   GrassBackend.h
 *
 * Declares the interface for the Grass backend that can be use to perform
 * the simulation.
 * Three map are used:
 * -ski_slope: vectorial map with the geometry of the ski slope
 * -elevation: raster map with the elevation data
 * -slope: raster map with the slope data
 */

#ifndef GRASSBACKEND_H
#define GRASSBACKEND_H

#ifdef __cplusplus
extern "C" {
#endif
#include <grass/gis.h>
#include <grass/Vect.h>
#ifdef __cplusplus
}
#endif

#include <string>
#include "gisbackend.h"

class GrassBackend : public GisBackend {
public:
  GrassBackend(std::string ski_slope_name,
               std::string right_edge,
               std::string left_edge,
               std::string start_area_name,
               std::string elevation_name,
               std::string slope_name,
               std::string aspect_name);
  ~GrassBackend();

  //returns the elevation value at the coordinates x y
  double get_elevation(double x, double y) const;

  //returns the slope value at the coordinates x y
  double get_slope(double x, double y) const;

  //returns the aspect value at the coordinates x y
  double get_aspect(double x, double y) const;

  //returns the distance between x y and the left slope edge
  double distance_from_left(double x, double y) const;

  //returns the distance between x y and the right slope edge
  double distance_from_right(double x, double y) const;

  //checks if the give point is inside the ski slope
  bool is_inside_slope(double x, double y) const;

  //returns the cell center coordinates
  void get_cell_center(double x, double y,
                       double *cx, double *cy) const;

  //return the coordinate of a random point inside the start area
  void get_start_point(double *x, double *y) const;

 private:
  Map_info ski_slope;
  Map_info right_edge;
  Map_info left_edge;
  Map_info start_area;
  //file descriptors
  int elevation;
  RASTER_MAP_TYPE elevation_type;
  int slope;
  RASTER_MAP_TYPE slope_type;
  int aspect;
  RASTER_MAP_TYPE aspect_type;
  //structures supporting distance computation
  line_pnts *line_right;
  line_pnts *line_left;

  bool open_vector(Map_info *map, const std::string name) const;
  bool open_raster(int *map,
                   RASTER_MAP_TYPE *data_type,
                   const std::string name) const;
};

#endif /* GRASSBACKEND_H */
