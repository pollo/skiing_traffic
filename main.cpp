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
#include <iostream>
#include "grassbackend.h"

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


int main (int argc, char** argv)
{
  GModule *module;
  Option *ski_slope_opt, *right_edge_opt, *left_edge_opt;
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

  //raster map options declaration
  dtm_opt = decl_rast("elevation", "Name of elevation raster map");
  slope_opt = decl_rast("slope","Name of slope raster map");
  aspect_opt = decl_rast("aspect","Name of aspect raster map");

  if (G_parser(argc, argv))
    exit(1);

  GrassBackend gb(ski_slope_opt->answer,
                  right_edge_opt->answer,
                  left_edge_opt->answer,
                  dtm_opt->answer,
                  slope_opt->answer,
                  aspect_opt->answer);

  cout << "x y elevation slope aspect left right inside" << endl;
  for (int i=0; i<500; i++)
  {
    double x,y,f,fmax,fmin;
    f = (double)rand() / RAND_MAX;
    fmin = 695512.73;
    fmax = 696567.64;
    x = fmin + f * (fmax - fmin);
    f = (double)rand() / RAND_MAX;
    fmin = 5134497.66;
    fmax = 5135308.53;
    y = fmin + f * (fmax - fmin);
    cout.precision(10);
    cout << x << " " << y << " ";
    cout << gb.get_elevation(x,y) << " ";
    cout <<  gb.get_slope(x,y) << " ";
    cout  <<  gb.get_aspect(x,y) << " ";
    cout << gb.distance_from_left(x,y);
    cout << " " <<  gb.distance_from_right(x,y) << " ";
    cout <<  gb.is_inside_slope(x,y) << " ";
    cout << endl;
  }
  return 0;
}
