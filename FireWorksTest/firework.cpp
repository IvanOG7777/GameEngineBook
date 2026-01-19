//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <queue>

#include "firework.h"

Firework::Firework() {
	fireworks.resize(maxFireworks);
	nodepool.resize(maxFireworks);
	currentFireworkType = UNUSED;
	singleFireWork = FireworkParticle();
	root = nullptr;
	poolUsed = 0;
}


// since each particle will have its own amount of payloads maybye pass in the node to initlaize its payload amount?
void Firework::init(unsigned int payloadCount) {
	payloadCount = payloadCount;
	std::vector<Payload*> payloads;
	payloads.resize(payloadCount);
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

void Firework::addFireworksFromVectorToTree(std::vector<FireworkParticle>& passedFireworks) {
	if (passedFireworks.empty()) return;
	
	for (auto& val : passedFireworks) {
		if (val.type == UNUSED) continue;
		FireworkNode* newNode = allocateNode(&val);
		if (!newNode) break;

		addNode(newNode);
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

void Firework::initFireworkType(FireworkSizeType type) {

	int roundIndex = 0;

	for (; roundIndex < fireworks.size(); roundIndex++) {
		if (fireworks[roundIndex].type == UNUSED) break;
	}

	if (roundIndex >= fireworks.size()) {
		// error logs
		std::cout << "Firework rounds is full" << std::endl;
		std::cout << "Cant initalize any more rounds" << std::endl;
		std::cout << "roundIndex: " << roundIndex << std::endl;
		return;
	}

	switch (type) {
	case Firework::UNUSED:
		break;
	case Firework::SMALL:
		fireworks[roundIndex].type = SMALL;
		fireworks[roundIndex].age = 12.0f;
		fireworks[roundIndex].particle.setMass(2.0f);
		fireworks[roundIndex].particle.setVelocity(0.0f, 35.0f, 0.0f);
		fireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		fireworks[roundIndex].particle.setDamping(0.99f);
		fireworks[roundIndex].particle.setRadius(2.0f);
		fireworks[roundIndex].particle.setPosition(540.0f, 960.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to SMALL" << std::endl;
		break;
	case Firework::MEDIUM:
		fireworks[roundIndex].type = MEDIUM;
		fireworks[roundIndex].age = 8.0f;
		fireworks[roundIndex].particle.setMass(5.5f);
		fireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		fireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		fireworks[roundIndex].particle.setDamping(0.99f);
		fireworks[roundIndex].particle.setRadius(10.0f);
		fireworks[roundIndex].particle.setPosition(550.0f, 950.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to MEDIUM" << std::endl;
		break;
	case Firework::LARGE:
		fireworks[roundIndex].type = LARGE;
		fireworks[roundIndex].age = 6.0f;
		fireworks[roundIndex].particle.setMass(11.0f);
		fireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		fireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		fireworks[roundIndex].particle.setDamping(0.99f);
		fireworks[roundIndex].particle.setRadius(10.0f);
		fireworks[roundIndex].particle.setPosition(533.0f, 910.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to LARGE" << std::endl;
		break;
	case Firework::EXTRALARGE:
		fireworks[roundIndex].type = EXTRALARGE;
		fireworks[roundIndex].age = 4.0f;
		fireworks[roundIndex].particle.setMass(15.0f);
		fireworks[roundIndex].particle.setVelocity(0.0f, 30.0f, 0.0f);
		fireworks[roundIndex].particle.setAcceleration(0.0f, 35.0f, 0.0f);
		fireworks[roundIndex].particle.setDamping(0.99f);
		fireworks[roundIndex].particle.setRadius(12.5f);
		fireworks[roundIndex].particle.setPosition(542.0f, 912.0f, 0.0f);
		std::cout << "Fireworks at index " << roundIndex << " has been initalized to EXTRALARGE" << std::endl;
		break;
	default:
		break;
	}
}