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
                            string elevation_name,
                            string slope_name)
{
    string mapset;
    //open ski slope map
    if ((mapset = G_find_vector2(ski_slope_name, "")) == NULL)
        throw domain_error("Vector map "+ski_slope_name+" not found");
    if (Vect_open_old(&ski_slope, ski_slope_name, mapset) == -1)
        throw domain_error("Unable to open vector map <%s>", "slope");

    //open elevation map

    //open slope map
}

GrassBackend::GrassBackend (const GrassBackend& orig) { }

GrassBackend::~GrassBackend () { }

