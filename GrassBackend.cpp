/*
 * File:   GrassBackend.cpp
 *
 * Defines the Grass Backend implementation.
 * Core Grass libraries are used to handle spatial data and performs spatial
 * queries.
 *
 */

#include "GrassBackend.h"
#include <stdexcept>

using namespace std;

//Opens the ski slope, elevation and slope maps.
//if an error occurs throws a domain_error exception
GrassBackend::GrassBackend (string ski_slope_name,
                            string right_edge_name,
                            string left_edge_name,
                            string elevation_name,
                            string slope_name)
{
    string mapset;
    //open ski slope map
    if (!open_vector(ski_slope,ski_slope_name))
        throw domain_error("Ski slope map "+elevation_name+" not found");
    //open right edge
    if (!open_vector(right_edge,right_edge_name))
        throw domain_error("Ski slope right edge map "+right_edge_name+" not found");
    //open left edge
    if (!open_vector(left_edge,left_edge_name))
        throw domain_error("Ski slope left edge map "+left_edge_name+" not found");
    //open elevation map
    mapset = G_find_cell2(elevation_name, "");
    if (mapset == NULL)
        throw domain_error("Raster map "+elevation_name+" not found");

    //determine the inputmap type (CELL/FCELL/DCELL)
    RASTER_MAP_TYPE data_type = G_raster_map_type(elevation_name, mapset);
    if ((elevation = G_open_cell_old(elevation_name, mapset)) < 0)
	throw domain_error("Unable to open raster map "+elevation_name);

    //open slope map
    mapset = G_find_cell2(slope_name, "");
    if (mapset == NULL)
        throw domain_error("Raster map "+slope_name+" not found");

    //determine the inputmap type (CELL/FCELL/DCELL)
    RASTER_MAP_TYPE data_type = G_raster_map_type(slope_name, mapset);
    if ((slope = G_open_cell_old(slope_name, mapset)) < 0)
	throw domain_error("Unable to open raster map "+slope_name);

}

bool GrassBackend::open_vector(Map_info& map, std::string name)
{
    string mapset;
    if ((mapset = G_find_vector2(name, "")) == NULL)
        return false;
    if (Vect_open_old(&map, name, mapset) == -1)
        return false;
    return true;
}

bool GrassBackend::open_raster(int& map, std::string name)
{
    string mapset;
    mapset = G_find_cell2(name, "");
    if (mapset == NULL)
        throw domain_error("Raster map "+name+" not found");

    //determine the inputmap type (CELL/FCELL/DCELL)
    RASTER_MAP_TYPE data_type = G_raster_map_type(name, mapset);
    if ((elevation = G_open_cell_old(name, mapset)) < 0)
	throw domain_error("Unable to open raster map "+name);

    //open slope map
    mapset = G_find_cell2(slope_name, "");
    if (mapset == NULL)
        throw domain_error("Raster map "+slope_name+" not found");

    //determine the inputmap type (CELL/FCELL/DCELL)
    RASTER_MAP_TYPE data_type = G_raster_map_type(slope_name, mapset);
    if ((slope = G_open_cell_old(slope_name, mapset)) < 0)
	throw domain_error("Unable to open raster map "+slope_name);
    

}


GrassBackend::~GrassBackend () { }

