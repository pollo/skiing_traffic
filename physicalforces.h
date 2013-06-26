/*
 *
 * The class Physicalforce is the abstract base class for all those
 * phsicalforces that need to be instantiated inside the simulation.
 * All those forces should have the method apply wich returns the
 * Vector force resultat by applying the force to the skier.
 * The forces that do not need to be instantiated inside the simulation
 * but are used only to compute the other forces are modelled by functions
 * that take a skier as input and returns the Vector force applied to the
 * skier.
 *
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
  Vector apply_without_skidding(const Skier& skier);

  ~DownhillForce() { }
};

class CentripetalForce : public PhysicalForce {
public:
  Vector apply(const Skier& skier);

  ~CentripetalForce() { }
};


class AirDragForce : public PhysicalForce {
public:
  Vector apply(const Skier& skier);

  ~AirDragForce() { }
};

class KineticFrictionForce : public PhysicalForce {
public:
  Vector apply(const Skier& skier);

  ~KineticFrictionForce() { }
};

Vector normal_force(const Skier& skier);

Vector fall_line_force(const Skier& skier);

Vector lat_force(const Skier& skier);

Vector effective_force(const Skier& skier);

#endif /* PHYSICALFORCE_H */
