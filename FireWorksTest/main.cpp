//
// Created by elder on 01/16/2026.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "firework.h"

int main() {
	Firework firework;

	firework.initFireworkType(Firework::EXTRALARGE);
	firework.initFireworkType(Firework::LARGE);
	firework.initFireworkType(Firework::MEDIUM);
	firework.initFireworkType(Firework::SMALL);
	firework.initFireworkType(Firework::SMALL);

	firework.addFireworksFromVectorToTree(firework.fireworks);

	Firework::FireworkNode* root = firework.getRoot();

	std::cout << root->fireworkNode->type << std::endl;
	std::cout << root->left->fireworkNode->type << std::endl;
	std::cout << root->right->fireworkNode->type << std::endl;

	firework.printByDepth();
}