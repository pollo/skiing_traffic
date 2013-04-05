/*
 * File:   gisbackend.h
 *
 * Declares an Interface for the Gis Backend needed by the model.
 *
 */

#ifndef GISBACKEND_H
#define	GISBACKEND_H

class GisBackend {
public:
  //returns the elevation value at the coordinates x y
  virtual double get_elevation(double x, double y) const = 0;

  //returns the slope value at the coordinates x y
  virtual double get_slope(double x, double y) const = 0;

  //returns the aspect value at the coordinates x y
  virtual double get_aspect(double x, double y) const = 0;

  //returns the distance between x y and the left slope edg
  //also the intersection point can be reported
  virtual double distance_from_left(double x, double y,
                                    double *px, double *py) const = 0;

  //returns the distance between x y and the right slope edge
  //also the intersection point can be reported
  virtual double distance_from_right(double x, double y,
                                     double *px, double *py) const = 0;

  //returns the cell center coordinates
  virtual void get_cell_center(double x, double y,
                               double *cx, double *cy) const = 0;

  virtual void get_cell_bound(double x, double y,
                              double *east, double *west,
                              double *north, double *sud) const = 0;

  //checks if the give point is inside the ski slope
  virtual bool is_inside_slope(double x, double y) const = 0;

  //returns the coordinate of a random point inside the start area
  virtual void get_start_point(double *x, double *y) const = 0;

  //Returns the intersection point of the line (sx,sy)(dx,dy) with the slope
  //edge and the angle that it forms with the perpendicular to the edge
  virtual bool reflect_line(double sx, double sy, double dx, double dy,
                            double *px, double *py, double *angle) const = 0;

  //returns true if the point is inside the stop area
  virtual bool is_inside_stop_area(double x, double y) const = 0;

  virtual ~GisBackend() { };
private:

};

#endif	/* GISBACKEND_H */
