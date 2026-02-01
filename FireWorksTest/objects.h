#pragma once

#include <vector>
#include "Particle.h"
#include "globalConstants.h"
#include "firework.h"


std::vector<Vector3> makeCircleFan(Vector3 center, float radius, int res);
void keepCircleInFrame(Particle& particle, int& windowWidth, int& windowHeight);
void sweptBounds(Particle& particle, double dt, int& windowWidth, int& windowHeight);
bool circleCollison(std::shared_ptr<Firework::FireworkNode> &node1, std::shared_ptr<Firework::FireworkNode>& node2);