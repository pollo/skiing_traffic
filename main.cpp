/*
 * File:   main.cpp
 * Author: pollo
 *
 * Created on March 2, 2013, 12:08 PM
 */

#ifdef __cplusplus
extern "C" {
#endif
#include <grass/gis.h>
#ifdef __cplusplus
}
#endif
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "grassbackend.h"
#include "slope.h"

using namespace std;

Option* decl_vect(const char* key,const char* description)
{
  Option *opt;

  opt = G_define_standard_option(G_OPT_V_INPUT);
  opt->key = key;
  opt->description = description;

  return opt;
}

Option* decl_rast(const char* key,const char* description)
{
  Option *opt;

  opt = G_define_standard_option(G_OPT_R_INPUT);
  opt->key = key;
  opt->description = description;

  return opt;
}

void test_gb(Slope& sl)
{
  cout << "x y elevation slope aspect left right inside center" << endl;
  for (int i=0; i<500; i++)
  {
    double f,fmax,fmin;
    Point p;
    Point c;
    f = (double)rand() / RAND_MAX;
    fmin = 695512.73;
    fmax = 696567.64;
    p.x = fmin + f * (fmax - fmin);
    f = (double)rand() / RAND_MAX;
    fmin = 5134497.66;
    fmax = 5135308.53;
    p.y = fmin + f * (fmax - fmin);
    cout.precision(10);
    cout << p.x << " " << p.y << " ";
    cout << sl.get_elevation(p) << " ";
    cout <<  sl.get_slope(p) << " ";
    cout  <<  sl.get_aspect(p) << " ";
    cout << sl.distance_from_left(p);
    cout << " " <<  sl.distance_from_right(p) << " ";
    cout <<  sl.is_inside_slope(p) << " ";
//    sl.get_cell_center(p,&p);
//    cout << "(" << p.x << "," << p.y << "," << p.z << ") ";
    cout << endl;
  }
}

void test_elevation(Slope& sl)
{
  cout << "id x y elevation" << endl;
  for (int i=0; i<10000; i++)
  {
    double f,fmax,fmin;
    Point p;
    Point c;
    f = (double)rand() / RAND_MAX;
    fmin = 695562.73;
    fmax = 695567.64;
    //vero
    //fmin = 695512.73;
    //fmax = 696567.64;
    //finto
    //fmin = 678550.020811;
    //fmax = 678559.989453;
    p.x = fmin + f * (fmax - fmin);
    f = (double)rand() / RAND_MAX;
    //vero
    fmin = 5135100.66;
    fmax = 5135105.53;
    //fmin = 5134497.66;
    //fmax = 5135308.53;
    //finto
    //fmin = 5101689.983547;
    //fmax = 5101700.014884;
    p.y = fmin + f * (fmax - fmin);
    cout.precision(10);
    cerr << "i " <<i << endl;
    cout << i << " " << p.x << " " << p.y << " ";
    cout << sl.get_elevation(p) << " " << endl;
  }
}

int main (int argc, char** argv)
{
  GModule *module;
  Option *ski_slope_opt, *right_edge_opt, *left_edge_opt, *start_area_opt;
  Option *dtm_opt, *slope_opt, *aspect_opt;

  G_gisinit(argv[0]);
  module = G_define_module();

  //initialize module
  module->keywords = "ski, traffic";
  module->description = "Simulate skiers on a slope";

  //vector map options declaration
  ski_slope_opt = decl_vect("ski_slope",
                            "Name of input vector map of the ski slope");
  right_edge_opt = decl_vect(
    "right_edge",
    "Name of input vector map of the ski slope right edge");
  left_edge_opt = decl_vect(
    "left_edge",
    "Name of input vector map of the ski slope left edge");
  start_area_opt = decl_vect(
    "start_area",
    "Name of input vector map of the area where the skiers start");

  //raster map options declaration
  dtm_opt = decl_rast("elevation", "Name of elevation raster map");
  slope_opt = decl_rast("slope","Name of slope raster map");
  aspect_opt = decl_rast("aspect","Name of aspect raster map");

  if (G_parser(argc, argv))
    exit(1);

  GrassBackend *gb;
  try {
    gb = new GrassBackend(ski_slope_opt->answer,
                          right_edge_opt->answer,
                          left_edge_opt->answer,
                          start_area_opt->answer,
                          dtm_opt->answer,
                          slope_opt->answer,
                          aspect_opt->answer);
  }
  catch (domain_error e) {
    cout << "ERROR: " << e.what()  << endl;
    exit(EXIT_FAILURE);
  }
  Slope sl(*gb);
  //test_gb(sl);
  //test_elevation(sl);
  /*GrassBackend *pgb;
  for (int i=0; i<112; i++)
  {
    cout << i << endl;
    pgb = new GrassBackend(ski_slope_opt->answer,
                  right_edge_opt->answer,
                  left_edge_opt->answer,
                  dtm_opt->answer,
                  slope_opt->answer,
                  aspect_opt->answer);
    delete pgb;
    for (int j=0; j<10000; j+=2)
      j-=1;
      }*/
  return 0;
}
