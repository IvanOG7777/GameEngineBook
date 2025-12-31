#pragma once

#include <vector>

#include "core.h"
#include "Particle.h"
#include "globalConstants.h"
#include "ballistics.h"

std::vector<Vector3> makeCircleFan(Vector3 center, float radius, int res);
void keepCircleInFrame(Particle& particle, float radius, int& windowWidth, int& windowHeight);
bool circleCollision(Ballistic::AmmoRound &round1, Ballistic::AmmoRound &round2);
void resolveCollision(std:: vector<Ballistic::AmmoRound> &rounds);
void keepCirclesInFrame(std::vector<Particle>& particles, std::vector<float> radius);