#include <iostream>
#include "Particle.h"
#include "ballistics.h"
int main() {
	Ballistic b;

	b.initalizeParticleForAmmoRound(Ballistic::ShotType::PISTOL);
	std::cout << "Radius: " << b.ammoRound.particle.getRadius() << std::endl;
	std::cout << "Mass: " << b.ammoRound.particle.getMass() << std::endl;
	std::cout << "Acceleration: ";
	b.ammoRound.particle.printAcceleration();
	std::cout << "Velocity: ";
	b.ammoRound.particle.printVelocity();
	std::cout << "Position: ";
	b.ammoRound.particle.printPosition();

	b.ammoRound.particle.update(0.016);
	std::cout << "After 0.016 change: " << std::endl;

	std::cout << "Radius: " << b.ammoRound.particle.getRadius() << std::endl;
	std::cout << "Mass: " << b.ammoRound.particle.getMass() << std::endl;
	std::cout << "Acceleration: ";
	b.ammoRound.particle.printAcceleration();
	std::cout << "Velocity: ";
	b.ammoRound.particle.printVelocity();
	std::cout << "Position: ";
	b.ammoRound.particle.printPosition();

	std::cout << std::endl;

	b.setAmmo(Ballistic::ShotType::PISTOL);

	for (auto& type : b.ammo) {
		std::cout << type.type << std::endl;
	}




	//while (true) {
	//	if (b.ammoRound.particle.getPosition().y <= 0) {
	//		std::cout << "Particle has hit the ground" << std::endl;
	//		break;
	//	}
	//	b.ammoRound.particle.update(0.016);

	//	std::cout << "Acceleration: ";
	//	b.ammoRound.particle.printAcceleration();

	//	std::cout << "Velocity: ";
	//	b.ammoRound.particle.printVelocity();

	//	std::cout << "Postion: ";
	//	b.ammoRound.particle.printPosition();
	//}

}