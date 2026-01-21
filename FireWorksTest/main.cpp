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

	GLFWwindow *window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT, true);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n";
		return 1;
	}


	Firework firework;

	firework.currentFireworkType = firework.EXTRALARGE; firework.initFireworkType(firework.currentFireworkType);
	firework.currentFireworkType = firework.LARGE; firework.initFireworkType(firework.currentFireworkType);
	firework.currentFireworkType = firework.MEDIUM; firework.initFireworkType(firework.currentFireworkType);
	firework.currentFireworkType = firework.SMALL; firework.initFireworkType(firework.currentFireworkType);

	firework.addFireworksFromVectorToTree(firework.activeFireworks);

	Firework::Payload extraLargePayload;
	Firework::Payload largePayload;
	Firework::Payload mediumPayload;
	Firework::Payload smallPayload;

	firework.initFireworkRules();

	for (auto& rule : firework.rules) {
		std::cout << "Rule type: " << rule.type << std::endl;
		std::cout << "Payload type: ";
		for (auto& payload : rule.payloads) {
			std::cout << payload.type << " ";
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}

	std::random_device gen;

	for (auto& firework : firework.activeFireworks) {
		std::cout << "Type: " << firework.type << std::endl;
	}

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
			glfwTerminate();
			return 0;
		}

		fWasDown = fDown;


		for (auto& particle : firework.activeFireworks) {
			if (particle.type == Firework::UNUSED) continue;

			float minAge = firework.rules[particle.type].minAge;
			float maxAge = firework.rules[particle.type].maxAge;

			if (particle.age <= 0.0f) {
				std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);
				particle.age = ageDistribution(gen);
			}
		}

		for (auto& particle : firework.activeFireworks) {
			if (particle.type == Firework::UNUSED) continue;

			std::cout << "Type: " << particle.type << std::endl;
			std::cout << "Age: " << particle.age << std::endl;

			std::cout << std::endl;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*
	float minAge = 0, maxAge = 0;
	minAge = firework.rules[fireworkParticle.type].minAge;
	maxAge = firework.rules[fireworkParticle.type].maxAge;
		
	std::random_device gen;
	std::uniform_real_distribution ageDistribution(minAge, maxAge);

	float randAge = ageDistribution(gen);

	fireworkParticle.age = randAge;

	
	while (fireworkParticle.age >= 0.0f) {
		std::cout << "Age: " << fireworkParticle.age << std::endl;
		fireworkParticle.age -= testDT;

		if (fireworkParticle.age <= 0) {
			std::cout << "EXTRALARGE particle has died" << std::endl;
			break;
		}
	}*/

	
	//while (!acticeParticles.empty()) {

	//	for (size_t i = 0; i < acticeParticles.size();) {
	//		if (acticeParticles[i].type == Firework::UNUSED) {
	//			acticeParticles.erase(acticeParticles.begin() + i);
	//			continue;
	//		}

	//		float minAge = firework.rules[acticeParticles[i].type].minAge;
	//		float maxAge = firework.rules[acticeParticles[i].type].maxAge;

	//		Vector3 minVel = firework.rules[acticeParticles[i].type].minVelocity;
	//		Vector3 maxVel = firework.rules[acticeParticles[i].type].maxVelocity;

	//		if (acticeParticles[i].age <= 0.0f) {
	//			std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);
	//			acticeParticles[i].age = ageDistribution(gen);
	//		}
	//		
	//		acticeParticles[i].age -= testDT;
	//		std::cout << "Active Age: " << acticeParticles[i].age << std::endl;

	//		if (acticeParticles[i].age <= 0.0f) {
	//			for (size_t j = 0; j < firework.rules[acticeParticles[i].type].payloads.size(); j++) {

	//				Firework::FireworkParticle newParticle;
	//				newParticle.type = firework.rules[acticeParticles[i].type].payloads[j].type;
	//				acticeParticles.push_back(newParticle);
	//			}
	//			acticeParticles[i].type = Firework::UNUSED;
	//		}

	//		std::cout << acticeParticles[i].type << std::endl;
	//		i++;
	//	}

	//	if (acticeParticles.empty()) std::cout << "No more active particles" << std::endl;
	//}

	/*firework.printByDepth();*/

	return 0;
}