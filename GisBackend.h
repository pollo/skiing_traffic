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
    virtual double get_elevation(double x, double y) const = 0;

    //returns the slope value at the coordinates x y
    virtual double get_slope(double x, double y) const = 0;

    //returns the distance between x y and the left slope edge
    virtual double distance_from_left(double x, double y) const = 0;

    //returns the distance between x y and the right slope edge
    virtual double distance_from_right(double x, double y) const = 0;

    virtual ~GisBackend();
private:

};

#endif	/* GISBACKEND_H */
