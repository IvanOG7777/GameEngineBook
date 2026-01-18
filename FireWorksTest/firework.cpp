//
// Created by elder on 01/16/2026.
//

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
	if (node1 == nullptr) return;
	if (node2 == nullptr) return;

	float xSum = node1->fireworkNode->particle.getPosition().x + node2->fireworkNode->particle.getPosition().x;
	float ySum = node1->fireworkNode->particle.getPosition().y + node2->fireworkNode->particle.getPosition().y;

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

	findNNHelper(nearChild, target, bestNode, bestDistance, depth);

	float difference = targetAxisValue - currentAxisValue;
	float differenceSquared = difference * difference;

	if (differenceSquared < bestDistance) {
		findNNHelper(farChild, target, bestNode, bestDistance, depth);
	}
}

void Firework::addNode(FireworkNode* node) {
	node->left = nullptr;
	node->right = nullptr;

	if (root == nullptr) {
		root == node;
	}

	FireworkNode* current = nullptr;
	int depth = 0;

	while (true) {
		if (depth % 2 == 0) {
			if (node->fireworkNode->particle.getPosition().x > current->fireworkNode->particle.getPosition().x) {
				if (current->right == nullptr) {
					current->right = node;
				}
				current = current->right;
			}
			else if (node->fireworkNode->particle.getPosition().x < current->fireworkNode->particle.getPosition().x) {
				if (current->left == nullptr) {
					current->left = node;
				}
				current = current->left;
			}
		}
		else if (depth == 1) {
			if (node->fireworkNode->particle.getPosition().y > current->fireworkNode->particle.getPosition().y) {
				if (current->right == nullptr) {
					current->right = node;
				}
				current = current->right;
			}
			else if (node->fireworkNode->particle.getPosition().y < current->fireworkNode->particle.getPosition().y) {
				if (current->left == nullptr) {
					current->left = node;
				}
				current = current->left;
			}
		}
		depth++;
	}
}

Firework::FireworkNode Firework::allocateNode(FireworkParticle* fireworkParticle) {
	if (poolUsed >= nodepool.size()) return nullptr;

	FireworkNode *newNode = &nodepool[poolUsed++];

	newNode->fireworkNode = fireworkParticle;

	addNode(newNode);
}