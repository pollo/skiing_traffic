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
#define	GRASSBACKEND_H

#ifdef __cplusplus
extern "C" {
#endif
#include <grass/gis.h>
#include <grass/Vect.h>
#ifdef __cplusplus
}
#endif

#include <string>
#include "GisBackend.h"

class GrassBackend : public GisBackend {
public:
    GrassBackend(std::string ski_slope_name,
                 std::string right_edge,
                 std::string left_edge,
                 std::string elevation_name,
                 std::string slope_name);
    ~GrassBackend();

private:
    Map_info ski_slope;
    Map_info right_edge;
    Map_info left_edge;
    //file descriptors
    int elevation;
    int slope;

    bool open_vector(Map_info& map, std::string name);
    bool open_raster(int& map, std::string name);
};

#endif	/* GRASSBACKEND_H */

