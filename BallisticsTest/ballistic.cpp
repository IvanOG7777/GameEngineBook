#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "ballistics.h"
#include "globalConstants.h"

// default ballistic constructor, when a ballistic object is created
// rounds is resized to hold 16 AmmoRound structs
// set initial current shot to unused.
Ballistic::Ballistic() {
    rounds.resize(MaxAmmo);
    currentShotType = UNUSED;
    ammoRound = AmmoRound();
}

// function to initlaize a shot type with appropriate particle parameters
void Ballistic::initializeParticleForAmmoRound(ShotType currentType) { // pass in the type of shot with ShotType currentType
    // switch used to select how to initlaize a shot
    switch (currentType) {
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

    ammoRound.particle.setPosition(SCREENHEIGHT / 2.0f, SCREENWIDTH / 2.0f, 0.0f);
    ammoRound.type = currentType;
}

// function used to initlize a slot within rounds with round types particle parameters
void Ballistic::fire() {
    // initial for loop to loop through the rounds vector
    int roundIndex = 0;
    for (; roundIndex < rounds.size(); roundIndex++) {
        if (rounds[roundIndex].type == UNUSED) break; // if the current index.type is unused we have found a valid empty slot to initalize
    }

    // boundry checker to see if curent index is out of bounds of MaxAmmo 
    // if it is we return out of function call before intializeing another round at invalid index
    if (roundIndex >= rounds.size()) {
        // error logs
        std::cout << "Rounds is full" << std::endl;
        std::cout << "Cant initalize any more rounds" << std::endl;
        std::cout << "roundIndex: " << roundIndex << std::endl;
        return;
    }

    // if we find a vaild UNUSED spot initilize that spot with current shotType partucle parametes
    switch (currentShotType) {
    case PISTOL:
        rounds[roundIndex].particle.setMass(2.0f);
        rounds[roundIndex].particle.setVelocity(-10.0f, 35.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -1.0f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(2.0f);
        rounds[roundIndex].particle.setPosition(500.0f, 950.0f, 0.0f);
        std::cout << "Round at index: " << roundIndex << " has been initlaized to PISTOL" << std::endl;
        break;
    case ARTILLERY:
        rounds[roundIndex].particle.setMass(200.0f);
        rounds[roundIndex].particle.setVelocity(-8.0f, 30.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -20.0f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(10.0f);
        rounds[roundIndex].particle.setPosition(500.0f, 850.0f, 0.0f);
        std::cout << "Round at index: " << roundIndex << " has been initlaized ARTILLERY" << std::endl;
        break;
    case FIREBALL:
        rounds[roundIndex].particle.setMass(1.0f);
        rounds[roundIndex].particle.setVelocity(-10.0f, 10.0f, 0.0f);
        rounds[roundIndex].particle.setAcceleration(0.0f, -0.6f, 0.0f);
        rounds[roundIndex].particle.setDamping(0.99f);
        rounds[roundIndex].particle.setRadius(5.0f);
        rounds[roundIndex].particle.setPosition(500.0f, 700.0f, 0.0f);
        std::cout << "Round at index: " << roundIndex << " has been initlaized FIREBALL" << std::endl;
        break;
    case LASER:
        rounds[roundIndex].particle.setMass(0.1f);
        rounds[roundIndex].particle.setVelocity(0.0f, 0.0f, 100.0f);
        rounds[roundIndex].particle.setAcceleration(0, 0, 0);
        rounds[roundIndex].particle.setDamping(0.999f);
        rounds[roundIndex].particle.setPosition(200.0f, 700.0f, 0.0f);
        rounds[roundIndex].particle.setRadius(1.1f);
        std::cout << "Round at index: " << roundIndex << " has been initlaized LASER" << std::endl;
        break;
    }

    // set the particles default position and reassign its type to currentType
    rounds[roundIndex].type = currentShotType;
}

void Ballistic::updateRound(double &dt) {

    for (int i = 0; i < rounds.size(); i++) {
        if (rounds[i].type != UNUSED) {
            rounds[i].particle.update(dt);

            if (rounds[i].particle.getPosition().y <= 0.0f) {
                std::cout << "Shot at index: " << i << " has hit the ground" << std::endl;
                std::cout << "Setting shot at index: " << i << " to UNUSED" << std::endl;
                rounds[i].particle.clearAccumulator();
                rounds[i].particle.clearAllValues();
                rounds[i].type = UNUSED;
            }
        }
    }
}

// function used to check if all rounds within rounds vector are unused
bool Ballistic::allRoundsUnused() {
    for (int i = 0; i < rounds.size(); i++) {
        if (rounds[i].type != UNUSED) {
            return false;
        }
    }
    return true;
}

void Ballistic::spawnRound(int key) {
    Ballistic::AmmoRound round;
    if (key == 80) { // case for PISTOL
        round.particle.setMass(2.0f);
        round.particle.setVelocity(-10.0f, 35.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -1.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(2.0f);
        round.particle.setPosition(500.0f, 950.0f, 0.0f);
        Ballistic::currentShotType = Ballistic::PISTOL;
        round.type == currentShotType;  Ballistic::fire();
        std::cout << "PISTOL has been added" << std::endl;
    }

    if (key == 65) { // case for ARTILLERY
        round.particle.setMass(200.0f);
        round.particle.setVelocity(-8.0f, 30.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -20.0f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(10.0f);
        round.particle.setPosition(500.0f, 850.0f, 0.0f);
        Ballistic::currentShotType = Ballistic::ARTILLERY;
        round.type == currentShotType;  Ballistic::fire();

        std::cout << "ARTILLERY has been added" << std::endl;
    }

    if (key == 70) { // case for FIREBALL
        round.particle.setMass(1.0f);
        round.particle.setVelocity(-10.0f, 10.0f, 0.0f);
        round.particle.setAcceleration(0.0f, -0.6f, 0.0f);
        round.particle.setDamping(0.99f);
        round.particle.setRadius(5.0f);
        round.particle.setPosition(500.0f, 700.0f, 0.0f);
        Ballistic::currentShotType = Ballistic::FIREBALL;
        round.type == currentShotType; Ballistic::fire();

        std::cout << "FIREBALL has been added" << std::endl;
    }
}