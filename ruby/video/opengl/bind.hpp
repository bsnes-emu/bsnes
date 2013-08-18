#if !defined(PLATFORM_OSX)

PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLVALIDATEPROGRAMPROC glValidateProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLGETUNIFORMIVPROC glGetUniformiv = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM2FPROC glUniform2f = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM4FPROC glUniform4f = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;

static bool OpenGLBind() {
  #define bind(prototype, function) \
    function = (prototype)glGetProcAddress(#function); \
    if(function == nullptr) return false

  bind(PFNGLCREATEPROGRAMPROC, glCreateProgram);
  bind(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
  bind(PFNGLUSEPROGRAMPROC, glUseProgram);
  bind(PFNGLCREATESHADERPROC, glCreateShader);
  bind(PFNGLDELETESHADERPROC, glDeleteShader);
  bind(PFNGLSHADERSOURCEPROC, glShaderSource);
  bind(PFNGLCOMPILESHADERPROC, glCompileShader);
  bind(PFNGLGETSHADERIVPROC, glGetShaderiv);
  bind(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
  bind(PFNGLATTACHSHADERPROC, glAttachShader);
  bind(PFNGLDETACHSHADERPROC, glDetachShader);
  bind(PFNGLLINKPROGRAMPROC, glLinkProgram);
  bind(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
  bind(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
  bind(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
  bind(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
  bind(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
  bind(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
  bind(PFNGLGENBUFFERSPROC, glGenBuffers);
  bind(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
  bind(PFNGLBINDBUFFERPROC, glBindBuffer);
  bind(PFNGLBUFFERDATAPROC, glBufferData);
  bind(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
  bind(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
  bind(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
  bind(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
  bind(PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation);
  bind(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
  bind(PFNGLGETUNIFORMIVPROC, glGetUniformiv);
  bind(PFNGLUNIFORM1IPROC, glUniform1i);
  bind(PFNGLUNIFORM1FPROC, glUniform1f);
  bind(PFNGLUNIFORM2FPROC, glUniform2f);
  bind(PFNGLUNIFORM2FVPROC, glUniform2fv);
  bind(PFNGLUNIFORM4FPROC, glUniform4f);
  bind(PFNGLUNIFORM4FVPROC, glUniform4fv);
  bind(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  bind(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
  bind(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers);
  bind(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
  bind(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
  bind(PFNGLACTIVETEXTUREPROC, glActiveTexture);

  #undef bind

  return true;
}

#else

static bool OpenGLBind() {
  return true;
}

#endif
