#if defined(PLATFORM_X)
  #include <GL/gl.h>
  #include <GL/glx.h>
  #define glGetProcAddress(name) (*glXGetProcAddress)((const GLubyte*)(name))
#elif defined(PLATFORM_OSX)
  #include <OpenGL/gl.h>
  #include <OpenGL/gl3.h>
#elif defined(PLATFORM_WIN)
  #include <GL/gl.h>
  #include <GL/glext.h>
  #define glGetProcAddress(name) wglGetProcAddress(name)
#else
  #error "ruby::OpenGL: unsupported platform"
#endif

namespace ruby {

#include "bind.hpp"
#include "shaders.hpp"
#include "utility.hpp"

struct OpenGL;

struct OpenGLTexture {
  GLuint texture = 0;
  unsigned width = 0;
  unsigned height = 0;
  GLuint format = GL_RGBA8;
  GLuint filter = GL_LINEAR;
  GLuint wrap = GL_CLAMP_TO_BORDER;
};

struct OpenGLSurface : OpenGLTexture {
  GLuint program = 0;
  GLuint framebuffer = 0;
  GLuint vao = 0;
  GLuint vbo[3] = {0, 0, 0};
  GLuint vertex = 0;
  GLuint geometry = 0;
  GLuint fragment = 0;
  uint32_t* buffer = nullptr;

  void allocate();
  void size(unsigned width, unsigned height);
  void release();
  void render(unsigned sourceWidth, unsigned sourceHeight, unsigned targetWidth, unsigned targetHeight);
};

struct OpenGLProgram : OpenGLSurface {
  //configuration
  unsigned phase = 0;   //frame counter
  unsigned modulo = 0;  //frame counter modulus
  unsigned absoluteWidth = 0;
  unsigned absoluteHeight = 0;
  double relativeWidth = 0;
  double relativeHeight = 0;
  vector<OpenGLTexture> pixmaps;

  void bind(OpenGL* instance, const Markup::Node& node, const string& pathname);
  void release();
};

struct OpenGL : OpenGLProgram {
  vector<OpenGLProgram> programs;
  vector<OpenGLTexture> frames;
  struct History {
    unsigned length = 0;
    GLuint format = GL_RGBA8;
    GLuint filter = GL_LINEAR;
    GLuint wrap = GL_CLAMP_TO_BORDER;
  } history;

  GLuint inputFormat = GL_UNSIGNED_INT_8_8_8_8_REV;
  unsigned outputWidth = 0;
  unsigned outputHeight = 0;

  void shader(const char* pathname);
  void bind(const Markup::Node& node, const string& pathname);
  bool lock(uint32_t*& data, unsigned& pitch);
  void clear();
  void refresh();
  bool init();
  void term();
};

#include "surface.hpp"
#include "program.hpp"
#include "main.hpp"

}
