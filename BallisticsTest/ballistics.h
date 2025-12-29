#pragma once
#include "Particle.h"
#include <cstdint>
#include <vector>

class Ballistic {
public:
    enum ShotType {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
    };

    struct AmmoRound {
        uint32_t startTime = 0;
        Particle particle;
        ShotType type = UNUSED;
    };

    static constexpr unsigned MaxAmmo = 16;

public:
    Ballistic(); // default Ballistic object constructor
    AmmoRound ammoRound; // used for now only to define a single round. Not neccescary. I will use mainly for testing
    ShotType currentShotType; // set the current type of shot for when use use fire() it will fire that current shot within rounds
    std::vector<AmmoRound> rounds; // vector of AmmoRound Structs, can hold multiple types of shots PISTOL,ARTILERY...

    void initializeParticleForAmmoRound(ShotType currentType); // function used to initalize an AmmoRound struct, function is really only used for testing since its not cycling through all elements of rounds

    // function used to initalize rounds within rounds vector
    void fire();
    // function used to updated rounds within rounds vector, acceleration, velocity, postion...
    void updateRound();
};