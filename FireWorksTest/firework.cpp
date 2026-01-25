//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <queue>
#include <limits>

#include "firework.h"

Firework::Firework() {
	activeFireworks.reserve(maxFireworks);
	activeFireworks.resize(maxFireworks);
	nodepool.reserve(maxFireworks);
	nodepool.resize(maxFireworks);
	currentFireworkType = UNUSED;
	singleFireWork = FireworkParticle();
	root = nullptr;
	poolUsed = 0;
	mousePositionX = 0.0;
	mousePositionY = 0.0;
}

void Firework:: fire(FireworkSizeType type, double &xPosition, double &yPosition) {

	size_t roundIndex = 0;
	for (; roundIndex < activeFireworks.size(); roundIndex++) {
		if (activeFireworks[roundIndex].type == UNUSED) break;
	}

	if (roundIndex >= activeFireworks.size()) {
		// error logs
		std::cout << "Rounds is full" << std::endl;
		std::cout << "Cant initalize any more fireworks" << std::endl;
		std::cout << "roundIndex: " << roundIndex << std::endl;
		return;
	}

	switch (type) {
	case Firework::UNUSED:
		break;
	case SMALL:
		activeFireworks[roundIndex].type = type;
		activeFireworks[roundIndex].particle.setMass(2.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(2.0f);
		activeFireworks[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to SMALL\n";
		break;
	case MEDIUM:
		activeFireworks[roundIndex].type = type;
		activeFireworks[roundIndex].particle.setMass(5.5f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(10.0f);
		activeFireworks[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to MEDIUM\n";
		break;
	case LARGE:
		activeFireworks[roundIndex].type = type;
		activeFireworks[roundIndex].particle.setMass(11.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(10.0f);
		activeFireworks[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to LARGE \n";
		break;
	case EXTRALARGE:
		activeFireworks[roundIndex].type = type;
		activeFireworks[roundIndex].particle.setMass(15.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(12.5f);
		activeFireworks[roundIndex].particle.setPosition(static_cast<float>(xPosition), static_cast<float>(yPosition), 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to EXTRALARGE \n";
		break;
	default:
		break;
	}
}

// since each particle will have its own amount of payloads maybye pass in the node to initlaize its payload amount?
void Firework::FireworkRule::init(unsigned int payloadCount) {
	FireworkRule::payloadCount = payloadCount;
	payloads.clear();
	FireworkRule::payloads.resize(payloadCount);
}


/*
* initialization of rules for the particles
* 
* Parameters: NONE
* return type void
* 
* Function only initalizes the the rules for specific particles values
*/
// for now particles will expand into a coneish shape, later will implement 
void Firework::initFireworkRules() {
	rules.clear();
	rules.resize(5);

	rules[0].type = UNUSED;
	rules[0].damping = 1.0f;
	rules[0].maxAge = 0.0f;
	rules[0].minAge = 0.0f;
	rules[0].maxVelocity = { 0.0f, 0.0f, 0.0f };
	rules[0].minVelocity = { 0.0f, 00.0f, 0.0f };
	rules[0].init(0);

	rules[1].type = SMALL;
	rules[1].damping = 0.99f;
	rules[1].maxAge = 1.5f;
	rules[1].minAge = 0.5f;
	rules[1].maxVelocity = { 115.0f, 40.0f, 0.0f };
	rules[1].minVelocity = { -115.0f, -40.0f, 0.0f };
	rules[1].init(0);

	rules[2].type = MEDIUM;
	rules[2].damping = 0.99f;
	rules[2].maxAge = 1.5f;
	rules[2].minAge = 0.5f;
	rules[2].maxVelocity = { 80.0f, 50.0f, 0.0f };
	rules[2].minVelocity = { -80.0f, -10.0f, 0.0f };
	rules[2].init(1);
	rules[2].payloads[0].set(Firework::SMALL, 8);

	rules[3].type = LARGE;
	rules[3].damping = 0.99f;
	rules[3].maxAge = 5.5f;
	rules[3].minAge = 3.5f;
	rules[3].maxVelocity = { 40.0f, 75.0f, 0.0f };
	rules[3].minVelocity = { 40.0f, 20.0f, 0.0f };
	rules[3].init(1);
	rules[3].payloads[0].set(Firework::MEDIUM, 4);

	rules[4].type = EXTRALARGE;
	rules[4].damping = 0.99f;
	rules[4].maxAge = 1.5f;
	rules[4].minAge = 0.5f;
	rules[4].maxVelocity = { 5.0f, 90.0f, 0.0f };
	rules[4].minVelocity = { -5.0f, 60.0f, 0.0f };
	rules[4].init(2);
	rules[4].payloads[0].set(Firework::LARGE, 2);
	rules[4].payloads[1].set(Firework::MEDIUM, 4);

}


void Firework::spawnFirework(int key) {
	FireworkParticle firework;

	// Spawn Extra large key F
	if (key == 70) {
		currentFireworkType = EXTRALARGE;
		fire(currentFireworkType, mousePositionX, mousePositionY);
	}
}

// function used to update particles position/velocity/acceleration 
void Firework:: updateFireworks(double dt) {
	for (auto& fireworkParticle : activeFireworks) {
		if (fireworkParticle.type == UNUSED) continue;

		fireworkParticle.particle.update(dt);
		if (fireworkParticle.particle.getPosition().y <= 0.0f) {
			fireworkParticle.particle.clearAccumulator();
			fireworkParticle.particle.clearAllValues();
			fireworkParticle.type = UNUSED;
		}
	}
}

void Firework::addFireworksFromVectorToTree(std::vector<FireworkParticle>& passedFireworks) {
	if (passedFireworks.empty()) return;

	for (auto& val : passedFireworks) {
		if (val.type == UNUSED) continue;
		FireworkNode* newNode = allocateNode(&val);
		if (!newNode) break;

		addNode(newNode);
	}
}

void Firework::resetTree() {
	root = nullptr;
	poolUsed = 0;
}


void Firework::findBestNodeHelper(FireworkNode* current, FireworkNode* target, FireworkNode*& bestNode, float &bestDistance, int depth) {

	if (current == nullptr) {
		return;
	}

	float currentDistance = distance2(current, target);

	if (currentDistance > 0.0f && currentDistance < bestDistance) {
		bestDistance = currentDistance;
		bestNode = current;
	}

	float targetAxisValue = (depth % 2 == 0) ? target->fireworkNode->particle.getPosition().x : target->fireworkNode->particle.getPosition().y;
	float currentAxisValue = (depth % 2 == 0) ? current->fireworkNode->particle.getPosition().x : current->fireworkNode->particle.getPosition().y;

	FireworkNode *nearChild = (targetAxisValue < currentAxisValue) ? current->left : current->right;
	FireworkNode *farChild = (targetAxisValue < currentAxisValue) ? current->right : current->left;

	findBestNodeHelper(nearChild, target, bestNode, bestDistance, depth + 1);

	float difference = targetAxisValue - currentAxisValue;
	float differenceSquared = difference * difference;

	if (differenceSquared < bestDistance) {
		findBestNodeHelper(farChild, target, bestNode, bestDistance, depth + 1);
	}
}

Firework::FireworkNode *Firework::findBestNode(FireworkNode* target) {

	if (root == nullptr) {
		std::cout << "No nodes returning target" << std::endl;
		return target;
	}

	float bestDistance = std::numeric_limits<float>::infinity();
	FireworkNode* bestNode = root;

	findBestNodeHelper(root, target, bestNode, bestDistance, 0);

	return bestNode;
}


void Firework::printByDepth() {
	if (root == nullptr) {
		std::cerr << "Tree is empty!" << std::endl;
		return;
	}

	int depth = 0;
	std::queue<FireworkNode*> nodeQueue;
	nodeQueue.push(root);

	while (!nodeQueue.empty()) {
		size_t length = nodeQueue.size();
		std::cout << "Depth: " << depth << std::endl;
		for (size_t i = 0; i < length; i++) {
			if (nodeQueue.front()->left != nullptr) {
				nodeQueue.push(nodeQueue.front()->left);
			}

			if (nodeQueue.front()->right != nullptr) {
				nodeQueue.push(nodeQueue.front()->right);
			};
			std::cout << "	";
			nodeQueue.front()->fireworkNode->particle.printPosition();
			nodeQueue.pop();
		}
		depth++;
		std::cout << std::endl;
	}
}

float Firework::distance2(FireworkNode* node1, FireworkNode* node2) {

	float xSum = node1->fireworkNode->particle.getPosition().x - node2->fireworkNode->particle.getPosition().x;
	float ySum = node1->fireworkNode->particle.getPosition().y - node2->fireworkNode->particle.getPosition().y;

	float xSumSquared = xSum * xSum;
	float ySumSquared = ySum * ySum;

	return xSumSquared + ySumSquared;
}

void Firework::addNode(FireworkNode* node) {

	node->left = nullptr;
	node->right = nullptr;

	if (root == nullptr) {
		root = node;
		return;
	}

	FireworkNode* current = root;
	int depth = 0;

	while (true) {
		if (depth % 2 == 0) {
			if (node->fireworkNode->particle.getPosition().x >= current->fireworkNode->particle.getPosition().x) {
				if (current->right == nullptr) {
					current->right = node;
					return;
				}
				current = current->right;
			}
			else if (node->fireworkNode->particle.getPosition().x <= current->fireworkNode->particle.getPosition().x) {
				if (current->left == nullptr) {
					current->left = node;
					return;
				}
				current = current->left;
			}
		}
		else if (depth % 2 == 1) {
			if (node->fireworkNode->particle.getPosition().y >= current->fireworkNode->particle.getPosition().y) {
				if (current->right == nullptr) {
					current->right = node;
					return;
				}
				current = current->right;
			}
			else if (node->fireworkNode->particle.getPosition().y <= current->fireworkNode->particle.getPosition().y) {
				if (current->left == nullptr) {
					current->left = node;
					return;
				}
				current = current->left;
			}
		}
		depth++;
	}
}


Firework::FireworkNode* Firework::allocateNode(FireworkParticle* fireworkParticle) {
	if (poolUsed >= nodepool.size()) return nullptr;

	FireworkNode* newNode = &nodepool[poolUsed++];

	newNode->fireworkNode = fireworkParticle;
	newNode->left = nullptr;
	newNode->right = nullptr;

	return newNode;
}

Firework::FireworkNode* Firework::getRoot() {
	return root;
}