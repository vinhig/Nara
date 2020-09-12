//
// Created by vincent on 12.08.20.
//

#include "BackendOgl.h"

// Constructor

BackendOgl::BackendOgl() = default;

BackendOgl::~BackendOgl() = default;

// API methods

uint32_t BackendOgl::CreateBuffer(void *data, size_t size) {
  uint32_t buffer;
  glCreateBuffers(1, &buffer);
  glNamedBufferStorage(buffer, size, data, GL_DYNAMIC_STORAGE_BIT);

  return buffer;
}

void BackendOgl::ClearColor(float red, float green, float blue, float alpha) {
  glClearColor(red, green, blue, alpha);
};

void BackendOgl::Clear(bool color_buffer, bool depth_buffer) {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

uint32_t BackendOgl::CreateVao(InputLayoutArgs inputLayout) {
  uint32_t vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  for (size_t i = 0; i < inputLayout.entries.Count(); i++) {
    auto entry = inputLayout.entries[i];
    glEnableVertexAttribArray(entry.index);
    glBindBuffer(GL_ARRAY_BUFFER, entry.buffer);
    // void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
    //                            GLboolean normalized, GLsizei stride,
    //                            const void *pointer);
    glVertexAttribPointer(entry.index, entry.size, (GLenum)entry.subtype,
                          entry.normalized ? GL_TRUE : GL_FALSE, entry.stride,
                          entry.offset);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);

  return vao;
}

uint32_t CompileShader(std::string source, GLenum shaderType) {
  // Compile shader
  uint32_t shader = glCreateShader(shaderType);
  char const *csources = (source).c_str();
  glShaderSource(shader, 1, &csources, nullptr);
  glCompileShader(shader);

  // Check result
  int result = GL_FALSE;
  int infoLength;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
  if (infoLength > 0) {
    char *errorMsg = new char[infoLength + 1];
    glGetShaderInfoLog(shader, infoLength, NULL, &errorMsg[0]);
    std::cout << source << std::endl;
    throw std::runtime_error(errorMsg);
  }

  return shader;
}

uint32_t BackendOgl::CreateProgram(std::string vertexShaderPath,
                                   std::string fragmentShaderPath) {
  // Read and compile shaders
  std::string vertexSource = File::ReadAllFile(vertexShaderPath);
  uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
  std::string fragmentSource = File::ReadAllFile(fragmentShaderPath);
  uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

  // Create corresponding program
  uint32_t program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // Delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

uint32_t BackendOgl::CreateTexture(TextureSpec textureSpec) {
  uint32_t texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureStorage2D(
      texture, 1, textureSpec.format == TextureFormat::RGB ? GL_RGB8 : GL_RGBA8,
      textureSpec.width, textureSpec.height);

  glTextureSubImage2D(
      texture, 0, 0, 0, textureSpec.width, textureSpec.height,
      textureSpec.format == TextureFormat::RGB ? GL_RGB : GL_RGBA,
      GL_UNSIGNED_BYTE, textureSpec.data);

  return texture;
}

void BackendOgl::DrawSingle(uint32_t vao, uint32_t ibo,
                            Array<uint32_t> *textures,
                            Array<uint32_t> *uniforms, int count) {
  if (vao <= 0 || ibo <= 0) {
    std::runtime_error("Bad vertex array object or index buffer object.");
  }
  // glBindVertexArray(vao);
  // glDrawArrays(GL_TRIANGLES, 0, 3);

  if (previousVao != vao) {
    previousVao = vao;
    glBindVertexArray(vao);
  }
  for (int i = 0; i < textures->Count(); i++) {
    if (textures->Get(i) == 0) {
      throw std::runtime_error("Null texture.");
    }
    glBindTextureUnit(i, textures->Get(i));
  }
  if (previousIbo != ibo) {
    previousIbo = ibo;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  }
  for (int i = 0; i < uniforms->Count(); i++) {
    if (uniforms->Get(i) == 0) {
      throw std::runtime_error("Null uniform buffer.");
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, i, uniforms->Get(i));
  }

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void BackendOgl::DrawInstanced(uint32_t vao, uint32_t ibo,
                               Array<uint32_t> *textures,
                               Array<uint32_t> *uniforms, int count,
                               int primcount) {
  if (vao <= 0 || ibo <= 0) {
    std::runtime_error("Bad vertex array object or index buffer object.");
  }

  if (previousVao != vao) {
    previousVao = vao;
    glBindVertexArray(vao);
  }
  for (int i = 0; i < textures->Count(); i++) {
    if (textures->Get(i) == 0) {
      throw std::runtime_error("Null texture.");
    }
    glBindTextureUnit(i, textures->Get(i));
  }
  if (previousIbo != ibo) {
    previousIbo = ibo;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  }
  for (int i = 0; i < uniforms->Count(); i++) {
    if (uniforms->Get(i) == 0) {
      throw std::runtime_error("Null uniform buffer.");
    }
    glBindBufferBase(GL_UNIFORM_BUFFER, i, uniforms->Get(i));
  }

  glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr,
                          primcount);
}

void BackendOgl::FeedTexture(unsigned char *data) {}

void BackendOgl::UseProgram(uint32_t program) { glUseProgram(program); }

// Device methods

void BackendOgl::Init() {
  std::cout << "Hello 33" << std::endl;
  if (!glfwInit()) {
    throw std::runtime_error("Unable to init GLFW.");
  }
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  this->window = glfwCreateWindow(1024, 768, "Nara Opengl 4.6", NULL, NULL);
  this->width = 1024;
  this->height = 768;
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

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(BackendOgl::MessageCallback, nullptr);

  this->suitable = true;
};

bool BackendOgl::IsOpen() { return !glfwWindowShouldClose(window); }

bool BackendOgl::IsSuitable() { return this->suitable; };

void BackendOgl::Destroy() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

std::string BackendOgl::Name() { return "OpenGL 3.3"; }

void BackendOgl::SwapBuffers() { glfwSwapBuffers(this->window); }
