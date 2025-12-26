#pragma once

#include <vector>

#include "core.h"
#include "Particle.h"
#include "globalConstants.h"

std::vector<Vector3> makeCircleFan(Vector3 center, float radius, int res);
void keepCircleInFrame(Particle& particle, float radius, int& windowWidth, int& windowHeight);
void keepCirclesInFrame(std::vector<Particle> &particles, std:: vector<float> radius);