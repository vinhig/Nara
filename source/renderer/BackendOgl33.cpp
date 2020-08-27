//
// Created by vincent on 12.08.20.
//

#include "BackendOgl33.h"

// Constructor

BackendOgl33::BackendOgl33() = default;

BackendOgl33::~BackendOgl33() = default;

// API methods

unsigned int BackendOgl33::CreateBuffer(void *data, GLBType bufferType,
                                        size_t size) {
  uint32_t buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer((GLenum)bufferType, buffer);
  glBufferData((GLenum)bufferType, size, data, GL_STATIC_DRAW);
  glBindBuffer((GLenum)bufferType, 0);

  return buffer;
}

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

uint32_t BackendOgl33::CreateVao(InputLayoutArgs inputLayout) {
  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  int sum = 0;
  for (size_t i = 0; i < inputLayout.entries.Count(); i++) {
    auto entry = inputLayout.entries[i];
    glBindBuffer(GL_ARRAY_BUFFER, entry.buffer);
    glVertexAttribPointer(entry.index, entry.size, (GLenum)entry.subtype,
                          entry.normalized ? GL_TRUE : GL_FALSE, entry.stride,
                          (const void *)sum);
    sum += entry.stride;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);

  return vao;
}

uint32_t CompileShader(std::string source, GLenum shaderType) {
  // Compile shader
  uint32_t shader = glCreateShader(shaderType);
  char const *csources = source.c_str();
  glShaderSource(shader, 1, &csources, nullptr);
  glCompileShader(shader);

  std::cout << "salut" << std::endl;
  // Check result
  int result = GL_FALSE;
  int infoLength;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
  if ( infoLength > 0 ){
		char* errorMsg = new char[infoLength+1];
		glGetShaderInfoLog(shader, infoLength, NULL, &errorMsg[0]);
    std::cout << "bug" << std::endl;
		throw std::runtime_error(errorMsg);
	}

  return shader;
}

uint32_t BackendOgl33::CreateProgram(std::string vertexShaderPath,
                                     std::string fragmentShaderPath) {
  std::string vertexSource = File::ReadAllFile(vertexShaderPath);
  uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
  return 0;
}

void BackendOgl33::DrawSingle(uint32_t vao) {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
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

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(BackendOgl33::MessageCallback, nullptr);

  this->suitable = true;
};

bool BackendOgl33::IsOpen() { return !glfwWindowShouldClose(window); }

bool BackendOgl33::IsSuitable() { return this->suitable; };

void BackendOgl33::Destroy() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

std::string BackendOgl33::Name() { return "OpenGL 3.3"; }

void BackendOgl33::SwapBuffers() { glfwSwapBuffers(this->window); }
