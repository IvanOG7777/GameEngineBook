//
// Created by elder on 01/16/2026.
//

#pragma once

#include <iostream>
#include <vector>

#include "core.h"
#include "Particle.h"

class Firework {
public:

	enum FireworkType {
		UNUSED = 0,
		SMALL,
		MEDIUM,
		LARGE,
		EXTRALARGE,
	};

	//Defines this firework, the type, min and max age, min/max vel and damping
	struct FireworkRule {
		Vector3 minVelocity{};
		Vector3 maxVelocity{};
		unsigned int type;
		float minAge;
		float maxAge;
		float damping;

		//default constructor
		FireworkRule() :
			minVelocity{},
			maxVelocity{},
			type(UNUSED),
			minAge(0),
			maxAge(0),
			damping(1)
		{}

		//constructor with passed parameters
		FireworkRule(unsigned int type, float minAge, float maxAge, float damping, const Vector3& minVelocity, const Vector3& maxVelocity) :
			minVelocity(minVelocity),
			maxVelocity(maxVelocity),
			type(type),
			minAge(minAge),
			maxAge(maxAge),
			damping(damping)
		{}

		/*~FireworkRule() {
			std::cout << "Instance of FireworkRule has been destroyed" << std::endl;
		}*/
	};


	// Defines when this firework dies and what new fireworks should be created
	struct Payload {
		unsigned int type;
		unsigned int count;

		//default constructor
		Payload() : type(UNUSED), count(0) {}

		//constructor with passed parameters
		Payload(unsigned int type, unsigned int count) : type(type), count(count) {}

		/*~Payload() {
			std::cout << "Instance of Payload has been destroyed" << std::endl;
		}*/
	};

	// struct to define a single firework particle
	struct FireworkParticle {
		Particle particle;
		float age;
		unsigned int type;

		FireworkParticle() : age(0), type(UNUSED) {}

		FireworkParticle(float age, unsigned int type) : age(age), type(type) {}

		/*~FireworkParticle() {
			std::cout << "Instance of FireworkParticle has been destroyed" << std::endl;
		}*/
	};

	struct FireworkNode {
		FireworkParticle* fireworkNode = nullptr;

		FireworkNode* left = nullptr;
		FireworkNode* right = nullptr;

		FireworkNode() : fireworkNode(nullptr), left(nullptr), right(nullptr) {}
		FireworkNode(FireworkParticle* fireworkParticle) : fireworkNode(fireworkParticle) {}

		/*~FireworkNode() {
			delete left;
			delete right;
			std::cout << "Instance of fireworkNode has been destroyed" << std::endl;
		}*/
	};

	static constexpr unsigned maxFireworks = 10;


private:
	FireworkNode* root;

	float distance2(FireworkNode* node1, FireworkNode *node2);
	void findNNHelper(FireworkNode* current, FireworkNode* target, FireworkNode *&bestNode, float &distance, int depth);
	void addNode(FireworkNode* node);
	FireworkNode allocateNode(FireworkParticle* fireworkParticle);

public:
	Firework();
	FireworkParticle singleFireWork;
	FireworkType currentFireworkType;
	std::vector<FireworkParticle> fireworks;
	std::vector<FireworkNode> nodepool;
	int poolUsed;

	/*unsigned int payloadCount;
	Payload* payloads;*/

	void init(unsigned int payloadCount);

	/*~Firework() {
		delete payloads;
		payloads = nullptr;
		std::cout << "Firework class has been destroyed" << std::endl;
	}*/

	
};