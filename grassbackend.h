/*
 *
 * Declares the interface for the Grass backend that can be use to perform
 * the simulation. It implements the GisBackend interface.
 *
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
#include <map>
#include <vector>
#include <utility>
#include "gisbackend.h"

class GrassBackend : public GisBackend {
public:
  GrassBackend(
    //vector map with slope polygon
    std::string ski_slope_name,
    //vector map with right edge line
    std::string right_edge,
    //vector map with left edge line
    std::string left_edge,
    //vector map with start area polygon
    std::string start_area_name,
    //vector map with stop area polygon
    std::string stop_area_name,
    //raster elevation map
    std::string elevation_name,
    //raster slope map
    std::string slope_name,
    //raster aspect map
    std::string aspect_name);
  ~GrassBackend();

  //returns the elevation value at the coordinates x y
  double get_elevation(double x, double y) const;

  //returns the slope value at the coordinates x y
  double get_slope(double x, double y) const;

  //returns the aspect value at the coordinates x y
  double get_aspect(double x, double y) const;

  //returns the distance between x y and the left slope edge
  //also the intersection point can be reported
  double distance_from_left(double x, double y,
                            double *px, double *py) const;

  //returns the distance between x y and the right slope edge
  //also the intersection point can be reported
  double distance_from_right(double x, double y,
                             double *px, double *py) const;

  //checks if the give point is inside the ski slope
  bool is_inside_slope(double x, double y) const;

  //returns the cell center coordinates
  void get_cell_center(double x, double y,
                       double *cx, double *cy) const;

  //returns the coordinate of a random point inside the start area
  void get_start_point(double *x, double *y) const;

  //returns the bound of the cell for the specified point
  void get_cell_bound(double x, double y,
                      double *east, double *west,
                      double *north, double *sud) const;

  //Returns the intersection point of the line (sx,sy)(dx,dy) with the slope
  //edge and the angle that it forms with the perpendicular to the edge
  bool reflect_line(double sx, double sy, double dx, double dy,
                    double *px, double *py, double *angle) const;

  //returns true if the point is inside the stop area
  bool is_inside_stop_area(double x, double y) const;

 private:
  Map_info ski_slope;
  Map_info right_edge;
  Map_info left_edge;
  Map_info start_area;
  Map_info stop_area;
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
  //cache raster map result
  mutable std::vector< std::map< std::pair<int,int>, double> > rast_cache;

  bool open_vector(Map_info *map, const std::string name) const;
  bool open_raster(int *map,
                   RASTER_MAP_TYPE *data_type,
                   const std::string name) const;
  double query_raster(int map, RASTER_MAP_TYPE type, double x, double y) const;
};

#endif /* GRASSBACKEND_H */
