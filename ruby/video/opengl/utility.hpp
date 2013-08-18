static unsigned glrSize(unsigned size) {
  return size;
//return bit::round(size);  //return nearest power of two
}

static GLuint glrFormat(const string& format) {
  if(format == "rgba8"  ) return GL_RGBA8;
  if(format == "rgb10a2") return GL_RGB10_A2;
  if(format == "rgba12" ) return GL_RGBA12;
  if(format == "rgba16" ) return GL_RGBA16;
  if(format == "rgba16f") return GL_RGBA16F;
  if(format == "rgba32f") return GL_RGBA32F;
  return GL_RGBA8;
}

static GLuint glrFilter(const string& filter) {
  if(filter == "nearest") return GL_NEAREST;
  if(filter == "linear" ) return GL_LINEAR;
  return GL_LINEAR;
}

static GLuint glrWrap(const string& wrap) {
  if(wrap == "border") return GL_CLAMP_TO_BORDER;
  if(wrap == "edge"  ) return GL_CLAMP_TO_EDGE;
  if(wrap == "repeat") return GL_REPEAT;
  return GL_CLAMP_TO_BORDER;
}

static unsigned glrModulo(unsigned modulo) {
  if(modulo) return modulo;
  return 300;  //divisible by 2, 3, 4, 5, 6, 10, 12, 15, 20, 25, 30, 50, 60, 100, 150
}

static GLuint glrProgram() {
  GLuint program = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
  return program;
}

static void glrUniform1i(const string& name, GLint value) {
  GLint location = glGetUniformLocation(glrProgram(), name);
  glUniform1i(location, value);
}

static void glrUniform4f(const string& name, GLfloat value0, GLfloat value1, GLfloat value2, GLfloat value3) {
  GLint location = glGetUniformLocation(glrProgram(), name);
  glUniform4f(location, value0, value1, value2, value3);
}

static void glrUniformMatrix4fv(const string& name, GLfloat *values) {
  GLint location = glGetUniformLocation(glrProgram(), name);
  glUniformMatrix4fv(location, 1, GL_FALSE, values);
}

static void glrParameters(GLuint filter, GLuint wrap) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}

static GLuint glrCreateShader(GLuint program, GLuint type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, 0);
  glCompileShader(shader);
  GLint result = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char text[length + 1];
    glGetShaderInfoLog(shader, length, &length, text);
    text[length] = 0;
    print("[ruby::OpenGL: shader compiler error]\n", (const char*)text, "\n\n");
    return 0;
  }
  glAttachShader(program, shader);
  return shader;
}

static void glrLinkProgram(GLuint program) {
  glLinkProgram(program);
  GLint result = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if(result == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char text[length + 1];
    glGetProgramInfoLog(program, length, &length, text);
    text[length] = 0;
    print("[ruby::OpenGL: shader linker error]\n", (const char*)text, "\n\n");
  }
  glValidateProgram(program);
  result = GL_FALSE;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
  if(result == GL_FALSE) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char text[length + 1];
    glGetProgramInfoLog(program, length, &length, text);
    text[length] = 0;
    print("[ruby::OpenGL: shader validation error]\n", (const char*)text, "\n\n");
  }
}
