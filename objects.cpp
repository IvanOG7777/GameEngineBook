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