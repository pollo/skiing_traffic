/*
 *
 * Main file, handle command line parameters and runs the simulation
 *
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
#include <fstream>
#include "grassbackend.h"
#include "slope.h"
#include "parameters.h"

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

void run_simulation(Slope& sl)
{
  double dtime = settings::dtime;
  for (double t=0; t<settings::total_simulation_time; t+=dtime)
  {
    sl.update(dtime);
  }
}

int main (int argc, char** argv)
{
  GModule *module;
  Option *ski_slope_opt, *right_edge_opt, *left_edge_opt, *start_area_opt;
  Option *stop_area_opt, *dtm_opt, *slope_opt, *aspect_opt;

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
  stop_area_opt = decl_vect(
    "stop_area",
    "Name of input vector map of the area where the skiers stop");

  //raster map options declaration
  dtm_opt = decl_rast("elevation", "Name of elevation raster map");
  slope_opt = decl_rast("slope","Name of slope raster map");
  aspect_opt = decl_rast("aspect","Name of aspect raster map");

  if (G_parser(argc, argv))
    exit(1);

  //instantiate GrassBackend
  GrassBackend *gb;
  try {
    gb = new GrassBackend(ski_slope_opt->answer,
                          right_edge_opt->answer,
                          left_edge_opt->answer,
                          start_area_opt->answer,
                          stop_area_opt->answer,
                          dtm_opt->answer,
                          slope_opt->answer,
                          aspect_opt->answer);
  }
  catch (domain_error e) {
    cout << "ERROR: " << e.what()  << endl;
    exit(EXIT_FAILURE);
  }

  //open output file
  ofstream output;
  output.open(settings::output_file_path);

  //instantiate Slope class
  Slope sl(*gb, output);

  run_simulation(sl);

  output.close();
  return 0;
}
