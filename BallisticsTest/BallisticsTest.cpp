#include <iostream>
#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "ballistics.h"
#include "windowFunctions.h"
#include "objects.h"
#include "globalConstants.h"

const char* cirlceVertex = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform vec2 uResolution;

        void main() {
            vec2 ndc = aPos.xy / uResolution;
            ndc = ndc * 2.0 - 1.0;
            

            gl_Position = vec4(ndc, 0.0, 1.0);
        }
    )GLSL";

const char* circleFragment = R"GLSL(
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


	GLFWwindow* window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT, true); // returns an addres in momory for the window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD\n";
		return 1;
	}

	GLuint program = createProgram(cirlceVertex, circleFragment);
	std::cout << "Program: " << program << std::endl;
	if (!program) return 1;
	glUseProgram(program);

	GLint uResolutionLoc = glGetUniformLocation(program, "uResolution");
	GLint uColorLoc = glGetUniformLocation(program, "uColor");

	GLuint vao = 0, vbo = 0;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Ballistic ballistic;
	int key;
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

	if (uResolutionLoc == -1 || uColorLoc == -1) {
		std::cerr << "Uniform Resoluction or Color was not found or optimized" << std::endl;
		return 1;
	}

	ballistic.currentShotType = ballistic.PISTOL; ballistic.fire();
	ballistic.currentShotType = ballistic.ARTILLERY; ballistic.fire();
	ballistic.currentShotType = ballistic.FIREBALL; ballistic.fire();
	ballistic.currentShotType = ballistic.LASER; ballistic.fire();

	ballistic.addNodesFromVectorToTree(ballistic.rounds);
	
	std::vector<Ballistic::BallisticNode*> bestNodes = ballistic.findMultipleNN(&ballistic.nodePool[0], 4);

	if (bestNodes.empty()) std::cout << "Best Nodes is empty" << std::endl;

	ballistic.addNodesFromVectorToTree(ballistic.rounds);



	std::cout << "Nearest Nodes to rounds at index 0: ";
	ballistic.rounds[0].particle.printPosition();

	std::cout << "Size of bestNodes is: " << bestNodes.size() << std::endl;

	for (auto& node : bestNodes) {
		if (node == nullptr) {
			std::cout << "nullptr" << std::endl;
			continue;
		}
		node->roundNode->particle.printPosition();
	}

	//bool pWasDown = false;
	//bool aWasDown = false;
	//bool fWasDown = false;
	//bool escWasDown = false;
	//double maxDt = 1.0 / 180.0;
	//auto start = std::chrono::high_resolution_clock::now();
	//while (!glfwWindowShouldClose(window)) {
	//	auto currentTime = std::chrono::high_resolution_clock::now();
	//	std::chrono::duration<double> deltaTime = currentTime - start;
	//	start = currentTime;
	//	double dt = deltaTime.count(); // seconds
	//	if (dt > maxDt) dt = maxDt;

	//	int w = SCREENWIDTH;
	//	int h = SCREENHEIGHT;

	//	glfwGetFramebufferSize(window, &w, &h);
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	glUseProgram(program);
	//	glUniform2f(uResolutionLoc, (float)w, (float)h);
	//	glBindVertexArray(vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//	bool pDown = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
	//	bool aDown = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	//	bool fDown = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
	//	bool escDown = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

	//	if (pDown && !pWasDown) {
	//		ballistic.spawnRound(GLFW_KEY_P);
	//	}

	//	if (aDown && !aWasDown) {
	//		ballistic.spawnRound(GLFW_KEY_A);
	//	}

	//	if (fDown && !fWasDown) {
	//		ballistic.spawnRound(GLFW_KEY_F);
	//	}

	//	if (escDown && !escWasDown) {
	//		std::cout << "Program has been killed " << std::endl;
	//		glDeleteBuffers(1, &vbo);
	//		glDeleteVertexArrays(1, &vao);
	//		glDeleteProgram(program);

	//		glfwTerminate();
	//		return 0;
	//	}

	//	pWasDown = pDown;
	//	aWasDown = aDown;
	//	fWasDown = fDown;

	//	for (int i = 0; i < ballistic.rounds.size(); i++) {
	//		if (ballistic.rounds[i].type == Ballistic::UNUSED) continue;
	//		float particleForceNewtons = SMALL_GRAVITY / ballistic.rounds[i].particle.getInverseMass();
	//		ballistic.rounds[i].particle.addForce(0, particleForceNewtons, 0);
	//	}

	//	ballistic.updateRound(dt);
	//	ballistic.treeReset();
	//	ballistic.addNodesFromVectorToTree(ballistic.rounds);
	//	resolveCollisionKDTree(ballistic, ballistic.rounds);

	//	for (int i = 0; i < ballistic.rounds.size(); i++) {
	//		if (ballistic.rounds[i].type == Ballistic::UNUSED) continue;

	//		float particleRadius = ballistic.rounds[i].particle.getRadius();
	//		sweptBounds(ballistic.rounds[i].particle, dt, w, h);

	//		Vector3 particlePosition = ballistic.rounds[i].particle.getPosition();
	//		particleVerticies = makeCircleFan(particlePosition, particleRadius, res);

	//		switch (ballistic.rounds[i].type) {
	//		case Ballistic::PISTOL: glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f); break;
	//		case Ballistic::ARTILLERY: glUniform3f(uColorLoc, 1.0f, 0.8f, 0.2f); break;
	//		case Ballistic::FIREBALL:  glUniform3f(uColorLoc, 1.0f, 0.2f, 0.2f); break;
	//		default:                   glUniform3f(uColorLoc, 0.6f, 0.6f, 0.6f); break;
	//		}

	//		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//		glBufferSubData(
	//			GL_ARRAY_BUFFER,
	//			0,
	//			particleVerticies.size() * sizeof(Vector3),
	//			particleVerticies.data()
	//		);

	//		glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)particleVerticies.size());

	//	}

	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	//glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &vao);
	//glDeleteProgram(program);

	glfwTerminate();
	return 0;
}