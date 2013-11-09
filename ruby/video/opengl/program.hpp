void OpenGLProgram::bind(OpenGL* instance, const Markup::Node& node, const string& pathname) {
  filter = glrFilter(node["filter"].text());
  wrap = glrWrap(node["wrap"].text());
  modulo = glrModulo(node["modulo"].integer());

  string w = node["width"].text(), h = node["height"].text();
  if(w.endswith("%")) relativeWidth = real(w.rtrim<1>("%")) / 100.0;
  else absoluteWidth = decimal(w);
  if(h.endswith("%")) relativeHeight = real(h.rtrim<1>("%")) / 100.0;
  else absoluteHeight = decimal(h);

  if(node.name != "program") return;
  format = glrFormat(node["format"].text());

  program = glCreateProgram();
  glGenFramebuffers(1, &framebuffer);

  if(file::exists({pathname, node["vertex"].text()})) {
    string source = file::read({pathname, node["vertex"].text()});
    parse(instance, source);
    vertex = glrCreateShader(program, GL_VERTEX_SHADER, source);
  } else {
    vertex = glrCreateShader(program, GL_VERTEX_SHADER, OpenGLVertexShader);
  }

  if(file::exists({pathname, node["geometry"].text()})) {
    string source = file::read({pathname, node["geometry"].text()});
    parse(instance, source);
    geometry = glrCreateShader(program, GL_GEOMETRY_SHADER, source);
  } else {
  //geometry shaders, when attached, must pass all vertex output through to the fragment shaders
  //geometry = glrCreateShader(program, GL_GEOMETRY_SHADER, OpenGLGeometryShader);
  }

  if(file::exists({pathname, node["fragment"].text()})) {
    string source = file::read({pathname, node["fragment"].text()});
    parse(instance, source);
    fragment = glrCreateShader(program, GL_FRAGMENT_SHADER, source);
  } else {
    fragment = glrCreateShader(program, GL_FRAGMENT_SHADER, OpenGLFragmentShader);
  }

  for(auto& leaf : node.find("pixmap")) {
    nall::image image({pathname, leaf.text()});
    image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    if(image.empty()) continue;

    GLuint texture;
    glGenTextures(1, &texture);

    unsigned n = pixmaps.size();
    pixmaps(n).texture = texture;
    pixmaps(n).width = image.width;
    pixmaps(n).height = image.height;
    pixmaps(n).format = format;
    pixmaps(n).filter = filter;
    pixmaps(n).wrap = wrap;
    if(leaf["format"].exists()) pixmaps(n).format = glrFormat(leaf["format"].text());
    if(leaf["filter"].exists()) pixmaps(n).filter = glrFilter(leaf["filter"].text());
    if(leaf["wrap"].exists()) pixmaps(n).wrap = glrWrap(leaf["wrap"].text());

    unsigned w = glrSize(image.width), h = glrSize(image.height);
    uint32_t* buffer = new uint32_t[w * h]();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, pixmaps(n).format, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, image.data);
    delete[] buffer;
  }

  OpenGLSurface::allocate();
  glrLinkProgram(program);
}

//apply manifest settings to shader source #in tags
void OpenGLProgram::parse(OpenGL* instance, string& source) {
  lstring lines = source.split("\n");
  for(auto& line : lines) {
    string s = line;
    if(auto position = s.find("//")) s.resize(position());  //strip comments
    s.strip();  //remove extraneous whitespace
    if(s.match("#in ?*")) {
      s.ltrim<1>("#in ").strip();
      if(auto setting = instance->settings.find({s})) {
        line = {"#define ", setting().name, " ", setting().value};
      } else {
        line.reset();  //undefined variable (test in source with #ifdef)
      }
    }
  }
  source = lines.merge("\n");
}

void OpenGLProgram::release() {
  OpenGLSurface::release();
  for(auto& pixmap : pixmaps) glDeleteTextures(1, &pixmap.texture);
  pixmaps.reset();

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
