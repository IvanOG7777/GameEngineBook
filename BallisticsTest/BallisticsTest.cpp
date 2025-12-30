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


    GLFWwindow* window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT); // returns an addres in momory for the window
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


    auto start = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)){
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = currentTime - start;
        start = currentTime;
        double dt = deltaTime.count(); // seconds

        int w = SCREENWIDTH;
        int h = SCREENHEIGHT;

        glfwGetFramebufferSize(window, &w, &h);

        glClear(GL_COLOR_BUFFER_BIT);

        ballistic.updateRound(dt);

        glUseProgram(program);
        glUniform2f(uResolutionLoc, (float)w, (float)h);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        for (int i = 0; i < ballistic.rounds.size(); i++) {
            if (ballistic.rounds[i].type == Ballistic::UNUSED) continue;

            Vector3 position = ballistic.rounds[i].particle.getPosition();

            switch (ballistic.rounds[i].type) {
            case Ballistic::PISTOL: glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f); break;
            case Ballistic::ARTILLERY: glUniform3f(uColorLoc, 1.0f, 0.8f, 0.2f); break;
            case Ballistic::FIREBALL:  glUniform3f(uColorLoc, 1.0f, 0.2f, 0.2f); break;
            default:                   glUniform3f(uColorLoc, 0.6f, 0.6f, 0.6f); break;
            }

            particleVerticies = makeCircleFan(position, ballistic.rounds[i].particle.getRadius(), res);

            glBufferSubData(GL_ARRAY_BUFFER, 0, particleVerticies.size() * sizeof(float), particleVerticies.data());
            glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(particleVerticies.size() / 3));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}