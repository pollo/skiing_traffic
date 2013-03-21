/*
 * File:   downhill_force.h
 *
 * Declares 
 *
 */

#ifndef DOWNHILL_FORCE_H
#define	DOWNHILL_FORCE_H

#include "physicalforce.h"

class DownhillForce : public PhysicalForce {
public:
  Vector apply(const Skier& skier);

private:
};

#endif	/* DOWNHILL_FORCE_H */
