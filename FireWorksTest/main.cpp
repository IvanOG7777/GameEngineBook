//
// Created by elder on 01/16/2026.
//

#include <iostream>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "Header/firework.h"
#include "Header/globalConstants.h"
#include "Header/windowFunctions.h"
#include "Header/objects.h"

const char *vertexShader = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        uniform vec2 uResolution;
        uniform vec2 uOffset;
        uniform float uScale;

        void main() {
            vec2 worldPos = aPos.xy * uScale + uOffset;

            vec2 ndc = worldPos / uResolution;
            ndc = ndc * 2.0 - 1.0;

            gl_Position = vec4(ndc, 0.0, 1.0);
        }
    )GLSL";

const char *fragmentShader = R"GLSL(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )GLSL";

int main() {
    //GLFW hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Created Window object
    GLFWwindow *window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT, false);

    // Use the cursor call back function to set the cursor on the window
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    // Make the window object the current window
    glfwMakeContextCurrent(window);

    // Get the size of the window using the bufferSize call back function
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

    // Checks to se if we can load glad
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return 1;
    }


    Firework firework; // Firework object instance
    glfwSetWindowUserPointer(window, &firework);
    // Attaches a pointer to a firework object to the current window, we can retrieve with callbacks later

    GLuint program = createProgram(vertexShader, fragmentShader); // create program with vertex and fragment shaders
    if (!program) return 1; // if not created return 1 (failure)
    glUseProgram(program); // tell glfw to use this program

    // Tell glfw where to find "uResolution" and "uColor" from the program and store them
    GLuint uResolutionLoc = glGetUniformLocation(program, "uResolution");
    GLuint uColorLoc = glGetUniformLocation(program, "uColor");
    GLuint uOffsetLoc = glGetUniformLocation(program, "uOffset");
    GLuint uScaleLoc = glGetUniformLocation(program, "uScale");

    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    int res = 15;
    float baseRadius = 1.0f;
    Vector3 basePosition;
    std::vector<Vector3> particleVertices = makeCircleFan(basePosition, baseRadius, res);

    glBufferData(
        GL_ARRAY_BUFFER,
        particleVertices.size() * sizeof(Vector3),
        particleVertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vector3),
        (void *) 0
    );


    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    bool escWasDown = false;
    bool fWasDown = false;

    auto startTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = currentTime - startTime;
        startTime = currentTime;

        const auto dt = static_cast<float>(deltaTime.count());

        int w = static_cast<int>(SCREENWIDTH);
        int h = static_cast<int>(SCREENHEIGHT);

        glfwGetFramebufferSize(window, &w, &h);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniform2f(uResolutionLoc, static_cast<float>(w), static_cast<float>(h));
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        bool escDown = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
        bool fDown = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;

        if (fDown && !fWasDown) {
            firework.rocketBurstRules();
            firework.spawnFirework(GLFW_KEY_F);
        }

        firework.rocketBurstLoop(dt);
        // TODO figure out how to only call this function when f was pressed. Right now we press then enter the loop, but if i put it inside of the if conditional it doesn't update.

        if (escDown && !escWasDown) {
            std::cout << "Program has been killed " << std::endl;
            glfwSetWindowShouldClose(window, true);
        }

        fWasDown = fDown;
        escWasDown = escDown;


        firework.updateFireworks(dt);
        firework.treeReset();
        firework.addFireworksFromVectorToTree();
        resolveAllCollisionsKDTree(firework);

        for (auto &activeFirework: firework.activeFireworks) {
            if (activeFirework == nullptr) continue;
            if (activeFirework->type == Firework::UNUSED) continue;

            auto &node = activeFirework;

            sweptBounds(node->particle, dt, w, h);

            float particleRadius = node->particle.getRadius();
            Vector3 particlePosition = node->particle.getPosition();

            switch (node->type) {
                case Firework::EXTRALARGE: glUniform3f(uColorLoc, 1.0f, 1.0f, 0.0f);
                    break;
                case Firework::LARGE: glUniform3f(uColorLoc, 0.0f, 1.0f, 0.0f);
                    break;
                case Firework::MEDIUM: glUniform3f(uColorLoc, 0.0f, 0.0f, 1.0f);
                    break;
                case Firework::SMALL: glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f);
                    break;
                default: glUniform3f(uColorLoc, 0.6f, 0.6f, 0.6f);
                    break;
            }

            glUniform2f(uOffsetLoc, particlePosition.x, particlePosition.y); // pass the particles position values to the uOffsetLocation in the program
            glUniform1f(uScaleLoc, particleRadius); // pass the particles radius value to the uScaleLocation in the program

            glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(particleVertices.size())); // draw the circle with premade vertices from above.
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Total added particles: " << firework.activeNodeCount << '\n';

    return 0;
}
