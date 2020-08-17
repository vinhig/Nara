//
// Created by vincent on 12.08.20.
//

#include "BackendOgl46.h"

// Constructor

BackendOgl46::BackendOgl46() = default;

BackendOgl46::~BackendOgl46() = default;

// API methods

void BackendOgl46::ClearColor(float red, float green, float blue, float alpha) {
  glClearColor(red, green, blue, alpha);
}

void BackendOgl46::Clear(bool color_buffer, bool depth_buffer) {
  if (color_buffer && !depth_buffer) {
	glClear(GL_COLOR_BUFFER_BIT);
  } else if (!color_buffer && depth_buffer) {
	glClear(GL_DEPTH_BUFFER_BIT);
  } else if (color_buffer /*&& depth_buffer*/) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
}

// Device methods

void BackendOgl46::Init() {
  std::cout << "Hello 46" << std::endl;
  if (!glfwInit()) {
	throw std::runtime_error("Unable to init GLFW.");
  }
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  this->window = glfwCreateWindow(1024, 768, "Nara Opengl 4.6", nullptr, nullptr);
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
}

bool BackendOgl46::IsOpen() {
  return false;
};

bool BackendOgl46::IsSuitable() {
  return this->suitable;
};

void BackendOgl46::Destroy() {

}

std::string BackendOgl46::Name() {
  return "OpenGL 4.6";
}

void BackendOgl46::SwapBuffers() {
  glfwSwapBuffers(this->window);
}