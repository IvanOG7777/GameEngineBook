#include <iostream>
#include <fstream>
#include <sstream>


#include "windowFunctions.h"
#include "ballistics.h"
#include "globalConstants.h"

GLFWwindow* startGLFWwindow(int height, int width, bool fullscreen) {
	if (!glfwInit()) {
		std::cerr << "Couldn't initialize GLFW window" << std::endl;
		return nullptr;
	}

	GLFWmonitor* monitor = nullptr;
	const GLFWvidmode* mode = nullptr;

	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
		if (!monitor) {
			std::cerr << "Couldn't get primary monitor" << std::endl;
			return nullptr;
		}

		mode = glfwGetVideoMode(monitor);
		if (!mode) {
			std::cerr << "Couldn't get video mode" << std::endl;
			return nullptr;
		}

		// Use native monitor resolution for fullscreen
		width = mode->width;
		height = mode->height;

		// Optional but nice: match monitor refresh rate
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	GLFWwindow* window = glfwCreateWindow(width,height, "Game Engine", monitor, nullptr);

	if (!window) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return nullptr;
	}

	return window;
}

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

static std::string loadTextFile(const char* path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << path << "\n";
		return {};
	}
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

// function to compile a vertex or fragment shader
// parameters are a GLunum type, type of shader we are passing
// and a string representing the shader
GLuint compileShader(GLenum type, const char* src) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int ok = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (!ok) {
		char log[2048];
		glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
		std::cerr << "Shader compile failed " << log << "\n";
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint createProgram(const char* vertexShader, const char* fragmentShader) {
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (!vs || !fs) return 0;

	GLuint prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	glDeleteShader(vs);
	glDeleteShader(fs);

	int ok = 0;
	glGetProgramiv(prog, GL_LINK_STATUS, &ok);
	if (!ok) {
		char log[2048];
		glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
		std::cerr << "Program link error:\n" << log << "\n";
		glDeleteProgram(prog);
		return 0;
	}
	return prog;
}

// function that gets the current x and y position of the mouse
// you have you pass this function into glfwSetCursorPosCallback where points to current screen and assigns x and y position
 void cursorPositionCallback(GLFWwindow* window, double positionX, double positionY) {

	 // create ballistic class pointer to an instance of the ballistic class from main. when did this: glfwSetWindowUserPointer(window, &ballistic) it stores the address of the ballistic class
	 auto* ballistic = static_cast<Ballistic*>(glfwGetWindowUserPointer(window)); // glfwGetWindowUserPointer returns the ballistic class address
	 if (!ballistic) return; // if no pointer is found return out of the function

	 int width = 0;
	 int height = 0;

	 glfwGetWindowSize(window, &width, &height);

	 double flippedY = static_cast<float>(height) - positionY;

	 ballistic->mousePositionX = positionX;
	 ballistic->mousePositionY = flippedY;
}

 // function used to check if the mouse is in the current screen
 // you have to pass the function into glfwSetCursorEnterCallback
 // glfwSetCursorEnterCallback uses entered kinda like a boolean flag, 1 for on screen 0 for off screen
 // glfwSetCursorEnterCallback or cursorEnterCallback can be considered conceptually as a boolean function but really arent
 void cursorEnterCallback(GLFWwindow* window, int entered) {
	 if (entered) {
		 std::cout << "Entered the window" << std::endl;
	 }
	 else {
		 std::cout << "Outside window" << std::endl;
	 }
 }

 // Function used register a mouse click,
 /* Parameters:
 *		Window pointer
 *		button we are pressing
 *		what action we want for the button
 *		what modifiers we with the button click (shift + click or cntrl + click)
 */
 void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {


	 // create ballistic class pointer to an instance of the ballistic class from main. when did this: glfwSetWindowUserPointer(window, &ballistic) it stores the address of the ballistic class
	 auto* ballistic = static_cast<Ballistic*>(glfwGetWindowUserPointer(window)); // glfwGetWindowUserPointer returns the ballistic class address
	 if (!ballistic) return; // if no pointer is found return out of the function

	 // check if the buttons value is the same as GLFW_MOUSE_BUTTON_LEFT and if out action is GLFW_PRESS
	 if (button == GLFW_MOUSE_BUTTON_LEFT) {
		 if (action == GLFW_PRESS) {
			 ballistic->isMouseDown = true;
		 }
		 else if (action == GLFW_RELEASE) {
			 ballistic->isMouseDown = false;
		 }
	 }
 }