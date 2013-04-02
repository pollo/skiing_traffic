/*
 * File:   socialforce.h
 *
 * The class Socialforce
 */

#ifndef SOCIALFORCE_H
#define SOCIALFORCE_H


#include "vector.h"
#include "skier.h"

class SocialForce {
public:
  virtual ~SocialForce() {};

  //returns the force Vector that this force applies on the skier
  virtual Vector apply(const Skier& skier) = 0;
};

//models the attraction force to the choosen waypoint
class DestinationForce : public SocialForce {
public:
  Vector apply(const Skier& skier);

  ~DestinationForce() { }
};

//models the repulsion force from the left edge of the slope
class LeftForce : public SocialForce {
public:
  Vector apply(const Skier& skier);

  ~LeftForce() { }
};

//models the repulsion force from the right edge of the slope
class RightForce : public SocialForce {
public:
  Vector apply(const Skier& skier);

  ~RightForce() { }
};

#endif /* SOCIALFORCE_H */