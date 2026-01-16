//
// Created by elder on 01/16/2026.
//

#pragma once

#include "core.h"
#include "Particle.h"

class Firework {
public:

	enum FireworkType {
		UNUSED = 0,
		ROCKET,
		BURST,
		SPARK,
		FOUNTAIN,
	};

	//Defines this firework, the type, min and max age, min/max vel and damping
	struct FireWorkRule {
		Vector3 minVelocity{};
		Vector3 maxVelocity{};
		unsigned int type;
		float minAge;
		float maxAge;
		float damping;

		FireWorkRule() :
			minVelocity{},
			maxVelocity{},
			type(0),
			minAge(0),
			maxAge(0),
			damping(1)
		{}

		FireWorkRule(unsigned int type, float minAge, float maxAge, float damping, const Vector3& minVelocity, const Vector3& maxVelocity) :
			minVelocity(minVelocity),
			maxVelocity(maxVelocity),
			type(type),
			minAge(minAge),
			maxAge(maxAge),
			damping(damping)
		{}
	};


	// Defines when this firework dies and what new fireworks should be created
	struct Payload {
		unsigned int type;
		unsigned int count;

		Payload() : type(0), count(0) {}

		Payload(unsigned int type, unsigned int count) : type(type), count(count) {}
	};

	struct FireworkParticle {
		Particle particle;
		float age;
		unsigned int type;
	};

	struct FireworkNode {
		FireworkParticle* fireworkNode = nullptr;

		FireworkNode* left = nullptr;
		FireworkNode* right = nullptr;

		FireworkNode() : fireworkNode(nullptr), left(nullptr), right(nullptr) {}
		FireworkNode(FireworkParticle* fireworkParticle) : fireworkNode(fireworkParticle) {}
	};

	static constexpr unsigned maxFireworks = 1000;


private:

public:

	unsigned int payLoadCount;

	Payload* payloads;

	
};