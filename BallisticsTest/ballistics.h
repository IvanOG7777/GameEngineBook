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
		AmmoRound ammoRound;
		ShotType shotType;
		std::vector<AmmoRound> ammo;

		void initalizeParticleForAmmoRound(ShotType shotType);

		void setAmmo(ShotType passedshotType);

		void setShotType(ShotType type);
		ShotType getShotType() const;

		void fire(uint32_t timeNowMs);
		void update(float dtSeconds, uint32_t timeNowMs);
};