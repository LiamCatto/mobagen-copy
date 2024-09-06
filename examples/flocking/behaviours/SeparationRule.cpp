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

  separatingForce = Vector2f::normalized(separatingForce);

  Vector2f boidPos = boid->getPosition();
  float distance = 0;
  Vector2f distVector = Vector2f::zero();
  Vector2f nearestNeighborPos = Vector2f::zero();
  float closestDistance = desiredMinimalDistance;

  if (!neighborhood.empty()) {
    for (auto* neighbor : neighborhood) {
      distance = sqrt(pow(boidPos.x - neighbor->getPosition().x,2) + pow(boidPos.y - neighbor->getPosition().y,2));
      if (distance < closestDistance) {
        closestDistance = distance;
        nearestNeighborPos = neighbor->getPosition();
      }
    }

    distVector = boidPos - nearestNeighborPos;
    separatingForce = distVector * 1/closestDistance;
  }

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
