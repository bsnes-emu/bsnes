void OpenGL::shader(const char* pathname) {
  for(auto& program : programs) program.release();
  programs.reset();

  format = GL_RGBA8;
  filter = GL_LINEAR;
  wrap = GL_CLAMP_TO_BORDER;
  absoluteWidth = 0, absoluteHeight = 0;
  relativeWidth = 0, relativeHeight = 0;

  if(pathname) {
    auto document = Markup::Document(file::read({pathname, "manifest.bml"}));
    for(auto& node : document.find("program")) {
      unsigned n = programs.size();
      programs(n).bind(this, node, pathname);
    }

    bind(this, document["output"], pathname);
  }
}

bool OpenGL::lock(uint32_t*& data, unsigned& pitch) {
  pitch = width * sizeof(uint32_t);
  return data = buffer;
}

void OpenGL::clear() {
  for(auto& p : programs) {
    glUseProgram(p.program);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p.framebuffer);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  glUseProgram(0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::refresh() {
  clear();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, inputFormat, buffer);

  struct History {
    GLuint texture;
    unsigned width, height;
    GLuint filter, wrap;
  };
  vector<History> history;

  unsigned sourceWidth = width, sourceHeight = height;
  history.prepend({texture, sourceWidth, sourceHeight, filter, wrap});

  for(auto& p : programs) {
    unsigned targetWidth = p.absoluteWidth ? p.absoluteWidth : outputWidth;
    unsigned targetHeight = p.absoluteHeight ? p.absoluteHeight : outputHeight;
    if(p.relativeWidth) targetWidth = sourceWidth * p.relativeWidth;
    if(p.relativeHeight) targetHeight = sourceHeight * p.relativeHeight;

    p.size(targetWidth, targetHeight);
    glUseProgram(p.program);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p.framebuffer);

    glrUniform1i("phase", p.phase);
    glrUniform1i("sourceLength", history.size());
    glrUniform1i("pixmapLength", p.pixmaps.size());
    glrUniform4f("targetSize", targetWidth, targetHeight, 1.0 / targetWidth, 1.0 / targetHeight);
    glrUniform4f("outputSize", outputWidth, outputHeight, 1.0 / outputWidth, 1.0 / outputHeight);
  //glrUniform4f("targetActualSize", glrSize(targetWidth), glrSize(targetHeight), 1.0 / glrSize(targetWidth), 1.0 / glrSize(targetHeight));
  //glrUniform4f("outputActualSize", glrSize(outputWidth), glrSize(outputHeight), 1.0 / glrSize(outputWidth), 1.0 / glrSize(outputHeight));

    unsigned aid = 0;
    for(auto& pixmap : history) {
      glrUniform1i({"source[", aid, "]"}, aid);
      glrUniform4f({"sourceSize[", aid, "]"}, pixmap.width, pixmap.height, 1.0 / pixmap.width, 1.0 / pixmap.height);
    //glrUniform4f({"sourceActualSize[", aid, "]"}, glrSize(pixmap.width), glrSize(pixmap.height), 1.0 / glrSize(pixmap.width), 1.0 / glrSize(pixmap.height));
      glActiveTexture(GL_TEXTURE0 + (aid++));
      glBindTexture(GL_TEXTURE_2D, pixmap.texture);
      glrParameters(pixmap.filter, pixmap.wrap);
    }

    unsigned bid = 0;
    for(auto& pixmap : p.pixmaps) {
      glrUniform1i({"pixmap[", bid, "]"}, aid + bid);
      glrUniform4f({"pixmapSize[", bid, "]"}, pixmap.width, pixmap.height, 1.0 / pixmap.width, 1.0 / pixmap.height);
    //glrUniform4f({"pixmapActualSize[", bid, "]"}, glrSize(pixmap.width), glrSize(pixmap.height), 1.0 / glrSize(pixmap.width), 1.0 / glrSize(pixmap.height));
      glActiveTexture(GL_TEXTURE0 + aid + (bid++));
      glBindTexture(GL_TEXTURE_2D, pixmap.texture);
      glrParameters(pixmap.filter, pixmap.wrap);
    }

    glActiveTexture(GL_TEXTURE0);
    glrParameters(p.filter, p.wrap);
    p.render(sourceWidth, sourceHeight, targetWidth, targetHeight);
    glBindTexture(GL_TEXTURE_2D, p.texture);

    p.phase = (p.phase + 1) % p.modulo;
    sourceWidth = p.width, sourceHeight = p.height;
    history.prepend({p.texture, sourceWidth, sourceHeight, p.filter, p.wrap});
  }

  unsigned targetWidth = absoluteWidth ? absoluteWidth : outputWidth;
  unsigned targetHeight = absoluteHeight ? absoluteHeight : outputHeight;
  if(relativeWidth) targetWidth = sourceWidth * relativeWidth;
  if(relativeHeight) targetHeight = sourceHeight * relativeHeight;

  glUseProgram(program);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glrUniform1i("source[0]", 0);
  glrUniform4f("targetSize", targetWidth, targetHeight, 1.0 / targetWidth, 1.0 / targetHeight);
  glrUniform4f("outputSize", outputWidth, outputHeight, 1.0 / outputWidth, 1.0 / outputHeight);

  glrParameters(filter, wrap);
  render(sourceWidth, sourceHeight, outputWidth, outputHeight);
}

bool OpenGL::init() {
  if(!OpenGLBind()) return false;

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_POLYGON_SMOOTH);
  glDisable(GL_STENCIL_TEST);

  glEnable(GL_DITHER);
  glEnable(GL_TEXTURE_2D);

  program = glCreateProgram();
  vertex = glrCreateShader(program, GL_VERTEX_SHADER, OpenGLOutputVertexShader);
//geometry = glrCreateShader(program, GL_GEOMETRY_SHADER, OpenGLGeometryShader);
  fragment = glrCreateShader(program, GL_FRAGMENT_SHADER, OpenGLFragmentShader);
  OpenGLSurface::allocate();
  glrLinkProgram(program);

  shader(nullptr);
  return true;
}

void OpenGL::term() {
  OpenGLSurface::release();
  if(buffer) { delete[] buffer; buffer = nullptr; }
}
