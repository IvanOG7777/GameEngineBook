#pragma once
#include "Particle.h"
#include <cstdint>

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
		Particle particle;
		ShotType type = UNUSED;
		uint32_t startTime = 0;
	};

	static constexpr unsigned MaxAmmo = 16;

	public:
		Ballistic();
		AmmoRound ammoRound;

		void setShotType(ShotType &type);
		ShotType getShotType() const;

		void fire(uint32_t timeNowMs);
		void update(float dtSeconds, uint32_t timeNowMs);
};