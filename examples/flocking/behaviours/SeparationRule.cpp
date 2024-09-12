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

  Vector2f distVector = Vector2f::zero();

  if (!neighborhood.empty()) {
    for (auto* neighbor : neighborhood) {

      // Get the distance between boid and neighbor
      distVector = boid->getPosition() - neighbor->getPosition();

      // Check if neighbor is within the desiredMinimalDistance aka "hate radius" as per my professor
      if (distVector.getMagnitude() > 0 && distVector.getMagnitude() <= desiredMinimalDistance) {
        separatingForce += Vector2f::normalized(distVector) / distVector.getMagnitude();
      } else if (distVector.getMagnitude() == 0 || distVector.getMagnitude() > desiredMinimalDistance) {
        separatingForce += Vector2f::zero();
      }
    }
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
