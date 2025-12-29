#include <iostream>
#include "Particle.h"
#include "ballistics.h"
int main() {
	Ballistic ballistic; 

	std::cout << "Size of round: " << ballistic.rounds.size() << std::endl;

	for (int i = 0; i < ballistic.rounds.size(); i++) {
		std::cout << "Type of munition: " << ballistic.rounds[i].type << std::endl;
	}
	ballistic.currentShotType = ballistic.PISTOL;
	ballistic.fire();
	/*ballistic.currentShotType = ballistic.ARTILLERY;
	ballistic.fire();
	ballistic.currentShotType = ballistic.FIREBALL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	ballistic.currentShotType = ballistic.PISTOL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.ARTILLERY;
	ballistic.fire();
	ballistic.currentShotType = ballistic.FIREBALL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	ballistic.currentShotType = ballistic.PISTOL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.ARTILLERY;
	ballistic.fire();
	ballistic.currentShotType = ballistic.FIREBALL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	ballistic.currentShotType = ballistic.FIREBALL;
	ballistic.fire();
	ballistic.currentShotType = ballistic.LASER;
	ballistic.fire();
	*/

	while (true) {
		ballistic.updateRound();
	}

	for (int i = 0; i < ballistic.rounds.size(); i++) {
		std::cout << "Type of munition: " << ballistic.rounds[i].type << std::endl;
		std::cout << "Rounds position: ";
		ballistic.rounds[i].particle.printPosition();
		std::cout << "Rounds Acceleration: ";
		ballistic.rounds[i].particle.printAcceleration();
		std::cout << "Rounds velocity: ";
		ballistic.rounds[i].particle.printVelocity();
		std::cout << std::endl;

	}
}