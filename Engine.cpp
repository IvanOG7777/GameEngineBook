//
// Created by elder on 12/19/2025.
//

#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Particle.h"
#include "windowFunctions.h"
#include "objects.h"

#define GLOBAL_TIME 0.01667f
#define PI 3.14159265358979323846f
#define SCREENWIDTH 600.0f
#define SCREENHEIGHT 800.0f
#define SMALL_GRAVITY -9.8f

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
    std::cout  << "Program: " << program << std::endl;
    if (!program) return 1;

    float cx = SCREENWIDTH / 2.0f;
    float cy = SCREENHEIGHT / 2.0f;
    float radius = 200.0f;
    int res = 100;

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


    std::vector<Vector3> verts = makeCircleFan(position, radius, res);
    GLsizei vertexCount = static_cast<GLsizei>(verts.size());
    std::cout << "Vertex Count: " << vertexCount << std::endl;
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
    GLint bound = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound);
    std::cout << "Bound GL_ARRAY_BUFFER VBO: " << bound << "\n";
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // makes vbo an active buffer, telling OpenGL where the vertex data will be uploaded and which buffer vertex attributes will read from 
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
    GLint uResolutionLoc = glGetUniformLocation(program, "uResolution"); // Looks up the uniform named "uResolution" in the linked shader program.OpenGL returns an integer location within the program
    GLint uColorLoc = glGetUniformLocation(program, "uColor"); // Looks up the uniform named "uColor" in the linked shader program. OpenGl returns an integer location within the program
    // These two above will return -1 for failure or some int value for location. They identify where each valiable lives inside pragram
    // In this case within prgram(id 3) at location 0 is where "uResolutionLoc" lives and at location 1 is where "uColorLoc"


    if (uResolutionLoc == -1 || uColorLoc == -1) {
        std::cerr << "Uniform Resoluction or Color was not found or optimized" << std::endl;
        return 1;
    }
    std::cout << "uResolutionLoc: " << uResolutionLoc << std::endl;
    std::cout << "uColorLoc: " << uColorLoc << std::endl;

    // glfwWindowShouldClose returns 0 or 1, 0 meaning we are running 1 meaning we stop,
    // so while !0 (1) keep running
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = currentTime - start;
        start = currentTime;
        double dt = deltaTime.count(); // seconds

        std::cout << "Delta Time (s): " << dt << std::endl;
        int w = SCREENWIDTH;
        int h = SCREENHEIGHT;
        glfwGetFramebufferSize(window, &w, &h); //brief Retrieves the size of the frame of the window.

        glClear(GL_COLOR_BUFFER_BIT); // clears background to black with GL_COLOR_BUFFER_BIT

        if (position.y == 280) {
            break;
        }
        verts = makeCircleFan(position, radius, res);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // rebind per frame the vbo
        // resend data and size in bytes to GPU
        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            verts.size() * sizeof(Vector3),
            verts.data()
        );


        glUseProgram(program); // Makes this shader program the active program for subsequent draw calls
        glUniform2f(uResolutionLoc, (float)w, (float)h); // Uploads the framebuffer width and height to the uResolution uniform (vec2) in the active shader program
        glUniform3f(uColorLoc, 1.0f, 1.0f, 1.0f); // Uploads the color (white) to the uColor uniform (vec3) in the active shader program

        glBindVertexArray(vao); // use pre recorded vertex layout for GPU to use
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount); // tell GPU how to interpret the verticies, where to start and to how many vertices to use
        
        glBindVertexArray(0); // unbinds the vao per frame leaves no active vao safety / cleanliness step,

        glfwSwapBuffers(window); // swaps the back buffer(black) to the front buffer (window)
        glfwPollEvents(); // processes OS/window events like keyboard,mouse input

    }


    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}