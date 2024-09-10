#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

//#include <bits/fs_path.h>

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f cohesionForce;
  Vector2f centerOfMass = Vector2f::zero();
  Vector2f distVector;

  // todo: add your code here to make a force towards the center of mass
  // hint: iterate over the neighborhood

  if (!neighborhood.empty()) {
    for (auto* neighbor : neighborhood) {
      centerOfMass += neighbor->getPosition();
    }
    centerOfMass /= neighborhood.size();
    distVector = centerOfMass - boid->getPosition();
    if (distVector.getMagnitude() <= boid->getDetectionRadius()) {
      cohesionForce = distVector / boid->getDetectionRadius();
    } else {
      cohesionForce = Vector2f::zero();
    }
  }

  return cohesionForce;
}