/*
 * File:   skier.h
 *
 * The class Skier
 */

#ifndef SKIER_H
#define SKIER_H

#include "point.h"

class Skier {
public:
  void update(double dtime);

private:
  Point position;
};

#endif /* SKIER_H */
