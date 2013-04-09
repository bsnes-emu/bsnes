void OpenGLProgram::bind(OpenGL *instance, const Markup::Node &node, const string &pathname) {
  filter = glrFilter(node["filter"].text());
  wrap = glrWrap(node["wrap"].text());
  modulo = glrModulo(node["modulo"].integer());

  string w = node["width"].text(), h = node["height"].text();
  if(w.endswith("%")) relativeWidth = fp(w.rtrim<1>("%")) / 100.0;
  else absoluteWidth = decimal(w);
  if(h.endswith("%")) relativeHeight = fp(h.rtrim<1>("%")) / 100.0;
  else absoluteHeight = decimal(h);

  if(node.name != "program") return;
  format = glrFormat(node["format"].text());

  program = glCreateProgram();
  glGenFramebuffers(1, &framebuffer);

  if(file::exists({pathname, node["vertex"].text()})) {
    string source(instance->settings);
    source.append(file::read({pathname, node["vertex"].text()}));
    vertex = glrCreateShader(program, GL_VERTEX_SHADER, source);
  } else {
    vertex = glrCreateShader(program, GL_VERTEX_SHADER, OpenGLVertexShader);
  }

  if(file::exists({pathname, node["geometry"].text()})) {
    string source(instance->settings);
    source.append(file::read({pathname, node["geometry"].text()}));
    geometry = glrCreateShader(program, GL_GEOMETRY_SHADER, source);
  } else {
  //geometry shaders, when attached, must pass all vertex output through to the fragment shaders
  //geometry = glrCreateShader(program, GL_GEOMETRY_SHADER, OpenGLGeometryShader);
  }

  if(file::exists({pathname, node["fragment"].text()})) {
    string source(instance->settings);
    source.append(file::read({pathname, node["fragment"].text()}));
    fragment = glrCreateShader(program, GL_FRAGMENT_SHADER, source);
  } else {
    fragment = glrCreateShader(program, GL_FRAGMENT_SHADER, OpenGLFragmentShader);
  }

  for(auto &leaf : node.find("texture")) {
    nall::image image({pathname, leaf.text()});
    image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    if(image.empty()) continue;

    GLuint texture;
    glGenTextures(1, &texture);

    unsigned n = textures.size();
    textures(n).texture = texture;
    textures(n).width = image.width;
    textures(n).height = image.height;
    textures(n).format = format;
    textures(n).filter = filter;
    textures(n).wrap = wrap;
    if(leaf["format"].exists()) textures(n).format = glrFormat(leaf["format"].text());
    if(leaf["filter"].exists()) textures(n).filter = glrFilter(leaf["filter"].text());
    if(leaf["wrap"].exists()) textures(n).wrap = glrWrap(leaf["wrap"].text());

    unsigned w = glrSize(image.width), h = glrSize(image.height);
    uint32_t *buffer = new uint32_t[w * h]();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, textures(n).format, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.data);
    delete[] buffer;
  }

  glrLinkProgram(program);
}

void OpenGLProgram::release() {
  OpenGLSurface::release();
  for(auto &texture : textures) glDeleteTextures(1, &texture.texture);
  textures.reset();

  width = 0;
  height = 0;
  format = GL_RGBA8;
  filter = GL_LINEAR;
  wrap = GL_CLAMP_TO_BORDER;
  phase = 0;
  modulo = 0;
  absoluteWidth = 0;
  absoluteHeight = 0;
  relativeWidth = 0;
  relativeHeight = 0;
}
