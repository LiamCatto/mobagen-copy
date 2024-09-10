#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

//#include <bits/fs_path.h>

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f cohesionForce;
  Vector2f centerOfMass;
  Vector2f distVector;
  Vector2f boidPos = boid->getPosition();
  float distanceFromCenter;

  // todo: add your code here to make a force towards the center of mass
  // hint: iterate over the neighborhood

  // find center of mass
  if (neighborhood.size() > 0) {
    for (auto neighbor : neighborhood) {
      centerOfMass += neighbor->getPosition();
    }
    centerOfMass += boidPos;
    centerOfMass /= neighborhood.size() + 1;

    distVector = boid->getPosition() - centerOfMass;
    distanceFromCenter = sqrt(pow(distVector.x,2) + pow(distVector.y,2));
    if (distanceFromCenter <= boid->getDetectionRadius()) {
      cohesionForce = distVector / boid->getDetectionRadius();
    } else {
      cohesionForce = Vector2f::zero();
    }
  }

  return cohesionForce;
}