#include <cassert>


#include "objects.h"
#include "ballistics.h"

std::vector <Vector3> makeCircleFan(Vector3 center, float radius, int res) {
	std::vector <Vector3> verticies;

	verticies.reserve(res + 2);
	verticies.push_back(center);

	for (int i = 0; i <= res; i++) {
		float t = static_cast<float>(i) / static_cast<float>(res);
		float a = t * 2.0f * PI;

		Vector3 position;

		position.x = center.x + std::cos(a) * radius;
		position.y = center.y + std::sin(a) * radius;
		position.z = 0;

		verticies.push_back(position);
	}

	return verticies;
}

// Parameters
// Pass in a particle object by reference
// Pass in the radis of particle
// Pass in the window height and width
void keepCircleInFrame(Particle& particle, float radius, int& windowWidth, int& windowHeight) {
	// Set the min x/y and max x/y values of the particle
	float minX = radius; // Min x is radius
	float maxX = static_cast<float>(windowWidth) - radius; // max is is the windowWidth - radius
	float minY = radius; // Min y is radius
	float maxY = static_cast<float>(windowHeight) - radius; // max is is the windowHeight - radius

	Vector3 p = particle.getPosition();
	Vector3 v = particle.getVelocity();

	// Right/Left
	if (p.x < minX) {
		p.x = minX;
		v.x = -v.x;
	}
	if (p.x > maxX) {
		p.x = maxX;
		v.x = -v.x;
	}

	// Bottom / Top
	if (p.y < minY) {
		p.y = minY;
		v.y = -v.y;
	}
	if (p.y > maxY) {
		p.y = maxY;
		v.y = -v.y;
	}

	// set the new position and velocity to the particle
	particle.setPosition(p.x, p.y, p.z);
	particle.setVelocity(v.x, v.y, v.z);
}

void resolveCollision(Ballistic::AmmoRound& round1, Ballistic::AmmoRound& round2) {
	Vector3 directionalVector = round2.particle.getPosition() - round1.particle.getPosition();
	float directionalVectorLength = directionalVector.magnitude();

	if (directionalVectorLength == 0.0f) return;

	Vector3 unitNormal = directionalVector / directionalVectorLength;

	float radius1 = round1.particle.getRadius();
	float radius2 = round2.particle.getRadius();

	float overlap = (radius1 + radius2) - directionalVectorLength;

	if (overlap <= 0.0f) return;

	float correction = overlap * 0.5f;

	Vector3 position1 = round1.particle.getPosition();
	Vector3 position2 = round2.particle.getPosition();

	position1 -= unitNormal * correction;
	position2 += unitNormal * correction;

	round1.particle.setPosition(position1);
	round2.particle.setPosition(position2);

	Vector3 v1 = round1.particle.getVelocity();
	Vector3 v2 = round2.particle.getVelocity();

	Vector3 relativeVelocity = round2.particle.getVelocity() - round1.particle.getVelocity();

	float velocityNormal = relativeVelocity.scalarProduct(unitNormal);

	if (velocityNormal > 0) {
		return;
	}

	float j = -(1 + e) * velocityNormal / (round1.particle.getInverseMass() + round2.particle.getInverseMass());

	Vector3 impulse = unitNormal * j;

	round1.particle.setVelocity(v1 - (impulse * round1.particle.getInverseMass()));
	round2.particle.setVelocity(v2 + (impulse * round2.particle.getInverseMass()));
}

bool circleCollision(Ballistic::AmmoRound &round1, Ballistic::AmmoRound &round2) {
	float distanceX = round1.particle.getPosition().x - round2.particle.getPosition().x;
	float distanceY = round1.particle.getPosition().y - round2.particle.getPosition().y;

	float realDistance = std::sqrtf((distanceX * distanceX) + (distanceY * distanceY));

	if (realDistance <= (round1.particle.getRadius() + round2.particle.getRadius())) {
		return true;
	}
	else {
		return false;
	}
}