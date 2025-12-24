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
    float cy = 800;
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


    GLuint vao = 0, vbo = 0; // declare two openGL object handles(ID's), initalize to 0 to mean no object yet
    glGenVertexArrays(1, &vao); // generates 1 VAO and stores its ID into vao
    glGenBuffers(1, &vbo); // generates 1 VBO and stores its ID into vbo

    std::cout << std::endl;

    std::cout << "VAO ID: " << vao << std::endl;
    std::cout << "VBO ID: " << vbo << std::endl;
    std::cout << std::endl;
    glBindVertexArray(vao); // Binds VAO so all vertex attribute calls will be sotred in current VAO ID
    std::cout << "GL_ARRAY_BUFFER before binding: " << GL_ARRAY_BUFFER << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // makes vbo an active buffer, telling OpenGL where the vertex data will be uploaded and which buffer vertex attributes will read from 
    std::cout << "GL_ARRAY_BUFFER after binding: " << GL_ARRAY_BUFFER << std::endl;
    std::cout << std::endl;

    std::cout << "Verts.size(): " << verts.size() << std::endl;
    std::cout << "Size of Vector3: " << sizeof(Vector3) << std::endl;
    std::cout << "Size of float: " << sizeof(float) << std::endl;
    std::cout << "Verts.data(): " << verts.data() << std::endl;
    glBufferData(
        GL_ARRAY_BUFFER, // openGL uses current bound VBO from GL_ARRAY_BUFFER above
        verts.size() * sizeof(Vector3), // uploads verts.size() * sizeof(Vector3) amount of bytes to allocate on the GPU
        verts.data(), // Copies data from CPU vertex array to GPU memory
        GL_DYNAMIC_DRAW // A usage hint, saying that data may chnage sometimes
    );


    glVertexAttribPointer(
        0, // location of attribute, should match the vertex shader
        3, // number of components per verticie, 3 floats
        GL_FLOAT, // type of the component, we are using float so GL_FLOAT
        GL_FALSE, // should values be normalized? This case NO
        sizeof(Vector3), // Stride, the distance between each verticie in bytes
        (void*)0 // byte offset into current bound VBO, essentially saying start at begin of buffer
    );

    glEnableVertexAttribArray(0); // activates vertex attributes since before this call vertex attributes have been defined but not enabled

    glBindVertexArray(0); // Stop recording VAO onto the GPU. At this point, the VAO contains all the vertex attribute configuration needed for drawing, so we stop recording additional state into it

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