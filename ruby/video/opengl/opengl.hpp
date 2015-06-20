#if defined(PLATFORM_XORG)
  #include <GL/gl.h>
  #include <GL/glx.h>
  #define glGetProcAddress(name) (*glXGetProcAddress)((const GLubyte*)(name))
#elif defined(PLATFORM_MACOSX)
  #include <OpenGL/gl.h>
  #include <OpenGL/gl3.h>
#elif defined(PLATFORM_WINDOWS)
  #include <GL/gl.h>
  #include <GL/glext.h>
  #define glGetProcAddress(name) wglGetProcAddress(name)
#else
  #error "ruby::OpenGL: unsupported platform"
#endif

#include "bind.hpp"
#include "shaders.hpp"
#include "utility.hpp"

struct OpenGL;

struct OpenGLTexture {
  auto getFormat() const -> GLuint;
  auto getType() const -> GLuint;

  GLuint texture = 0;
  unsigned width = 0;
  unsigned height = 0;
  GLuint format = GL_RGBA8;
  GLuint filter = GL_LINEAR;
  GLuint wrap = GL_CLAMP_TO_BORDER;
};

struct OpenGLSurface : OpenGLTexture {
  auto allocate() -> void;
  auto size(unsigned width, unsigned height) -> void;
  auto release() -> void;
  auto render(unsigned sourceWidth, unsigned sourceHeight, unsigned targetWidth, unsigned targetHeight) -> void;

  GLuint program = 0;
  GLuint framebuffer = 0;
  GLuint vao = 0;
  GLuint vbo[3] = {0, 0, 0};
  GLuint vertex = 0;
  GLuint geometry = 0;
  GLuint fragment = 0;
  uint32_t* buffer = nullptr;
};

struct OpenGLProgram : OpenGLSurface {
  auto bind(OpenGL* instance, const Markup::Node& node, const string& pathname) -> void;
  auto parse(OpenGL* instance, string& source) -> void;
  auto release() -> void;

  unsigned phase = 0;   //frame counter
  unsigned modulo = 0;  //frame counter modulus
  unsigned absoluteWidth = 0;
  unsigned absoluteHeight = 0;
  double relativeWidth = 0;
  double relativeHeight = 0;
  vector<OpenGLTexture> pixmaps;
};

struct OpenGL : OpenGLProgram {
  auto shader(const string& pathname) -> void;
  auto allocateHistory(unsigned size) -> void;
  auto lock(uint32_t*& data, unsigned& pitch) -> bool;
  auto clear() -> void;
  auto refresh() -> void;
  auto init() -> bool;
  auto term() -> void;

  vector<OpenGLProgram> programs;
  vector<OpenGLTexture> history;
  GLuint inputFormat = GL_RGBA8;
  unsigned outputWidth = 0;
  unsigned outputHeight = 0;
  struct Setting {
    string name;
    string value;
    bool operator< (const Setting& source) const { return name <  source.name; }
    bool operator==(const Setting& source) const { return name == source.name; }
    Setting() = default;
    Setting(const string& name) : name(name) {}
    Setting(const string& name, const string& value) : name(name), value(value) {}
  };
  set<Setting> settings;
  bool initialized = false;
};

#include "texture.hpp"
#include "surface.hpp"
#include "program.hpp"
#include "main.hpp"
