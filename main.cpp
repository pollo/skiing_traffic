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

using namespace std;

int main (int argc, char** argv)
{
  GModule *module;
  Option *ski_slope_opt, *dtm_opt, *slope_opt;

  G_gisinit(argv[0]);
  module = G_define_module();

  module->keywords = "ski, traffic";
  module->description = "Simulate skiers on a slope";

  ski_slope_opt = G_define_standard_option(G_OPT_V_INPUT);
  ski_slope_opt->key = "ski_slope";
  ski_slope_opt->description = "Name of input vector map of the ski slope";

  dtm_opt = G_define_standard_option(G_OPT_V_INPUT);
  dtm_opt->key = "elevation";
  dtm_opt->description = "Name of elevation raster map";

  slope_opt = G_define_standard_option(G_OPT_V_INPUT);
  slope_opt->key = "slope";
  slope_opt->description = "Name of slope raster map";

  if (G_parser(argc, argv))
    exit(EXIT_FAILURE);

  cout << "Ski slope: " << ski_slope_opt->answer << endl;
  cout << "Elevation map: " << dtm_opt->answer << endl;
  cout << "Slope map: " << slope_opt->answer << endl;

  free(ski_slope_opt);
  free(dtm_opt);
  free(slope_opt);
  return 0;
}

