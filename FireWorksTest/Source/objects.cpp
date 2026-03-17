#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>


#include "../Header/objects.h"
#include "../Header/firework.h"

// function used to create verticies for a particle in a circle shape
// Parameters:
//		The center of the particle, on each frame loop we rerender and pass in the new positions for the particle and use that as center
//		The radius of the particle
//		The res, amount of points per circle, if res is more more triangles to make and more computations
std::vector <Vector3> makeCircleFan(Vector3 center, float radius, int res) {
	std::vector <Vector3> verticies; // create a vector of Vector3 called verticies

	verticies.reserve(res + 2); // reserve res amount of points plus 2 in memory for vertices
	verticies.emplace_back(center); // emplace_back the first center passed into the function to the vector

	// loop through res
	for (int i = 0; i <= res; i++) {
		float progress = static_cast<float>(i) / static_cast<float>(res); // progress is given by the formula (i/res)
		float theta = progress * 2.0f * PI; // compute the current theta angle with progress (i/res) * 2.0f * PI

		Vector3 position; // create a new Vector3 call position per loop

		// currently only using 2d object so z axis is being ignored
		position.x = center.x + std::cos(theta) * radius; // pass in the centers.x + the cos(theta) * radius, used for horizontal movement
		position.y = center.y + std::sin(theta) * radius; // pass in the centers.y + the sin(theta) * radius, used for vertical movement
		position.z = 0;

		verticies.emplace_back(position); // emplace_bakc the newly built position into the vertices vector
	}

	// once we finish the loop through res
	// return the fully built cirlce vertices
	return verticies;
}

// Parameters
// Pass in a particle object by reference
// Pass in the radis of particle
// Pass in the window height and width
void keepCircleInFrame(Particle& particle, int& windowWidth, int& windowHeight) {
	float radius = particle.getRadius();
	// Set the min x/y and max x/y values of the particle
	float minX = radius; // Min x is radius
	float maxX = static_cast<float>(windowWidth) - radius; // max is the windowWidth - radius
	float minY = radius; // Min y is radius
	float maxY = static_cast<float>(windowHeight) - radius; // max is the windowHeight - radius

	Vector3 p = particle.getPosition();
	Vector3 v = particle.getVelocity();

	// Right/Left
	// if p.x is less than the minX
	if (p.x < minX) {
		p.x = minX; // swap current x position with minX
		v.x = -v.x * e; // reverse the x velocity
	}
	// if p.x is less than the minX
	if (p.x > maxX) {
		p.x = maxX; // swap current x position with maxX
		v.x = -v.x * e; // reverse the x velocity
	}

	// Bottom / Top
	// if p.y is less than the minY
	if (p.y < minY) {
		p.y = minY; // swap current y position with minY
		v.y = -v.y * e; // reverse the y velocity
	}
	// if p.y is more than the maxY
	if (p.y > maxY) {
		p.y = maxY; // swap current y position with maxY
		v.y = -v.y * e; // reverse the y velocity
	}

	// set the new position and velocity to the particle
	particle.setPosition(p.x, p.y, p.z);
	particle.setVelocity(v.x, v.y, v.z);
}

void sweptBounds(Particle& particle, double dt, int& windowWidth, int& windowHeight) {
	float radius = particle.getRadius();
	Vector3 p = particle.getPosition();
	Vector3 v = particle.getVelocity();

	float maxX = static_cast<float>(windowWidth) - radius;
	float minX = radius;
	float maxY = static_cast<float>(windowHeight) - radius;
	float minY = radius;

	// moving to the right
	if (v.x > 0.0f) {
		double tHit = (maxX - p.x) / v.x; // calculates the time particle hits the wall

		// checks 0.0 < tHit < dt
		if (tHit >= 0.0 && tHit <= dt) {

			p.x += v.x * static_cast<float>(tHit); // moves to the actual impact point in this case the wall

			v.x = -v.x * e; // inverse and take away a small amount of the velocity

			double remaining = dt - tHit; // calculate the remaining time

			if (remaining > 0.0) { // if remaining time is greater than 0
				p.x += v.x * static_cast<float>(remaining); // move updated position by v.x * remaining pixels for rest of frame
			}
		}
	}
	else if (v.x < 0.0) { // moving to the left
		double tHit = (minX - p.x) / v.x;

		if (tHit >= 0.0 && tHit <= dt) {

			p.x += v.x * static_cast<float>(tHit);

			v.x = -v.x * e;

			double remaining = dt - tHit;

			if (remaining > 0.0) {
				p.x += v.x * static_cast<float>(remaining);
			}
		}
	}

	if (v.y > 0.0f) {
		double tHit = (maxY - p.y) / v.y;

		if (tHit >= 0.0 && tHit <= dt) {
			p.y += v.y * static_cast<float>(tHit);

			v.y = -v.y * e;

			double remaining = dt - tHit;

			if (remaining > 0.0) {
				p.y += v.y * static_cast<float>(remaining);
			}
		}
	}
	else if (v.y < 0.0f) {
		double tHit = (minY - p.y) / v.y;

		if (tHit >= 0.0 && tHit <= dt) {
			p.y += v.y * static_cast<float>(tHit);

			v.y = -v.y * e;

			double remaining = dt - tHit;

			if (remaining > 0.0) {
				p.y += v.y * static_cast<float>(remaining);
			}
		}
	}

	if (p.x < minX) {
		p.x = minX; // swap current x position with minX
		v.x = -v.x * e; // reverse the x velocity
	}
	// if p.x is less than the minX
	if (p.x > maxX) {
		p.x = maxX; // swap current x position with maxX
		v.x = -v.x * e; // reverse the x velocity
	}

	// Bottom / Top
	// if p.y is less than the minY
	if (p.y < minY) {
		p.y = minY; // swap current y position with minY
		v.y = -v.y * e; // reverse the y velocity
	}
	// if p.y is more than the maxY
	if (p.y > maxY) {
		p.y = maxY; // swap current y position with maxY
		v.y = -v.y * e; // reverse the y velocity
	}

	particle.setPosition(p.x, p.y, p.z);
	particle.setVelocity(v.x, v.y, v.z);
}

bool circleCollision(std::shared_ptr<Firework::FireworkNode>& node1, std::shared_ptr<Firework::FireworkNode>& node2) {
	float distanceX = node1->particle.getPosition().x - node2->particle.getPosition().x;
	float distanceY = node1->particle.getPosition().y - node2->particle.getPosition().y;

	float radiusSum = node1->particle.getRadius() + node2->particle.getRadius();

	float distance = sqrtf((distanceX * distanceX) + (distanceY * distanceY));

	if (distance <= radiusSum) return true;

	return false;
}

void resolveAllCollisionsKDTree(Firework &firework) {
	if (firework.activeFireworks.empty()) return;

	for (auto &node : firework.activeFireworks) {
		if (node == nullptr) continue;
		auto closestNode = firework.findBestNode(node);
		auto lockedClosestNode = closestNode.lock();

		if (lockedClosestNode == nullptr) return;

		resolvePairCollision(node, lockedClosestNode);
	}
}

void resolvePairCollision(std::shared_ptr<Firework::FireworkNode> &node1, std::shared_ptr<Firework::FireworkNode> &node2) {
	if (!circleCollision(node1, node2)) return;

	Vector3 directionalVector = node2->particle.getPosition() - node1->particle.getPosition();
	float directionalVectorLength = directionalVector.magnitude();

	if (directionalVectorLength == 0.0f) return;

	Vector3 unitNormal = directionalVector / directionalVectorLength;

	float radius1 = node1->particle.getRadius();
	float radius2 = node2->particle.getRadius();

	float overlap = (radius1 + radius2) - directionalVectorLength;

	float correction = overlap * 0.5f;

	if (overlap <= 0.0f) return;

	Vector3 position1 = node1->particle.getPosition();
	Vector3 position2 = node2->particle.getPosition();

	position1 -= unitNormal * correction;
	position2 += unitNormal * correction;

	node1->particle.setPosition(position1);
	node2->particle.setPosition(position2);

	Vector3 velocity1 = node1->particle.getVelocity();
	Vector3 velocity2 = node2->particle.getVelocity();

	Vector3 relativeVelocity = velocity2 - velocity1;

	float velocityNormal = relativeVelocity.scalarProduct(unitNormal);

	if (velocityNormal > 0.0f) return;

	float jImpulse = -(1 + e) * velocityNormal / (node1->particle.getInverseMass() + node2->particle.getInverseMass());

	Vector3 impulse = unitNormal * jImpulse;

	node1->particle.setVelocity(velocity1 - (impulse * node1->particle.getInverseMass()));
	node2->particle.setVelocity(velocity2 + (impulse * node2->particle.getInverseMass()));
}