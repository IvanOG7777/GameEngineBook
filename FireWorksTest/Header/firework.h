//
// Created by elder on 01/16/2026.
//

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <random>

#include "core.h"
#include "Particle.h"

class Firework {
public:

	// this enum will eventually handle how the effect will look like

	// size of firework particle
	enum BurstType {
		ROCKET = 0, // shape will be more concentrated vertically
		FOUNTAIN, // more spread out horizontally, not too much vertically
		FIREWORK, // shape will maybe be a mix of the two, maybe certain particles going up high then flowing horizontally
		SPHERE, // first works spawn in a circle around init firework
		COMMET, // initial particle will move acros the screen with small particles trailing behind then finally exploding
	};

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
	};

	//Defines this firework, the type, min and max age, min/max vel and damping
	struct FireworkRule {
		// will give a firework particle a rand age between min and max age
		// will spawn a payload particle at rand vel between min and max vel
		Vector3 minVelocity{};
		Vector3 maxVelocity{};
		float minAge;
		float maxAge;

		unsigned int type;
		float damping;

		float spawnBudget;

		// declare as static so the same value is used throughout scopes
		unsigned int payloadCount;
		std::vector<Payload> payloads;

		void init(unsigned int payloadCount);

		//default constructor
		FireworkRule() : minAge(0), maxAge(0), type(UNUSED), damping(1), spawnBudget(0), payloadCount(0){}
	};

	struct FireworkNode {
		std::shared_ptr<FireworkNode> left;
		std::shared_ptr<FireworkNode> right;
		Particle particle;
		float age;
		unsigned int type;
		std::string name;

		float emissionRate;
		float emissionInterval;
		float emissionAccumulator;
		int spawnedSoFar;
		bool hasBurst;

		FireworkNode() : left(nullptr), right(nullptr), age(0), type(UNUSED), emissionRate(0), emissionInterval(0), emissionAccumulator(0), spawnedSoFar(0), hasBurst(false) {}

		FireworkNode(const std::string &name) : left(nullptr), right(nullptr), age(0), type(UNUSED), name(name), emissionRate(0), emissionInterval(0), emissionAccumulator(0), spawnedSoFar(0), hasBurst(false) {}
	};

	static constexpr unsigned maxFireworks = 1024;


private:
	std::shared_ptr<FireworkNode> root;

	static void findNearestNeighborHelper(std::shared_ptr<FireworkNode>& current, std::shared_ptr<FireworkNode>& target,
	                                      std::shared_ptr<FireworkNode>& bestNode, float& bestDistance, int depth);

	static inline float distanceSquared(std::shared_ptr<FireworkNode>& node1, std::shared_ptr<FireworkNode>& node2);

public:
	Firework();
	~Firework() {
		std::cout << "Instance of firework class has been destroyed" << '\n';
	}

	void fireworkBurstRules(); // default use case for fireworks, returns nothing
	void rocketBurstRules();
	void fountainBurst();
	void sphereBurst();
	void commetBurst();


	void fireworkLoop(const float &dt);
	void rocketBurstLoop(const float &dt);

	void spawnFirework(int key); // function that spawns firework,  returns nothing
	void fire(FireworkSizeType type, const float& xPosition, const float& yPosition); // function to fire generic firework,  returns nothing
	void fireRocket(FireworkSizeType type, const float &xPosition,const float &yPosition); // function to fire a rocket burst,  returns nothing
	void updateFireworks(double dt); // function to update the firework particles, returns nothing

	std::shared_ptr<FireworkNode> allocateNode(unsigned int type); // function to allocate a firework node,  returns a shared pointer to the newly allocated node
	void addNode(std:: shared_ptr<FireworkNode> &node); // function to add node to tree, returns nothing
	void treeReset(); // function to reset the tree,  returns nothing
	void addFireworksFromVectorToTree(); // function to add fireworks from vector to the tree, returns nothing

	std:: weak_ptr<FireworkNode> findBestNode(std::shared_ptr<FireworkNode> &targetNode); // function used to find nearest node to target node,  returns weak pointer to best node
	inline FireworkNode* getRoot() const; // function used to get root of the tree,  returns a pointer to a FireworkNode


	FireworkSizeType currentFireworkType;
	std::vector<std::shared_ptr<FireworkNode>> activeFireworks;
	std::vector<FireworkRule> rules;
	int nodeCount;
	int activeNodeCount;
	float mousePositionX, mousePositionY;
	std:: mt19937 mtSeed;
	bool rebuildTree;
};