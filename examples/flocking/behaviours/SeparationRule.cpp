#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to avoid boids too close
  Vector2f separatingForce = Vector2f::zero();

  //    float desiredDistance = desiredMinimalDistance;
  //
  //    // todo: implement a force that if neighbor(s) enter the radius, moves the boid away from it/them
  //    if (!neighborhood.empty()) {
  //        Vector2f position = boid->transform.position;
  //        int countCloseFlockmates = 0;
  //        // todo: find and apply force only on the closest mates
  //    }

  Vector2f boidPos = boid->getPosition();
  Vector2f distVector = Vector2f::zero();
  Vector2f nearestNeighborPos = Vector2f::zero();
  float distance = 0;
  float closestDistance = desiredMinimalDistance;

  if (!neighborhood.empty()) {  // Find the position of the closest neighbor
    for (auto* neighbor : neighborhood) {
      distance = sqrt(pow(boidPos.x - neighbor->getPosition().x,2) + pow(boidPos.y - neighbor->getPosition().y,2));
      if (distance < closestDistance) {
        closestDistance = distance;
        nearestNeighborPos = neighbor->getPosition();
      }
    }

    /* Best attempt */
    distVector = boidPos - nearestNeighborPos;
    separatingForce = distVector * 1/closestDistance;

    /* similar to formula without summation, doesn't work in practice */
    //distVector = nearestNeighborPos - boidPos;
    //separatingForce = Vector2f::normalized(distVector) / distVector.getMagnitude();

    /* Most similar to formula, includes summation, doesn't work in practice */
    /*for (auto* neighbor : neighborhood) {
      distance = sqrt(pow(boidPos.x - neighbor->getPosition().x,2) + pow(boidPos.y - neighbor->getPosition().y,2));
      if (distance > 0 && distance < desiredMinimalDistance) {
        distVector = boidPos - neighbor->getPosition();
        //separatingForce += (Vector2f::normalized(distVector) / distVector.getMagnitude());
        separatingForce = distVector * 1/closestDistance;
      } else {
        separatingForce = Vector2f::zero();
      }
    }*/
  }

  //separatingForce = Vector2f::normalized(separatingForce);
  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
