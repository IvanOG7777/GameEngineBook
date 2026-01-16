#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"

struct FireWorkRule {
	Particle fireWorkParticle;
	unsigned int type;
	float minAge;
	float maxAge;
};

class Firework {
private:

public:
};
int main() {

	std::cout << "sizeof(particle): " << sizeof(Particle) << std::endl;
	std::cout << "sizeof(firework): " << sizeof(Firework) << std::endl;
	std::cout << "sizeof(FireworkRule) = " << sizeof(FireWorkRule) << "\n";
	std:: cout	<< "alignof(FireworkRule) = " << alignof(FireWorkRule) << "\n";
}