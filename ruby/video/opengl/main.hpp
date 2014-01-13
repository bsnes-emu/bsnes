void OpenGL::shader(const char* pathname) {
  for(auto& program : programs) program.release();
  programs.reset();

  settings.reset();

  format = inputFormat;
  filter = GL_LINEAR;
  wrap = GL_CLAMP_TO_BORDER;
  absoluteWidth = 0, absoluteHeight = 0;
  relativeWidth = 0, relativeHeight = 0;

  unsigned historySize = 0;
  if(pathname) {
    auto document = Markup::Document(file::read({pathname, "manifest.bml"}));

    for(auto& node : document["settings"]) {
      settings.insert({node.name, node.text()});
    }

    for(auto& node : document["input"]) {
      if(node.name == "history") historySize = node.decimal();
      if(node.name == "format") format = glrFormat(node.text());
      if(node.name == "filter") filter = glrFilter(node.text());
      if(node.name == "wrap") wrap = glrWrap(node.text());
    }

    for(auto& node : document["output"]) {
      string text = node.text();
      if(node.name == "width") {
        if(text.endsWith("%")) relativeWidth = real(text.rtrim<1>("%")) / 100.0;
        else absoluteWidth = decimal(text);
      }
      if(node.name == "height") {
        if(text.endsWith("%")) relativeHeight = real(text.rtrim<1>("%")) / 100.0;
        else absoluteHeight = decimal(text);
      }
    }

    for(auto& node : document.find("program")) {
      unsigned n = programs.size();
      programs(n).bind(this, node, pathname);
    }
  }

  //changing shaders may change input format, which requires the input texture to be recreated
  if(texture) { glDeleteTextures(1, &texture); texture = 0; }
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, getFormat(), getType(), buffer);
  allocateHistory(historySize);
}

void OpenGL::allocateHistory(unsigned size) {
  for(auto& frame : history) glDeleteTextures(1, &frame.texture);
  history.reset();
  while(size--) {
    OpenGLTexture frame;
    frame.filter = filter;
    frame.wrap = wrap;
    glGenTextures(1, &frame.texture);
    glBindTexture(GL_TEXTURE_2D, frame.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, frame.width = width, frame.height = height, 0, getFormat(), getType(), buffer);
    history.append(frame);
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
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, getFormat(), getType(), buffer);

  struct Source {
    GLuint texture;
    unsigned width, height;
    GLuint filter, wrap;
  };
  vector<Source> sources;
  sources.prepend({texture, width, height, filter, wrap});

  for(auto& p : programs) {
    unsigned targetWidth = p.absoluteWidth ? p.absoluteWidth : outputWidth;
    unsigned targetHeight = p.absoluteHeight ? p.absoluteHeight : outputHeight;
    if(p.relativeWidth) targetWidth = sources[0].width * p.relativeWidth;
    if(p.relativeHeight) targetHeight = sources[0].height * p.relativeHeight;

    p.size(targetWidth, targetHeight);
    glUseProgram(p.program);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p.framebuffer);

    glrUniform1i("phase", p.phase);
    glrUniform1i("historyLength", history.size());
    glrUniform1i("sourceLength", sources.size());
    glrUniform1i("pixmapLength", p.pixmaps.size());
    glrUniform4f("targetSize", targetWidth, targetHeight, 1.0 / targetWidth, 1.0 / targetHeight);
    glrUniform4f("outputSize", outputWidth, outputHeight, 1.0 / outputWidth, 1.0 / outputHeight);

    unsigned aid = 0;
    for(auto& frame : history) {
      glrUniform1i({"history[", aid, "]"}, aid);
      glrUniform4f({"historySize[", aid, "]"}, frame.width, frame.height, 1.0 / frame.width, 1.0 / frame.height);
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
    glrParameters(sources[0].filter, sources[0].wrap);
    p.render(sources[0].width, sources[0].height, targetWidth, targetHeight);
    glBindTexture(GL_TEXTURE_2D, p.texture);

    p.phase = (p.phase + 1) % p.modulo;
    sources.prepend({p.texture, p.width, p.height, p.filter, p.wrap});
  }

  unsigned targetWidth = absoluteWidth ? absoluteWidth : outputWidth;
  unsigned targetHeight = absoluteHeight ? absoluteHeight : outputHeight;
  if(relativeWidth) targetWidth = sources[0].width * relativeWidth;
  if(relativeHeight) targetHeight = sources[0].height * relativeHeight;

  glUseProgram(program);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glrUniform1i("source[0]", 0);
  glrUniform4f("targetSize", targetWidth, targetHeight, 1.0 / targetWidth, 1.0 / targetHeight);
  glrUniform4f("outputSize", outputWidth, outputHeight, 1.0 / outputWidth, 1.0 / outputHeight);

  glrParameters(sources[0].filter, sources[0].wrap);
  render(sources[0].width, sources[0].height, outputWidth, outputHeight);

  if(history.size() > 0) {
    OpenGLTexture frame = history.takeLast();

    glBindTexture(GL_TEXTURE_2D, frame.texture);
    if(width == frame.width && height == frame.height) {
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, getFormat(), getType(), buffer);
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, format, frame.width = width, frame.height = height, 0, getFormat(), getType(), buffer);
    }

    history.prepend(frame);
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
