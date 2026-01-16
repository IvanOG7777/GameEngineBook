#pragma once

#include "Particle.h"

class Firework {
private:

public:

	struct FireWorkRule {
		Particle fireWorkParticle;
		unsigned int type;
		float minAge;
		float maxAge;
	};
};