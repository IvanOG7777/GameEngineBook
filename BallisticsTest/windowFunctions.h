#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


GLFWwindow* startGLFWwindow(int height, int width, bool fullscreen);
void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
GLuint compileShader(GLenum type, const char* src);
GLuint createProgram(const char* vertexShader, const char* fragmentShader);