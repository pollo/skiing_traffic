/*
 * File:   GisBackend.h
 *
 * Declares an Interface for the Gis Backend needed by the model.
 *
 */

#ifndef GISBACKEND_H
#define	GISBACKEND_H

class GisBackend {
public:
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
  bool is_inside_slope(double x, double y);

  virtual ~GisBackend() { };
private:

};

#endif	/* GISBACKEND_H */
