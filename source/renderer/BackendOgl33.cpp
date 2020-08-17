//
// Created by vincent on 12.08.20.
//

#include "BackendOgl33.h"

// Constructor

BackendOgl33::BackendOgl33() = default;

BackendOgl33::~BackendOgl33() = default;

// API methods

void BackendOgl33::ClearColor(float red, float green, float blue, float alpha) {
  glClearColor(red, green, blue, alpha);
};

void BackendOgl33::Clear(bool color_buffer, bool depth_buffer) {
  if (color_buffer && !depth_buffer) {
	glClear(GL_COLOR_BUFFER_BIT);
  } else if (!color_buffer && depth_buffer) {
	glClear(GL_DEPTH_BUFFER_BIT);
  } else if (color_buffer /*&& depth_buffer*/) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
}

// Device methods

void BackendOgl33::Init() {
  std::cout << "Hello 33" << std::endl;
  if (!glfwInit()) {
	throw std::runtime_error("Unable to init GLFW.");
  }
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  this->window = glfwCreateWindow(1024, 768, "Nara Opengl 3.3", NULL, NULL);
  if (!this->window) {
	// throw std::runtime_error("Unable to create a window.");
	// Don't interrupt
	this->suitable = false;
	return;
  }
  glfwMakeContextCurrent(this->window);

  if (GLEW_OK != glewInit()) {
	throw std::runtime_error("Unable to init GLEW.");
  }

  this->suitable = true;
};

bool BackendOgl33::IsOpen() {
  return !glfwWindowShouldClose(window);
}

bool BackendOgl33::IsSuitable() {
  return this->suitable;
};

void BackendOgl33::Destroy() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

std::string BackendOgl33::Name() {
  return "OpenGL 3.3";
}

void BackendOgl33::SwapBuffers() {
  glfwSwapBuffers(this->window);
}
