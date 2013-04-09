void OpenGLSurface::size(unsigned w, unsigned h) {
  if(width == w && height == h) return;
  width = w, height = h;
  w = glrSize(w), h = glrSize(h);

  if(texture) { glDeleteTextures(1, &texture); texture = 0; }
  if(buffer) { delete[] buffer; buffer = nullptr; }

  buffer = new uint32_t[w * h]();
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

  if(framebuffer) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    delete[] buffer;
    buffer = nullptr;
  }
}

void OpenGLSurface::release() {
  if(vertex) { glDetachShader(program, vertex); glDeleteShader(vertex); vertex = 0; }
  if(geometry) { glDetachShader(program, geometry); glDeleteShader(geometry); geometry = 0; }
  if(fragment) { glDetachShader(program, fragment); glDeleteShader(fragment); fragment = 0; }
  if(texture) { glDeleteTextures(1, &texture); texture = 0; }
  if(framebuffer) { glDeleteFramebuffers(1, &framebuffer); framebuffer = 0; }
  if(program) { glDeleteProgram(program); program = 0; }
  width = 0, height = 0;
}
