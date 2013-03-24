/*
 * File:   GrassBackend.cpp
 *
 * Defines the Grass Backend implementation.
 * Core Grass libraries are used to handle spatial data and performs spatial
 * queries.
 *
 */

#include "grassbackend.h"
#include <stdexcept>
#include <iostream>
#include <limits>

using namespace std;

//Reads a line from the vector map
//If more then a line throws a domain_error exception
line_pnts* read_vector_line(Map_info& map)
{
  int type;
  line_pnts* line;

  line = Vect_new_line_struct();
  do {
    type = Vect_read_next_line(&map, line, NULL); //cat not needed
  } while (type>0 && type!=GV_LINE);
  if (!type>0)
    throw runtime_error("Cannot find a line in the edge map");
  return line;
}

//Opens the ski slope, elevation and slope maps.
//if an error occurs throws a domain_error exception
GrassBackend::GrassBackend (string ski_slope_name,
                            string right_edge_name,
                            string left_edge_name,
                            string start_area_name,
                            string elevation_name,
                            string slope_name,
                            string aspect_name)
{
  string mapset;
  //open ski slope map
  if (!open_vector(&ski_slope,ski_slope_name))
    throw domain_error("Ski slope map "+elevation_name+" not found");
  //check ski slope map
  if (Vect_get_num_areas(&ski_slope) != 1)
    throw domain_error("Ski slope map "+elevation_name+\
                       " must have a single feature of type AREA");
  //open start area map
  if (!open_vector(&start_area,start_area_name))
    throw domain_error("Start area map "+start_area_name+" not found");
  //check ski slope map
  if (Vect_get_num_areas(&start_area) != 1)
    throw domain_error("Start area map "+start_area_name+\
                       " must have a single feature of type AREA");
  //open right edge
  if (!open_vector(&right_edge,right_edge_name))
    throw domain_error("Slope right edge map "+right_edge_name+" not found");
  //check right edge
  if (Vect_get_num_lines(&right_edge) != 1)
    throw domain_error("Slope right edge map "+right_edge_name+\
                       " must have a single feature of type LINE");
  line_right = read_vector_line(right_edge);
  //open left edge
  if (!open_vector(&left_edge,left_edge_name))
    throw domain_error("Slope left edge map "+left_edge_name+" not found");
  //check left edge
  if (Vect_get_num_lines(&left_edge) != 1)
    throw domain_error("Slope left edge map "+left_edge_name+\
                       " must have a single feature of type LINE");
  line_left = read_vector_line(left_edge);

  //open elevation map
  if (!open_raster(&elevation,&elevation_type,elevation_name))
    throw domain_error("Elevation map "+elevation_name+" not found");
  //open slope map
  if (!open_raster(&slope,&slope_type,slope_name))
    throw domain_error("Slope map "+slope_name+" not found");
  //open aspect map
  if (!open_raster(&aspect,&aspect_type,aspect_name))
    throw domain_error("Aspect map "+aspect_name+" not found");
}

//opens the vector map name
//returns the Map_info structure resulting
bool GrassBackend::open_vector(Map_info *map, std::string name) const
{
  char* mapset;
  if ((mapset = G_find_vector2(name.c_str(), "")) == NULL)
    return false;
  if (Vect_open_old(map, name.c_str(), mapset) == -1)
    return false;
  return true;
}

//opens the raster map name
//returns the file descriptor and optionally the data_type
bool GrassBackend::open_raster(int *map,
                               RASTER_MAP_TYPE *data_type,
                               std::string name) const
{
  char* mapset;
  mapset = G_find_cell2(name.c_str(), "");
  if (mapset == NULL)
    return false;

  //determine the inputmap type (CELL/FCELL/DCELL)
  if (data_type)
    (*data_type) = G_raster_map_type(name.c_str(), mapset);
  *map = G_open_cell_old(name.c_str(), mapset);
  if (*map < 0)
    return false;
  return true;
}

double query_raster(int map, RASTER_MAP_TYPE type, double x, double y)
{
  int row, col;
  double res;
  Cell_head window;

  G_get_window(&window);
  row = (int) G_northing_to_row(y, &window);
  col = (int) G_easting_to_col(x, &window);

  if (type == CELL_TYPE)
  {
    //TODO: avoid memory allocation each time
    CELL *cell = G_allocate_c_raster_buf();
    if (G_get_c_raster_row(map, cell, row) < 0)
      return numeric_limits<double>::quiet_NaN();
    res = cell[col];
    G_free(cell);
  }
  else
  {
    //TODO: avoid memory allocation each time
    DCELL *cell = G_allocate_d_raster_buf();

    if (G_get_d_raster_row(map, cell, row) < 0)
      return numeric_limits<double>::quiet_NaN();
    res = cell[col];
    G_free(cell);
  }
  return res;
}

double GrassBackend::get_elevation(double x, double y) const
{
  return query_raster(elevation,elevation_type,x,y);
}

double GrassBackend::get_slope(double x, double y) const
{
  return query_raster(slope,slope_type,x,y);
}

double GrassBackend::get_aspect(double x, double y) const
{
  return query_raster(aspect,aspect_type,x,y);
}

bool GrassBackend::is_inside_slope(double x, double y) const
{
  Map_info temp;
  //GRASS 6 does not declare the map parameter const
  //this trick is to mantain this method const
  temp = ski_slope;
  return (Vect_point_in_area_outer_ring(x,y,&temp,1) == 1);
}

//Assumes that there is unique feature in the map: the line rappresenting
//the left edge. Computes the distance from this line
double GrassBackend::distance_from_left(double x, double y) const
{
  double dist;
  Vect_line_distance(line_left,
                     x,
                     y,
                     0,      //z
                     0,      //with_z FALSE
                     NULL,   //point_on_line x
                     NULL,   //point_on_line y
                     NULL,   //point_on_line z
                     &dist,  //distance
                     NULL,   //distance from segment
                     NULL);  //distance from line along line

  return dist;
}

//Assumes that there is unique feature in the map: the line rappresenting
//the right edge. Computes the distance from this line
double GrassBackend::distance_from_right(double x, double y) const
{
  double dist;

  Vect_line_distance(line_right,
                     x,
                     y,
                     0,      //z
                     0,      //with_z FALSE
                     NULL,   //point_on_line x
                     NULL,   //point_on_line y
                     NULL,   //point_on_line z
                     &dist,  //distance
                     NULL,   //distance from segment
                     NULL);  //distance from line along line

  return dist;
}

//returns the cell center coordinates for the cell where the point x y is
//coordinates are stored in cx and cy
void GrassBackend::get_cell_center(double x, double y,
                       double *cx, double *cy) const
{
  Cell_head window;
  double row, col;

  G_get_window(&window);
  col = (int) G_easting_to_col(x, &window);
  row = (int) G_northing_to_row(y, &window);
  *cx = G_col_to_easting(col+0.5, &window);
  *cy = G_row_to_northing(row+0.5, &window);
}

double frand(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

//return the coordinate of a random point inside the start area
void GrassBackend::get_start_point(double *x, double *y) const
{
  BOUND_BOX box;
  Map_info temp;

  //GRASS 6 does not declare the map parameter const
  //this trick is to mantain this method const
  temp = start_area;
  Vect_get_area_box(&temp, 1, &box);
  do {
    *x = frand(box.W,box.E);
    *y = frand(box.S,box.N);
  } while (!Vect_point_in_area_outer_ring(*x,*y,&temp,1));
}

GrassBackend::~GrassBackend () {
  Vect_close(&ski_slope);
  Vect_close(&right_edge);
  Vect_close(&left_edge);
  if (line_right)
    Vect_destroy_line_struct(line_right);
  if(line_left)
    Vect_destroy_line_struct(line_left);
  G_close_cell(elevation);
  G_close_cell(slope);
  G_close_cell(aspect);
}
