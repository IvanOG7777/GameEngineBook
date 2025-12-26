#include "objects.h"

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
void keepCircleInFrame(Particle& particle, float radius, int&windowWidth, int &windowHeight) {
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