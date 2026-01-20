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

int main() {
	Firework firework;

	firework.initFireworkType(Firework::EXTRALARGE);
	firework.initFireworkType(Firework::LARGE);
	firework.initFireworkType(Firework::MEDIUM);
	firework.initFireworkType(Firework::SMALL);
	firework.initFireworkType(Firework::SMALL);

	firework.addFireworksFromVectorToTree(firework.fireworks);

	Firework::Payload extraLargePayload;
	Firework::Payload largePayload;
	Firework::Payload mediumPayload;
	Firework::Payload smallPayload;

	//std::vector<Firework::FireworkRule> rules;
	//rules.resize(4); // resize to 4 becasue we have4 different types of rules to set EXLARGE, LARGE, MEDIUM, SMALL

	//extraLargePayload.set(Firework::LARGE, 2);
	//largePayload.set(Firework::MEDIUM, 4);
	//mediumPayload.set(Firework::SMALL, 8);
	//smallPayload.set(0,0);

	//std::cout << std::endl;
	//std::cout << "Payload for extra large is: " << std::endl;
	//std::cout << "Type: " << ((extraLargePayload.type == Firework::LARGE) ? "LARGE" : "other type") << std::endl;
	//std::cout << "Count: " << extraLargePayload.count << std::endl;
	//std::cout << std::endl;

	//std::cout << "Payload for large is: " << std::endl;
	//std::cout << "Type: " << ((largePayload.type == Firework::MEDIUM) ? "MEDIUM" : "other type") << std::endl;
	//std::cout << "Count: " << largePayload.count << std::endl;
	//std::cout << std::endl;

	//std::cout << "Payload for medium is: " << std::endl;
	//std::cout << "Type: " << ((mediumPayload.type == Firework::SMALL) ? "SMALL" : "other type") << std::endl;
	//std::cout << "Count: " << mediumPayload.count << std::endl;
	//std::cout << std::endl;

	//std::cout << "Payload for small is: " << std::endl;
	//std::cout << "Type: " << ((smallPayload.type == Firework::UNUSED) ? "UNUSED" : "other type") << std::endl;
	//std::cout << "Count: " << smallPayload.count << std::endl;
	//std::cout << std::endl;

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

	Firework::FireworkParticle fireworkParticle;
	fireworkParticle.type = Firework::EXTRALARGE;
	std::vector< Firework::FireworkParticle> acticeParticles;
	acticeParticles.reserve(50);

	acticeParticles.push_back(fireworkParticle);

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

	std::random_device gen;

	
	while (!acticeParticles.empty()) {

		for (size_t i = 0; i < acticeParticles.size();) {
			if (acticeParticles[i].type == Firework::UNUSED) {
				acticeParticles.erase(acticeParticles.begin() + i);
				continue;
			}

			float minAge = firework.rules[acticeParticles[i].type].minAge;
			float maxAge = firework.rules[acticeParticles[i].type].maxAge;

			Vector3 minVel = firework.rules[acticeParticles[i].type].minVelocity;
			Vector3 maxVel = firework.rules[acticeParticles[i].type].maxVelocity;

			if (acticeParticles[i].age <= 0.0f) {
				std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);
				acticeParticles[i].age = ageDistribution(gen);
			}
			
			acticeParticles[i].age -= testDT;
			std::cout << "Active Age: " << acticeParticles[i].age << std::endl;

			if (acticeParticles[i].age <= 0.0f) {
				for (size_t j = 0; j < firework.rules[acticeParticles[i].type].payloads.size(); j++) {

					Firework::FireworkParticle newParticle;
					newParticle.type = firework.rules[acticeParticles[i].type].payloads[j].type;
					acticeParticles.push_back(newParticle);
				}
				acticeParticles[i].type = Firework::UNUSED;
			}

			std::cout << acticeParticles[i].type << std::endl;
			i++;
		}

		if (acticeParticles.empty()) std::cout << "No more active particles" << std::endl;
	}

	/*firework.printByDepth();*/

	return 0;
}