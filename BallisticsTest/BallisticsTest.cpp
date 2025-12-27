#include <iostream>
#include "Particle.h"
#include "ballistics.h"
int main() {
	Ballistic ballistic; // create  a ballistic object

	ballistic.initalizeParticleForAmmoRound(Ballistic::ShotType::PISTOL);
	std::cout << "Radius: " << ballistic.ammoRound.particle.getRadius() << std::endl;
	std::cout << "Mass: " << ballistic.ammoRound.particle.getMass() << std::endl;
	std::cout << "Acceleration: ";
	ballistic.ammoRound.particle.printAcceleration();
	std::cout << "Velocity: ";
	ballistic.ammoRound.particle.printVelocity();
	std::cout << "Position: ";
	ballistic.ammoRound.particle.printPosition();

	ballistic.ammoRound.particle.update(0.016);
	std::cout << "After 0.016 change: " << std::endl;

	std::cout << "Radius: " << ballistic.ammoRound.particle.getRadius() << std::endl;
	std::cout << "Mass: " << ballistic.ammoRound.particle.getMass() << std::endl;
	std::cout << "Acceleration: ";
	ballistic.ammoRound.particle.printAcceleration();
	std::cout << "Velocity: ";
	ballistic.ammoRound.particle.printVelocity();
	std::cout << "Position: ";
	ballistic.ammoRound.particle.printPosition();


	std::cout << "Type of round: " << ballistic.ammoRound.type << std::endl;


	while (true) {
		if (ballistic.ammoRound.particle.getPosition().y <= 0) {
			std::cout << "particle has hit the ground" << std::endl;
			break;
		}

		ballistic.ammoRound.particle.update(0.016);
		std::cout << "acceleration: ";
		ballistic.ammoRound.particle.printAcceleration();

		std::cout << "velocity: ";
		ballistic.ammoRound.particle.printVelocity();

		std::cout << "postion: ";
		ballistic.ammoRound.particle.printPosition();
	}

	std::cout << "Size of ammoRounds array: " << ballistic.rounds->type << std::endl;
}