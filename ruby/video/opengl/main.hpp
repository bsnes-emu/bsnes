void OpenGL::shader(const char* pathname) {
  for(auto& program : programs) program.release();
  programs.reset();

  for(auto& frame : frames) glDeleteTextures(1, &frame.texture);
  frames.reset();

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

    bind(document, pathname);
    OpenGLProgram::bind(this, document["output"], pathname);
  } else {
    //no shader; assign default values
    history.length = 0;
    history.format = GL_RGBA8;
    history.filter = GL_LINEAR;
    history.wrap = GL_CLAMP_TO_BORDER;
  }
}

void OpenGL::bind(const Markup::Node& node, const string& pathname) {
  history.length = node["history/frames"].decimal();
  if(node["history/format"].exists()) history.format = glrFormat(node["history/format"].text());
  if(node["history/filter"].exists()) history.filter = glrFilter(node["history/filter"].text());
  if(node["history/wrap"].exists()) history.wrap = glrWrap(node["history/wrap"].text());
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

  //frame[] must always contain max# of previous frames: allocate them now, so first few frames can use them
  while(frames.size() < history.length) {
    OpenGLTexture frame;
    glGenTextures(1, &frame.texture);
    glBindTexture(GL_TEXTURE_2D, frame.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, frame.format = history.format, frame.width = width, frame.height = height, 0, GL_BGRA, inputFormat, buffer);
    frame.filter = history.filter;
    frame.wrap = history.wrap;
    frames.append(frame);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, inputFormat, buffer);

  struct Source {
    GLuint texture;
    unsigned width, height;
    GLuint filter, wrap;
  };
  vector<Source> sources;

  unsigned sourceWidth = width, sourceHeight = height;
  sources.prepend({texture, sourceWidth, sourceHeight, filter, wrap});

  for(auto& p : programs) {
    unsigned targetWidth = p.absoluteWidth ? p.absoluteWidth : outputWidth;
    unsigned targetHeight = p.absoluteHeight ? p.absoluteHeight : outputHeight;
    if(p.relativeWidth) targetWidth = sourceWidth * p.relativeWidth;
    if(p.relativeHeight) targetHeight = sourceHeight * p.relativeHeight;

    p.size(targetWidth, targetHeight);
    glUseProgram(p.program);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p.framebuffer);

    glrUniform1i("phase", p.phase);
    glrUniform1i("frameLength", frames.size());
    glrUniform1i("sourceLength", sources.size());
    glrUniform1i("pixmapLength", p.pixmaps.size());
    glrUniform4f("targetSize", targetWidth, targetHeight, 1.0 / targetWidth, 1.0 / targetHeight);
    glrUniform4f("outputSize", outputWidth, outputHeight, 1.0 / outputWidth, 1.0 / outputHeight);

    unsigned aid = 0;
    for(auto& frame : frames) {
      glrUniform1i({"frame[", aid, "]"}, aid);
      glrUniform4f({"frameSize[", aid, "]"}, frame.width, frame.height, 1.0 / frame.width, 1.0 / frame.height);
      glActiveTexture(GL_TEXTURE0 + (aid++));
      glBindTexture(GL_TEXTURE_2D, frame.texture);
      glrParameters(frame.filter, frame.wrap);
    }

    unsigned bid = 0;
    for(auto& source : sources) {
      glrUniform1i({"source[", bid, "]"}, aid + bid);
      glrUniform4f({"sourceSize[", bid, "]"}, source.width, source.height, 1.0 / source.width, 1.0 / source.height);
      glActiveTexture(GL_TEXTURE0 + aid + (bid++));
      glBindTexture(GL_TEXTURE_2D, source.texture);
      glrParameters(source.filter, source.wrap);
    }

    unsigned cid = 0;
    for(auto& pixmap : p.pixmaps) {
      glrUniform1i({"pixmap[", cid, "]"}, aid + bid + cid);
      glrUniform4f({"pixmapSize[", bid, "]"}, pixmap.width, pixmap.height, 1.0 / pixmap.width, 1.0 / pixmap.height);
      glActiveTexture(GL_TEXTURE0 + aid + bid + (cid++));
      glBindTexture(GL_TEXTURE_2D, pixmap.texture);
      glrParameters(pixmap.filter, pixmap.wrap);
    }

    glActiveTexture(GL_TEXTURE0);
    glrParameters(p.filter, p.wrap);
    p.render(sourceWidth, sourceHeight, targetWidth, targetHeight);
    glBindTexture(GL_TEXTURE_2D, p.texture);

    p.phase = (p.phase + 1) % p.modulo;
    sourceWidth = p.width, sourceHeight = p.height;
    sources.prepend({p.texture, sourceWidth, sourceHeight, p.filter, p.wrap});
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

  if(frames.size() > 0) {
    OpenGLTexture frame = frames.take();

    glBindTexture(GL_TEXTURE_2D, frame.texture);
    if(width == frame.width && height == frame.height) {
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, inputFormat, buffer);
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, frame.format, frame.width = width, frame.height = height, 0, GL_BGRA, inputFormat, buffer);
    }

    frames.prepend(frame);
  }
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
  shader(nullptr);  //release shader resources (eg frame[] history)
  OpenGLSurface::release();
  if(buffer) { delete[] buffer; buffer = nullptr; }
}
