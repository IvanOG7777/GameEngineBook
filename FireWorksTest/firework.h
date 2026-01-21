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

	// this enum will eventually handle how the effect will look like
	enum BurstType {
		ROCKET, // shape will me more concetrated vertically
		FOUNTAIN, // more spread out horizontally, not to much vertically
		FIREWORK, // shape will maybe be a mix of the two, aybe certian particles goin up high then flowing horizontally
	};

	// size of firework particle
	enum FireworkSizeType {
		UNUSED = 0,
		SMALL,
		MEDIUM,
		LARGE,
		EXTRALARGE,
	};

	// Defines when this firework dies and what new fireworks should be created
	struct Payload {
		unsigned int type;
		unsigned int count;

		void set(unsigned int type, unsigned int count) {
			Payload::type = type;
			Payload::count = count;
		}

		//default constructor
		Payload() : type(UNUSED), count(0) {}

		//constructor with passed parameters
		Payload(unsigned int type, unsigned int count) : type(type), count(count) {}

		/*~Payload() {
			std::cout << "Instance of Payload has been destroyed" << std::endl;
		}*/
	};

	//Defines this firework, the type, min and max age, min/max vel and damping
	struct FireworkRule {
		// will give a firework particle a rand age between min and max age
		// will spawn a payload particle at rand vel between min and max vel
		Vector3 minVelocity{};
		Vector3 maxVelocity{};
		float minAge;
		float maxAge;
		//

		unsigned int type;
		float damping;

		// declare as static so the same value is used throughout scopes
		unsigned int payloadCount;
		std::vector<Payload> payloads;

		void init(unsigned int payloadCount);

		//default constructor
		FireworkRule() : minVelocity{}, maxVelocity{}, type(UNUSED), minAge(0), maxAge(0), damping(1), payloadCount(0){}

		//constructor with passed parameters
		FireworkRule(unsigned int type, float minAge, float maxAge, float damping, const Vector3& minVelocity, const Vector3& maxVelocity, unsigned int payloadCount) :
			minVelocity(minVelocity), maxVelocity(maxVelocity),
			type(type), minAge(minAge), maxAge(maxAge),
			damping(damping), payloadCount(payloadCount){}

		/*~FireworkRule() {
			std::cout << "Instance of FireworkRule has been destroyed" << std::endl;
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

	static constexpr unsigned maxFireworks = 1024;


private:
	FireworkNode* root;

	float distance2(FireworkNode* node1, FireworkNode* node2);
	void findNNHelper(FireworkNode* current, FireworkNode* target, FireworkNode*& bestNode, float& distance, int depth);
	void addNode(FireworkNode* node);
	FireworkNode* allocateNode(FireworkParticle* fireworkParticle);

public:
	Firework();

	void initFireworkType(FireworkSizeType type);
	void initFireworkRules();
	void spawnFirework();

	void addFireworksFromVectorToTree(std::vector<FireworkParticle>& passedFireworks);
	void printByDepth();
	FireworkNode* getRoot();

	FireworkParticle singleFireWork; // used for testing
	FireworkSizeType currentFireworkType;
	std::vector<FireworkParticle> fireworks;
	std::vector<FireworkNode> nodepool;
	std::vector<Firework::FireworkRule> rules;
	int poolUsed;
	
};