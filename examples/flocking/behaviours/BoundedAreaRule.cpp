#include "BoundedAreaRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f BoundedAreaRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Return a force proportional to the proximity of the boids with the bounds, and opposed to it
  Vector2f force = Vector2f::zero();  // zero

  // todo: add here your code code here do make the boid follow the bounded box rule
  // hint: use this->world->engine->window->size() and desiredDistance

  auto size = this->world->engine->window->size();
  Vector2f boidPos = boid->getPosition();
  Vector2f displacement = Vector2f::zero();

  // The point on the nearest boundary that the boid is closest to
  Vector2f boundIntercept = Vector2f::zero();

  // Left boundary position
  if (boidPos.x <= 0 + desiredDistance) {
    boundIntercept = Vector2f(0, boidPos.y);
  }
  // Right boundary position
  if (boidPos.x >= size.x - desiredDistance) {
    boundIntercept = Vector2f(size.x, boidPos.y);
  }
  // Bottom boundary position
  if (boidPos.y <= 0 + desiredDistance) {
    boundIntercept = Vector2f(boidPos.x, 0);
  }
  // Top boundary position
  if (boidPos.y >= size.y - desiredDistance) {
    boundIntercept = Vector2f(boidPos.x, size.y);
  }

  // Only apply this force if the boid is near a boundary
  if (boundIntercept.getMagnitude() != 0) {
    displacement = boidPos - boundIntercept;
    force += displacement / boid->getDetectionRadius();
    force *= boid->getVelocity().getMagnitude();
  }

  return force;
}

bool BoundedAreaRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  auto size = this->world->engine->window->size();
  auto widthWindows = size.x;
  auto heightWindows = size.y;
  bool valueHasChanged = false;

  // We cap the max separation as the third of the min of the width.height
  auto minHeightWidth = std::min(widthWindows, heightWindows);

  if (ImGui::SliderInt("Desired Distance From Borders", &desiredDistance, 0.0f, (int)(minHeightWidth / 3), "%i")) {
    valueHasChanged = true;
  }

  return valueHasChanged;
}

void BoundedAreaRule::draw(const Boid& boid, SDL_Renderer* renderer) const {
  FlockingRule::draw(boid, renderer);
  auto size = this->world->engine->window->size();
  auto dist = (float)desiredDistance;

  // Draw a rectangle on the map
  Polygon::DrawLine(renderer, Vector2f(dist, dist), Vector2f(size.x - dist, dist), Color::Gray);                    // TOP
  Polygon::DrawLine(renderer, Vector2f(size.x - dist, dist), Vector2f(size.x - dist, size.y - dist), Color::Gray);  // RIGHT
  Polygon::DrawLine(renderer, Vector2f(size.x - dist, size.y - dist), Vector2f(dist, size.y - dist), Color::Gray);  // Bottom
  Polygon::DrawLine(renderer, Vector2f(dist, size.y - dist), Vector2f(dist, dist), Color::Gray);                    // LEFT
}