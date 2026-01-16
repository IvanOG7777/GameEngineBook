#pragma once

#include <vector>

#include "core.h"
#include "Particle.h"
#include "globalConstants.h"
#include "ballistics.h"

std::vector<Vector3> makeCircleFan(Vector3 center, float radius, int res);
void keepCircleInFrame(Particle& particle, int& windowWidth, int& windowHeight);
void sweptBounds(Particle& particle, double dt, int& windowWidth, int& windowHeight);
bool circleCollision(Ballistic::AmmoRound &round1, Ballistic::AmmoRound &round2);
void resolveCollision(std:: vector<Ballistic::AmmoRound> &rounds);
void resolveCollision2(Ballistic::BallisticNode* node1, Ballistic::BallisticNode* node2);
void resolveCollisionKDTree(Ballistic& ballistic, std::vector<Ballistic::AmmoRound>& rounds);
//void resolveMultipleCollitionKDTree(Ballistic& ballistic, std::vector < Ballistic::AmmoRound>& rounds);