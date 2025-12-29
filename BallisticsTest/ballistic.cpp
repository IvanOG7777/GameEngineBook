#include <iostream>
#include "ballistics.h"

// default ballistic constructor, when a ballistic object is created
// rounds is resized to hold 16 AmmoRound structs
// set initial current shot to unused.
Ballistic::Ballistic() {
    rounds.resize(MaxAmmo);
    currentShotType = UNUSED;
    ammoRound = AmmoRound();
}


void Ballistic::initializeParticleForAmmoRound(ShotType currentShotType) {
    switch (currentShotType) {
    case (Ballistic::ShotType::UNUSED):
        std::cout << "Unsed breaking out of initlaizeation" << std::endl;
        break;
    case(Ballistic::ShotType::PISTOL):
        ammoRound.particle.setMass(2.0f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 35.0f);
        ammoRound.particle.setAcceleration(0.0f, -1.0f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(2.0f);
        break;
    case(Ballistic::ShotType::ARTILLERY):
        ammoRound.particle.setMass(200.0f);
        ammoRound.particle.setVelocity(0.0f, 30.0f, 40.0f);
        ammoRound.particle.setAcceleration(0.0f, -20.0f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(10.0f);
        break;
    case (Ballistic::ShotType::FIREBALL):
        ammoRound.particle.setMass(1.0f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 10.0f);
        ammoRound.particle.setAcceleration(0.0f, 0.6f, 0.0f);
        ammoRound.particle.setDamping(0.99f);
        ammoRound.particle.setRadius(5.0f);
        break;
    case (Ballistic::ShotType::LASER):
        ammoRound.particle.setMass(0.1f);
        ammoRound.particle.setVelocity(0.0f, 0.0f, 100.0f);
        ammoRound.particle.setAcceleration(0, 0, 0);
        ammoRound.particle.setDamping(0.999f);
        ammoRound.particle.setRadius(.1f);
        break;
    }

    ammoRound.particle.setPosition(0.0f, 1.5f, 0.0f);
    ammoRound.type = currentShotType;
}

//void Ballistic::setShotType(ShotType passedShotType) {
//	shotType = passedShotType;
//}
//
//Ballistic::ShotType Ballistic::getShotType() const {
//	return shotType;
//}
//
//void Ballistic::setAmmo(ShotType passedshotType) {
//	initalizeParticleForAmmoRound(passedshotType);
//
//	ammo.resize(MaxAmmo);
//	std::cout << ammo.size() << std::endl;
//
//	for (auto i = 0; i < ammo.size(); i++) {
//		ammo[i] = ammoRound;
//	}
//}