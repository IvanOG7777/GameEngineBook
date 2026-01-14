#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


GLFWwindow* startGLFWwindow(int height, int width, bool fullscreen);
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
GLuint compileShader(GLenum type, const char* src);
GLuint createProgram(const char* vertexShader, const char* fragmentShader);

void cursorPositionCallback(GLFWwindow* window, double positionX, double positionY);
void cursorEnterCallback(GLFWwindow* window, int entered);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);