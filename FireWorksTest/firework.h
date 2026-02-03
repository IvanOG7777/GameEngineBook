//
// Created by elder on 01/16/2026.
//

#pragma once

#include <iostream>
#include <vector>
#include <memory>

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


		/*~FireworkRule() {
			std::cout << "Instance of FireworkRule has been destroyed" << std::endl;
		}*/
	};

	struct FireworkNode {
		std::shared_ptr<FireworkNode> left;
		std::shared_ptr<FireworkNode> right;
		Particle particle;
		float age;
		unsigned int type;
		std::string name;

		FireworkNode() : left(nullptr), right(nullptr), age(0), type(UNUSED) {}

		FireworkNode(std::string &name) : left(nullptr), right(nullptr), age(0), type(UNUSED), name(name) {
			std::cout << name << " has been created" << '\n';
		}

		~FireworkNode() {
			std::cout << name << " has been destroyed" << '\n';
		}

	};

	static constexpr unsigned maxFireworks = 1024;


private:
	std::shared_ptr<FireworkNode> root;

	void findNearestNeighborHelper(std::weak_ptr<FireworkNode>& current, std::weak_ptr<FireworkNode>& target,
		std::weak_ptr<FireworkNode>& bestNode, float& bestDistance, int depth);

	float distance2(std::weak_ptr<FireworkNode>& node1, std::weak_ptr<FireworkNode>& node2);

public:
	Firework();
	~Firework() {
		std::cout << "Instance of firework class has been destroyed" << '\n';
	}

	void initFireworkRules();
	void spawnFirework(int key);
	void fire(FireworkSizeType type, double& xPosition, double& yPosition);
	void updateFireworks(double dt);

	std::shared_ptr<FireworkNode> allocateNode(unsigned int type);
	void addNode(std:: shared_ptr<FireworkNode> &node);
	void treeReset();
	void addFireworksFromVectorToTree();

	std:: weak_ptr<FireworkNode> findBestNode(std::shared_ptr<FireworkNode> &targetNode);
	FireworkNode* getRoot();

	void printByDepth();


	FireworkSizeType currentFireworkType;
	std::vector<std::shared_ptr<FireworkNode>> activeFireworks;
	std::vector<Firework::FireworkRule> rules;
	int nodeCount;
	int activeNodeCount;
	//FireworkNode singleFireWork; // used for testing
	double mousePositionX, mousePositionY;	
};