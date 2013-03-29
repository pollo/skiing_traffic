/*
 * File:   physicalforce.h
 *
 * The class Physicalforce
 */

#ifndef PHYSICALFORCE_H
#define PHYSICALFORCE_H

#include "vector.h"
#include "skier.h"

class PhysicalForce {
public:
  virtual ~PhysicalForce() {};

  //returns the force Vector that this force applies on the skier
  virtual Vector apply(const Skier& skier) = 0;
};

class DownhillForce : public PhysicalForce {
public:
  Vector apply(const Skier& skier);

  ~DownhillForce() { }
private:
};

#endif /* PHYSICALFORCE_H */
