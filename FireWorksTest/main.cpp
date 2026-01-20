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

	Firework::Payload extraLargePayload;
	Firework::Payload largePayload;
	Firework::Payload mediumPayload;
	Firework::Payload smallPayload;

	extraLargePayload.set(Firework::LARGE, 2);
	largePayload.set(Firework::MEDIUM, 4);
	mediumPayload.set(Firework::SMALL, 8);
	smallPayload.set(0,0);

	std::cout << std::endl;
	std::cout << "Payload for extra large is: " << std::endl;
	std::cout << "Type: " << ((extraLargePayload.type == Firework::LARGE) ? "LARGE" : "other type") << std::endl;
	std::cout << "Count: " << extraLargePayload.count << std::endl;
	std::cout << std::endl;

	std::cout << "Payload for large is: " << std::endl;
	std::cout << "Type: " << ((largePayload.type == Firework::MEDIUM) ? "MEDIUM" : "other type") << std::endl;
	std::cout << "Count: " << largePayload.count << std::endl;
	std::cout << std::endl;

	std::cout << "Payload for medium is: " << std::endl;
	std::cout << "Type: " << ((mediumPayload.type == Firework::SMALL) ? "SMALL" : "other type") << std::endl;
	std::cout << "Count: " << mediumPayload.count << std::endl;
	std::cout << std::endl;

	std::cout << "Payload for small is: " << std::endl;
	std::cout << "Type: " << ((smallPayload.type == Firework::UNUSED) ? "UNUSED" : "other type") << std::endl;
	std::cout << "Count: " << smallPayload.count << std::endl;

	/*firework.printByDepth();*/
}