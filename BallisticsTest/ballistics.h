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
    AmmoRound ammoRound; // used for now only to define a single round. Not neccescary.
    ShotType currentShotType;
    std::vector<AmmoRound> rounds;

    void initializeParticleForAmmoRound(ShotType currentShotType);

    //IGNORE FOR NOW
    /*std::vector<AmmoRound> ammo;

    void setAmmo(ShotType passedShotType);

    void setShotType(ShotType type);
    ShotType getShotType() const;

    void fire(uint32_t timeNowMs);
    void update(float dtSeconds, uint32_t timeNowMs);*/
};