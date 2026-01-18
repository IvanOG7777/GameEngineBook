//
// Created by elder on 01/16/2026.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "firework.h"

int main() {
	Particle particle;
	particle.setPosition(10, 10, 10);

	std::cout << "Particle should exitist" << std::endl;
	particle.printPosition();

	Firework firework;

}