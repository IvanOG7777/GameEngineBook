//
// Created by elder on 01/16/2026.
//

#pragma once

#include "core.h"

class Firework {
public:

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

		FireWorkRule(unsigned int type, float minAge, float maxAge, float damping, Vector3& minVelocity, Vector3& maxVelocity) :
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


private:

public:

	unsigned int payLoadCount;

	Payload* payloads;

	
};