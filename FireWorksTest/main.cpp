//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <random>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "firework.h"
#include "globalConstants.h"
#include "windowFunctions.h"
#include "objects.h"

const char* vertexShader = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform vec2 uResolution;

        void main() {
            vec2 ndc = aPos.xy / uResolution;
            ndc = ndc * 2.0 - 1.0;
            

            gl_Position = vec4(ndc, 0.0, 1.0);
        }
    )GLSL";

const char* fragmentShader = R"GLSL(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )GLSL";

int main() {

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT, true);

	glfwSetCursorPosCallback(window, cursorPositionCallback);

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n";
		return 1;
	}


	Firework firework;
	glfwSetWindowUserPointer(window, &firework);

	firework.currentFireworkType = firework.EXTRALARGE; firework.fire(firework.currentFireworkType, firework.mousePositionX, firework.mousePositionY);

	firework.addFireworksFromVectorToTree(firework.activeFireworks);
	firework.initFireworkRules();

	GLuint program = createProgram(vertexShader, fragmentShader);
	if (!program) return 1;
	glUseProgram(program);

	GLuint uResolutionLoc = glGetUniformLocation(program, "uResolution");
	GLuint uColorLoc = glGetUniformLocation(program, "uColor");

	GLuint vao = 0, vbo = 0;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int res = 100;
	float baseRadius = 10.0f;
	Vector3 basePosition;
	std::vector<Vector3> particleVerticies = makeCircleFan(basePosition, baseRadius, res);

	glBufferData(
		GL_ARRAY_BUFFER,
		particleVerticies.size() * sizeof(Vector3),
		particleVerticies.data(),
		GL_DYNAMIC_DRAW
	);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vector3),
		(void*)0
	);


	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	std::random_device gen;
	bool escWasDown = false;
	bool fWasDown = false;

	auto startTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)) {

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> deltaTime = currentTime - startTime;
		startTime = currentTime;

		double dt = deltaTime.count();

		int w = SCREENWIDTH;
		int h = SCREENHEIGHT;

		glfwGetFramebufferSize(window, &w, &h);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glUniform2f(uResolutionLoc, (float)w, (float)h);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

		firework.updateFireworks(dt);

		for (size_t i = 0; i < firework.activeFireworks.size(); i++) {
			Firework::FireworkParticle &particle = firework.activeFireworks[i];
			if (particle.type == Firework::UNUSED) continue;

			keepCircleInFrame(particle.particle, w, h);

			float minAge = firework.rules[particle.type].minAge;
			float maxAge = firework.rules[particle.type].maxAge;

			if (particle.age <= 0.0f) {
				std::uniform_real_distribution<float> ageDistribution(minAge, maxAge);
				particle.age = ageDistribution(gen);
			}

			particle.age -= dt;


			if (particle.age <= 0.0f) {
				for (size_t i = 0; i < firework.rules[particle.type].payloads.size(); i++) {
					for (size_t j = 0; j < firework.rules[particle.type].payloads[i].count; j++) {
						int currentType = firework.rules[particle.type].payloads[i].type;
						firework.fire(static_cast<Firework::FireworkSizeType>(currentType), firework.mousePositionX, firework.mousePositionY);
					}
				}
				particle.type = Firework::UNUSED;
			}

			float particleRadius = firework.activeFireworks[i].particle.getRadius();
			Vector3 particlePosition = firework.activeFireworks[i].particle.getPosition();
			particleVerticies = makeCircleFan(particlePosition, particleRadius, res);
			switch (firework.activeFireworks[i].type) {
			case Firework::SMALL: glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f); break;
			case Firework::EXTRALARGE: glUniform3f(uColorLoc, 1.0f, 0.8f, 0.2f); break;
			case Firework::LARGE:  glUniform3f(uColorLoc, 1.0f, 0.2f, 0.2f); break;
			case Firework::MEDIUM: glUniform3f(uColorLoc, 0.6f, 0.3f, 0.5f); break;
			default:                   glUniform3f(uColorLoc, 0.6f, 0.6f, 0.6f); break;
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(
				GL_ARRAY_BUFFER,
				0,
				particleVerticies.size() * sizeof(Vector3),
				particleVerticies.data()
			);

			glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)particleVerticies.size());
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