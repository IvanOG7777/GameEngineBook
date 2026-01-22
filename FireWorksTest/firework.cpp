//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <queue>

#include "firework.h"

Firework::Firework() {
	activeFireworks.resize(maxFireworks);
	nodepool.resize(maxFireworks);
	currentFireworkType = UNUSED;
	singleFireWork = FireworkParticle();
	root = nullptr;
	poolUsed = 0;
}

void Firework::initFireworkType(FireworkSizeType type) {

	int roundIndex = 0;

	for (; roundIndex < activeFireworks.size(); roundIndex++) {
		if (activeFireworks[roundIndex].type == UNUSED) break;
	}

	if (roundIndex >= activeFireworks.size()) {
		// error logs
		std::cout << "Firework rounds is full" << std::endl;
		std::cout << "Cant initalize any more rounds" << std::endl;
		std::cout << "roundIndex: " << roundIndex << std::endl;
		return;
	}

	switch (type) {
	case Firework::UNUSED:
		break;
	case SMALL:
		activeFireworks[roundIndex].type = SMALL;
		activeFireworks[roundIndex].particle.setMass(2.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(2.0f);
		activeFireworks[roundIndex].particle.setPosition(540.0f, 960.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to SMALL" << std::endl;
		break;
	case MEDIUM:
		activeFireworks[roundIndex].type = MEDIUM;
		activeFireworks[roundIndex].particle.setMass(5.5f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(10.0f);
		activeFireworks[roundIndex].particle.setPosition(550.0f, 950.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to MEDIUM" << std::endl;
		break;
	case LARGE:
		activeFireworks[roundIndex].type = LARGE;
		activeFireworks[roundIndex].particle.setMass(11.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(10.0f);
		activeFireworks[roundIndex].particle.setPosition(533.0f, 910.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to LARGE" << std::endl;
		break;
	case EXTRALARGE:
		activeFireworks[roundIndex].type = EXTRALARGE;
		activeFireworks[roundIndex].particle.setMass(15.0f);
		activeFireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		activeFireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		activeFireworks[roundIndex].particle.setDamping(0.99f);
		activeFireworks[roundIndex].particle.setRadius(12.5f);
		activeFireworks[roundIndex].particle.setPosition(542.0f, 912.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to EXTRALARGE" << std::endl;
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
	rules[3].maxAge = 1.5f;
	rules[3].minAge = 0.5f;
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
		initFireworkType(currentFireworkType);
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



void Firework::findNNHelper(FireworkNode* current, FireworkNode* target, FireworkNode*& bestNode, float& bestDistance, int depth) {
	if (current == nullptr) return;

	float currentDistance = distance2(current, target);

	if (currentDistance > 0.0f && currentDistance < bestDistance) {
		bestDistance = currentDistance;
		bestNode = current;
	}

	int axis = depth % 2;

	float targetAxisValue = (axis == 0) ? target->fireworkNode->particle.getPosition().x : target->fireworkNode->particle.getPosition().y;
	float currentAxisValue = (axis == 0) ? current->fireworkNode->particle.getPosition().x : current->fireworkNode->particle.getPosition().y;

	FireworkNode* nearChild = (targetAxisValue < currentAxisValue) ? current->left : current->right;
	FireworkNode* farChild = (targetAxisValue < currentAxisValue) ? current->right : current->left;

	findNNHelper(nearChild, target, bestNode, bestDistance, depth+1);

	float difference = targetAxisValue - currentAxisValue;
	float differenceSquared = difference * difference;

	if (differenceSquared < bestDistance) {
		findNNHelper(farChild, target, bestNode, bestDistance, depth+1);
	}
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