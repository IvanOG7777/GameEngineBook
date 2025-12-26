#include "objects.h"
#define PI 3.14159265358979323846f

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

void keepCircleInFrame(Particle& particle, float& radius, int&windowWidth, int &windowHeight) {
	float minX = radius;
	float maxX = static_cast<float>(windowWidth) - radius;
	float minY = radius;
	float maxY = static_cast<float>(windowHeight) - radius;

	Vector3 p = particle.getPosition();
	Vector3 v = particle.getVelocity();

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

	particle.setPosition(p.x, p.y, p.z);
	particle.setVelocity(v.x, v.y, v.z);
}