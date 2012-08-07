
  bool gxMatrixMode();
  bool gxMatrixPush();
  bool gxMatrixPop();
  bool gxMatrixStore();
  bool gxMatrixRestore();
  bool gxMatrixLoadIdentity();
  bool gxMatrixLoad4x4();
  bool gxMatrixLoad4x3();
  bool gxMatrixMult4x4();
  bool gxMatrixMult4x3();
  bool gxMatrixRotate();
  bool gxMatrixScale();
  bool gxMatrixTranslate();
  
  bool gxColor();
  bool gxNormal();
  bool gxTexCoord();
  bool gxVertex3i_16();
  bool gxVertex3i_10();
  bool gxVertex2i_XY();
  bool gxVertex2i_XZ();
  bool gxVertex2i_YZ();
  bool gxVertex3i_Rel();
  bool gxAttribute();
  bool gxTexImage();
  bool gxTexPalette();
  
  bool gxLightDiffuseAmbient();
  bool gxLightSpecularEmission();
  bool gxLightDirection();
  bool gxLightColor();
  bool gxLightShininess();
  
  bool gxBeginPrimitive();
  bool gxSwapBuffers();
  bool gxViewport();
  
  bool gxCullTest();
  bool gxPositionTest();
  bool gxDirectionTest();
