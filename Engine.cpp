//
// Created by elder on 12/19/2025.
//

#include "Particle.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <cmath>
#include <vector>
#include "windowFunctions.h"

#define GLOBAL_TIME 0.01667f
#define PI 3.14159265358979323846f
#define SCREENWIDTH 600.0f
#define SCREENHEIGHT 800.0f
#define SMALL_GRAVITY -9.8f


static std::vector<Vector3> makeCircleFanPixels(Vector3 center, float radius, int res) {
    std::vector<Vector3> v;
    v.reserve(res + 2);
    // center
    v.push_back(center);

    // rim: res + 1 to close the fan
    for (int i = 0; i <= res; i++) {
        float t = static_cast<float>(i) / static_cast<float>(res);
        float a = t * 2.0f * PI;

        Vector3 p;
        p.x = center.x + std::cos(a) * radius;
        p.y = center.y + std::sin(a) * radius;
        p.z = center.z;
        v.push_back(p);
    }
    return v;
}

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

    GLFWwindow* window = startGLFWwindow(SCREENWIDTH, SCREENHEIGHT);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return 1;
    }

    GLuint program = createProgram(cirlceVertex, circleFragment);
    if (!program) return 1;

    float cx = SCREENWIDTH / 2.0f;
    float cy = SCREENHEIGHT / 2.0f;
    float radius = 50.0f;
    int res = 8;

    for (int i = 0; i <= res; i++) {
        float t = static_cast<float>(i) / static_cast<float>(res);
        float a = t * 2.0 * PI;
        std::cout << "T: " << t << std::endl;
        std::cout << "A: " << a << std::endl;
        std::cout << "cos(a): " << std::cos(a) << std::endl;
        std::cout << "sin(a):" << std::sin(a) << std::endl;

        float x = cx + std::cos(a) * radius;
        float y = cy + std::sin(a) * radius;

        std::cout << "(x,y): " << "(" << x << "," << y << ")" << std::endl;
        std::cout << std::endl;
    }

    Vector3 position = { cx, cy, 0};


    std::vector<Vector3> verts = makeCircleFanPixels(position, radius, res);
    GLsizei vertexCount = static_cast<GLsizei>(verts.size());
    std::cout << std::endl;

    std::cout << "The size of verts is: " << verts.size() << std::endl;

    for (auto vert : verts) {
        std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
    }


    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        verts.size() * sizeof(Vector3),
        verts.data(),
        GL_DYNAMIC_DRAW
    );
    // aPos = 2 floats
    glVertexAttribPointer(
        0,                  // layout(location = 0)
        3,                  // x, y, z
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vector3),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Uniform locations (cache them)
    GLint uResolutionLoc = glGetUniformLocation(program, "uResolution");
    GLint uColorLoc = glGetUniformLocation(program, "uColor");

    while (!glfwWindowShouldClose(window)) {
        int w = SCREENWIDTH;
        int h = SCREENHEIGHT;
        glfwGetFramebufferSize(window, &w, &h);

        glClear(GL_COLOR_BUFFER_BIT);

        position.y -= 1;
        verts = makeCircleFanPixels(position, 50.0f, res);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            verts.size() * sizeof(Vector3),
            verts.data()
        );


        glUseProgram(program);
        glUniform2f(uResolutionLoc, (float)w, (float)h);
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f); // white circle

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteBuffers(1, &vbo);
    //glDeleteVertexArrays(1, &vao);
    //glDeleteProgram(program);

    //glfwDestroyWindow(window);
    //glfwTerminate();

    //Particle particle1;
    //particle1.setPosition(0.0f, 10.0f, 0.0f);
    //particle1.setVelocity(0.0f, 0.0f, 0.0f);
    //particle1.setAcceleration(0, 0, 0);
    //particle1.setMass(5.0f);
    //particle1.setDamping(1.0f); // damping of 1.0 means object keeps all of its velocity, we do 0 < damping < 1 if we want to reduce velocity a bit

    //float floor = 0.0f;
    //float wallRight = 1.0f;

    //int step = 0;
    //while (particle1.getPosition().y > floor) {
    //    std::cout << "Current step: " << ++step << std::endl;
    //    float force = SMALL_GRAVITY / particle1.getInverseMass();

    //    particle1.addForce(0, force, 0);
    //    
    //    particle1.update(0.1);
    // 
    //    if (particle1.getPosition().y <= 0) {
    //        std::cout << "Particle has hit the floor" << std::endl;
    //        particle1.setPosition(0, 0, 0);
    //        break;
    //    }
    //}
    return 0;
}