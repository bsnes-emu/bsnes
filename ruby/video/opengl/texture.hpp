GLuint OpenGLTexture::getFormat() const {
  if(format == GL_R32I) return GL_RED_INTEGER;
  if(format == GL_R32UI) return GL_RED_INTEGER;
  return GL_BGRA;
}

GLuint OpenGLTexture::getType() const {
  if(format == GL_R32I) return GL_UNSIGNED_INT;
  if(format == GL_R32UI) return GL_UNSIGNED_INT;
  if(format == GL_RGB10_A2) return GL_UNSIGNED_INT_2_10_10_10_REV;
  return GL_UNSIGNED_INT_8_8_8_8_REV;
}
