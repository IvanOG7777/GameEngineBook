#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>


#include "objects.h"
#include "ballistics.h"

// function used to create verticies for a particle in a cirlce shape
// Parameters:
//		The center of the particle, on each frame loop we rerender and pass in the new positions for the particle and use that as center
//		The radius of the particle
//		The res, amount of points per circle, if res is more more triangles to make and more computations
std::vector <Vector3> makeCircleFan(Vector3 center, float radius, int res) {
	std::vector <Vector3> verticies; // create a vector of Vector3 called verticies

	verticies.reserve(res + 2); // reserve res amount of points plus 2 in memory for vertices
	verticies.emplace_back(center); // emplace_bakc the first center passed into the function to the vector

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
	float maxX = static_cast<float>(windowWidth) - radius; // max is is the windowWidth - radius
	float minY = radius; // Min y is radius
	float maxY = static_cast<float>(windowHeight) - radius; // max is is the windowHeight - radius

	Vector3 p = particle.getPosition();
	Vector3 v = particle.getVelocity();

	// Right/Left
	// if p.x is less than the minX
	if (p.x < minX) {
		p.x = minX; // swap current x position with minX
		v.x = -v.x * e; // reverse the x veclocity
	}
	// if p.x is less than the minX
	if (p.x > maxX) {
		p.x = maxX; // swap current x position with maxX
		v.x = -v.x * e; // reverse the x veclocity
	}

	// Bottom / Top
	// if p.y is less than the minY
	if (p.y < minY) {
		p.y = minY; // swap current y position with minY
		v.y = -v.y * e; // reverse the y veclocity
	}
	// if p.y is more than the maxY
	if (p.y > maxY) {
		p.y = maxY; // swap current y position with maxY
		v.y = -v.y * e; // reverse the y veclocity
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
	if ((v.x > 0.0f) && (p.x < maxX)) {
		double tHit = (maxX - p.x) / v.x;

		if (tHit >= 0.0 && tHit <= dt) {

			p.x += v.x * static_cast<float>(tHit); // moves to the actual imact point in this case the wall

			v.x = -v.x * e; // inverse and take away a small amount of the velocity

			double remaining = dt - tHit; // calculate the remaining time

			if (remaining > 0.0) { // if remaining time is greater than 0
				p.x += v.x * static_cast<float>(remaining); // move
			}
		}
	}
	else if (p.x > maxX){
		p.x = maxX; // swap current x position with maxX
		v.x = -v.x * e; // reverse the x veclocity
	}
	
	if ((v.x < 0.0) && (p.x > minX)) { // moving to the left
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
	else if (p.x < minX) {
		p.x = minX; // swap current x position with minX
		v.x = -v.x * e; // reverse the x veclocity
	}

	if ((v.y > 0.0f) && (p.y < maxY)) {
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
	else if (p.y > maxY) {
		p.y = maxY; // swap current x position with maxX
		v.y = -v.y * e; // reverse the x veclocity
	}
	
	if ((v.y < 0.0f) && (p.y > minY)) {
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
	else if (p.y < minY) {
		p.y = minY; // swap current x position with maxX
		v.y = -v.y * e; // reverse the x veclocity
	}

	particle.setPosition(p.x, p.y, p.z);
	particle.setVelocity(v.x, v.y, v.z);
}


// Loops over all rounds
// Its ok for now since our test cases are small but with more particles we will be doing many uncessacery collision checks
// Tested with about 1000 particles and frames droped from around 160 to 30
// we need to change this to a sweep and prune algorithim or kd-trees or something similar
void resolveCollision(std::vector<Ballistic::AmmoRound>& rounds) {

	// Doing a nested for loop
	// Each object needs to check all other objects within the vector
	for (int i = 0; i < rounds.size(); i++) {
		if (rounds[i].type == Ballistic::UNUSED) continue; // if current rounds[i] type is UNUSED skip it
		for (int j = i + 1; j < rounds.size(); j++) {
			if (rounds[j].type == Ballistic::UNUSED) continue; // if current rounds[j] type is UNUSED skip it

			// grab a reference of objects at i and j
			// assign to round1 and round2 respectivly
			Ballistic::AmmoRound &round1 = rounds[i];
			Ballistic::AmmoRound &round2 = rounds[j];

			// if circleCollison returns false no collison continue to the next J index
			// if circleCollison returns true execute code below
			if (!circleCollision(round1, round2)) continue;

				// compute the directionlVector
				Vector3 directionalVector = round2.particle.getPosition() - round1.particle.getPosition(); // calculates the vector it takes to get from r2 to r1
				float directionalVectorLength = directionalVector.magnitude(); // calculates the length it takes to get from r2 to r1 in a stright line

				if (directionalVectorLength == 0.0f) continue; // check if the length is equal to 0 if it is we move to next j index

				Vector3 unitNormal = directionalVector / directionalVectorLength; // calculate the unit normal vector of (directional vector) / directionalLength
				// this vector only conserves the direction of where the objects need to do after collison.

				// grab each particles radius
				float radius1 = round1.particle.getRadius();
				float radius2 = round2.particle.getRadius();

				// calculate thier overlap
				// radius1 + radius2 the distance where circles would just touch
				// directionalVectorLength the actual distance between centers
				float overlap = (radius1 + radius2) - directionalVectorLength;

				// if overlap is seperated (<) or just touching (=), continue to next index
				if (overlap <= 0.0f) continue;

				// calculate the correction for the overlap
				float correction = overlap * 0.5f;

				// create new positon vectors to edit
				Vector3 position1 = round1.particle.getPosition();
				Vector3 position2 = round2.particle.getPosition();

				// seperation the particles through the unitNormal vector
				// Ex if they hit directly on the x axis, one will go left and the other to the right
				position1 -= unitNormal * correction;
				position2 += unitNormal * correction;

				// pass the new postions to the actual object
				round1.particle.setPosition(position1);
				round2.particle.setPosition(position2);

				// create new velocity vectors to edit
				Vector3 v1 = round1.particle.getVelocity();
				Vector3 v2 = round2.particle.getVelocity();

				// Calculate the relative velocity between the two
				// Think of it as how is round2 moving compared to round1
				//If both move together the relative velocity = 0
				// If they move toward each other the relative velocity points inward
				// If they move away from each other the relative velocity points outward
				Vector3 relativeVelocity = v2 - v1;

				//calculate the velocity Normal vector by scaling each component by unitNormal vector and returing sum
				float velocityNormal = relativeVelocity.scalarProduct(unitNormal);

				// velocityNormal greater than 0 means objects are moving away from eachother along the collision axis
				if (velocityNormal > 0) continue;

				// calculate the impulse scaler'
				// smaller jImpulse values means a soft collisions
				// larger jImpulse values means Bouncy collisions
				float jImpulse = -(1 + e) * velocityNormal / (round1.particle.getInverseMass() + round2.particle.getInverseMass());

				Vector3 impulse = unitNormal * jImpulse; // calculate the impulse vector by scaling the unitNormal vector(vector the objects will be moving in

				// set each particles new velocities
				round1.particle.setVelocity(v1 - (impulse * round1.particle.getInverseMass()));
				round2.particle.setVelocity(v2 + (impulse * round2.particle.getInverseMass()));
		}
	}
}

// function used to check if two cirlces have collided
// credit for logic @: https://jeffreythompson.org/collision-detection/circle-circle.php, this webside has MANY different ways to test for collisions
// bool function since we only want to know if they have collided
// Parametes:
//		Two Ballistic class AmmoRound struct's, passing them in by reference since we want to thier actualy values
bool circleCollision(Ballistic::AmmoRound &round1, Ballistic::AmmoRound &round2) {

	// calculations for distance between x and y axis of both rounds
	float distanceX = round1.particle.getPosition().x - round2.particle.getPosition().x; // grab round1 x position and subtract by round2 x position, This givesus the distance int the x direction between rounds
	float distanceY = round1.particle.getPosition().y - round2.particle.getPosition().y; // grab round1 x position and subtract by round2 y position, This givesus the distance int the y direction between rounds

	// this calculates the real distance between the circles
	float realDistance = std::sqrtf((distanceX * distanceX) + (distanceY * distanceY)); // we take the sqrt(distanceX^2 + distanceY^2) to get real distance

	// check if the realDistance is small than or equal to the sum of both rounds radiuses.
	// This means that the objects are either touching or inside of eachother
	if (realDistance <= (round1.particle.getRadius() + round2.particle.getRadius())) return true; // return true that we have collided
	
	// if all else above doesnt return true
	return false; // return false we have not collided
}