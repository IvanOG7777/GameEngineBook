//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <random>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "firework.h"
#include "globalConstants.h"
#include "windowFunctions.h"

int main() {

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT, false);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n";
		return 1;
	}


	Firework firework;

	firework.currentFireworkType = firework.EXTRALARGE; firework.initFireworkType(firework.currentFireworkType);

	firework.addFireworksFromVectorToTree(firework.activeFireworks);

	Firework::Payload extraLargePayload;
	Firework::Payload largePayload;
	Firework::Payload mediumPayload;
	Firework::Payload smallPayload;

	firework.initFireworkRules();

	std::random_device gen;

	/*for (auto& fireworkParticle : firework.activeFireworks) {
		if (fireworkParticle.type == Firework::SMALL) continue;
		if (fireworkParticle.type == Firework::UNUSED) continue;
		std::string parentName = "";

		switch (fireworkParticle.type) {
			case Firework:: EXTRALARGE:
				parentName = "EXTRA LARGE";
				break;
			case Firework::LARGE:
				parentName = "LARGE";
				break;
			case Firework::MEDIUM:
				parentName = "MEDIUM";
				break;
			default:
				break;
		}

		std::cout << "Parent Type: " << parentName << std::endl;
		for (size_t i = 0; i < firework.rules[fireworkParticle.type].payloads.size(); i++) {
			for (size_t j = 0; j < firework.rules[fireworkParticle.type].payloads[i].count; j++) {
				int currentType = firework.rules[fireworkParticle.type].payloads[i].type;
				firework.initFireworkType(static_cast<Firework::FireworkSizeType>(currentType));
			}
		}
	}*/

	bool escWasDown = false;
	bool fWasDown = false;
	
	while (!glfwWindowShouldClose(window)) {

		int w = SCREENWIDTH;
		int h = SCREENHEIGHT;

		glfwGetFramebufferSize(window, &w, &h);
		glClear(GL_COLOR_BUFFER_BIT);

		bool escDown = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
		bool fDown = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

		if (fDown && !fWasDown) {
			std::cout << "F key was hit" << std::endl;
			firework.spawnFirework(GLFW_KEY_F);
		}

		if (escDown && !escWasDown) {
			std::cout << "Program has been killed " << std::endl;
			glfwSetWindowShouldClose(window, true);
		}

		fWasDown = fDown;
		escWasDown = escDown;


		for (auto& particle : firework.activeFireworks) {
			if (particle.type == Firework::UNUSED) continue;
			std::string parentName = "";

			switch (particle.type) {
				case Firework::EXTRALARGE:
					parentName = "EXTRALARGE";
					break;
				case Firework::LARGE:
					parentName = "LARGE";
					break;
				case Firework::MEDIUM:
					parentName = "MEDIUM";
					break;
				case Firework::SMALL:
					parentName = "SMALL";
					break;
				default:
					break;
			}

			float minAge = firework.rules[particle.type].minAge;
			float maxAge = firework.rules[particle.type].maxAge;

			if (particle.age <= 0.0f) {
				std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);
				particle.age = ageDistribution(gen);
			}

			particle.age -= testDT;


			if (particle.age <= 0.0f) {
				for (size_t i = 0; i < firework.rules[particle.type].payloads.size(); i++) {
					for (size_t j = 0; j < firework.rules[particle.type].payloads[i].count; j++) {
						int currentType = firework.rules[particle.type].payloads[i].type;
						firework.initFireworkType(static_cast<Firework::FireworkSizeType>(currentType));
					}
				}

				std::cout << "Parent Name: " << parentName << ", Age: " << particle.age << std::endl;
				particle.type = Firework::UNUSED;
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	for (auto& particle : firework.activeFireworks) {
		std::cout << "Particle Type: " << particle.type << std::endl;
	}

	return 0;
}