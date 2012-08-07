
bool GPU::gxMatrixMode() {
  if(numArgs < 1) return false;
  
  matrixMode = args[0];
  return true;
}

bool GPU::gxMatrixPush() {
  if(matrixMode == mmTexture) {
    return true;
  }
  if(matrixMode == mmProjection) {
    projStack[0] = projMatrix;
    if(projSP++ == 1) { stackOverflow = true; }
    return true;
  }
  // mmModelView or mmLitView
  lightStack[viewSP % 32] = lightMatrix;
  viewStack[viewSP % 32] = viewMatrix;
  
  if(viewSP++ >= 31) { stackOverflow = true; }
  return true;
}

bool GPU::gxMatrixPop() {
  if(numArgs < 1) return false;
  
  if(matrixMode == mmTexture) {
    return true;
  }
  if(matrixMode == mmProjection) {
    if(projSP-- == 0) { stackOverflow = true; }
    projMatrix = projStack[0];
    return true;
  }
  // mmModelView or mmLitView
  args[0] %= 64;
  viewSP -= args[0];
  if(viewSP >= 31) { stackOverflow = true; }
  
  lightMatrix = lightStack[viewSP % 32];
  viewMatrix = viewStack[viewSP % 32];
  return true;
}

bool GPU::gxMatrixStore() {
  if(numArgs < 1) return false;
  
  if(matrixMode == mmTexture) {
    return true;
  }
  if(matrixMode == mmProjection) {
    projStack[0] = projMatrix;
    return true;
  }
  // mmModelView or mmLitView
  args[0] %= 32;
  if(args[0] == 31) { stackOverflow = true; }
  
  lightStack[args[0]] = lightMatrix;
  viewStack[args[0]]  = viewMatrix;
  return true;
}

bool GPU::gxMatrixRestore() {
  if(numArgs < 1) return false;
  
  if(matrixMode == mmTexture) {
    return true;
  }
  if(matrixMode == mmProjection) {
    projMatrix  = projStack[0];
    clipMatrix  = projMatrix;
    transform(clipMatrix, viewMatrix);
    return true;
  }
  // mmModelView or mmLitView
  args[0] %= 32;
  if(args[0] == 31) { stackOverflow = true; }
  
  lightMatrix = lightStack[args[0]];
  viewMatrix  = viewStack[args[0]];
  clipMatrix  = projMatrix;
  transform(clipMatrix, viewMatrix);
  return true;
}

bool GPU::gxMatrixLoadIdentity() {
  loadMatrix({ 0x1000,      0,      0,      0,
                    0, 0x1000,      0,      0,
                    0,      0, 0x1000,      0,
                    0,      0,      0, 0x1000 });
  return true;
}

// Storage is transposed to make each row contiguous, which is opposite
// of OpenGL. This allows easy multiplication with column vectors.

bool GPU::gxMatrixLoad4x4() {
  if(numArgs < 16) return false;
  
  loadMatrix({ args[ 0], args[ 4], args[ 8], args[12],
               args[ 1], args[ 5], args[ 9], args[13],
               args[ 2], args[ 6], args[10], args[14],
               args[ 3], args[ 7], args[11], args[15] });
  return true;
}

bool GPU::gxMatrixLoad4x3() {
  if(numArgs < 12) return false;
  
  loadMatrix({ args[ 0], args[ 3], args[ 6], args[ 9],
               args[ 1], args[ 4], args[ 7], args[10],
               args[ 2], args[ 5], args[ 8], args[11],
                      0,        0,        0,   0x1000 });
  return true;
}

bool GPU::gxMatrixMult4x4() {
  if(numArgs < 16) return false;
  
  multMatrix({ args[ 0], args[ 4], args[ 8], args[12],
               args[ 1], args[ 5], args[ 9], args[13],
               args[ 2], args[ 6], args[10], args[14],
               args[ 3], args[ 7], args[11], args[15] });
  return true;
}

bool GPU::gxMatrixMult4x3() {
  if(numArgs < 12) return false;

  multMatrix({ args[ 0], args[ 3], args[ 6], args[ 9],
               args[ 1], args[ 4], args[ 7], args[10],
               args[ 2], args[ 5], args[ 8], args[11],
                      0,        0,        0,   0x1000 });
  return true;
}

bool GPU::gxMatrixRotate() {
  if(numArgs < 9) return false;
  
  multMatrix({ args[0], args[3], args[6],      0,
               args[1], args[4], args[7],      0,
               args[2], args[5], args[8],      0,
                     0,       0,       0, 0x1000 });
  return true;
}

bool GPU::gxMatrixScale() {
  if(numArgs < 3) return false;
  
  // Don't scale the lighting matrix
  unsigned prevMode = matrixMode;
  if(matrixMode == mmLitView)
    matrixMode = mmModelView;
  
  multMatrix({ args[0],       0,       0,      0,
                     0, args[1],       0,      0,
                     0,       0, args[2],      0,
                     0,       0,       0, 0x1000 });
  matrixMode = prevMode;
  return true;
}

bool GPU::gxMatrixTranslate() {
  if(numArgs < 3) return false;

  multMatrix({  0x1000,       0,       0, args[0],
                     0,  0x1000,       0, args[1],
                     0,       0,  0x1000, args[2],
                     0,       0,       0,  0x1000 });
  return true;
}



void GPU::unpackColor(int32* color, uint16 rgb) {
  // Yes, it's really this crazy. PPU and GPU colors do NOT match!
  // Super annoying when trying to hide things or make them seamless.
  int r = 2*(rgb>> 0 & 31);  r += (r>0);
  int g = 2*(rgb>> 5 & 31);  g += (g>0);
  int b = 2*(rgb>>10 & 31);  b += (b>0);
  
  color[0] = 0x1000*r;
  color[1] = 0x1000*g;
  color[2] = 0x1000*b;
}

bool GPU::gxColor() {
  if(numArgs < 1) return false;
  
  unpackColor(vertex.color, args[0]);
  return true;
}

bool GPU::gxNormal() {
  if(numArgs < 1) return false;
  
  // Normalize to +/- 1 range
  normal[0] = int10(args[0]>> 0) << 3;
  normal[1] = int10(args[0]>>10) << 3;
  normal[2] = int10(args[0]>>20) << 3;
  normal[3] = 0;
  
  if(primitive.texTransform == PS::ttNormal) {
    // Used for environment mapping.
    textureMatrix(0,3) = vertex.texCoord[0];
    textureMatrix(1,3) = vertex.texCoord[1];
    
    vertex.texCoord[0] = normal[0];
    vertex.texCoord[1] = normal[1];
    vertex.texCoord[2] = normal[2];
    vertex.texCoord[3] = 0x1000;
    
    transform(textureMatrix, vertex.texCoord);
  }
  
  transform(lightMatrix, normal);
  
  vertex.color = emission;
  
  for(auto& light : lights) {
    if(light.enable == false)
      continue;
    
    int32 dl = max(0, -dot(light.eyeVector, normal));
    int32 sl = max(0, -dot(light.halfVector, normal));
    
    sl = min(0xfff, sl*sl / 0x1000);
    if(useShineTable)
      sl = shininess[sl>>5] << 4;
    
    for(unsigned n = 0; n < 3; n++) {
      vertex.color[n] += (int64) specular[n] * light.color[n] *    sl / 0x8000000;
      vertex.color[n] += (int64)  diffuse[n] * light.color[n] *    dl / 0x8000000;
      vertex.color[n] += (int64)  ambient[n] * light.color[n] * 0x200 / 0x8000000;  // ??
    }
  }
  return true;
}

bool GPU::gxTexCoord() {
  if(numArgs < 1) return false;

  vertex.texCoord[0] = 0x1000 * int16(args[0]>> 0);
  vertex.texCoord[1] = 0x1000 * int16(args[0]>>16);
  vertex.texCoord[2] = 0x1000;
  vertex.texCoord[3] = 0x1000;
  
  // Affine texture transformations
  if(primitive.texTransform == PS::ttTexCoord)
    transform(textureMatrix, vertex.texCoord);
  
  //if(primitive.texTransform == PS::ttNormal || primitive.texTransform == PS::ttVertex) {
  //  // Plug in U,V for environment/vertex mapping
  //  textureMatrix(0,3) = vertex.texCoord[0];
  //  textureMatrix(1,3) = vertex.texCoord[1];
  //}
  return true;
}

bool GPU::gxVertex3i_16() {
  if(numArgs < 2) return false;
  
  // 4.12, +/- 16 range
  vertex.position[0] = int16(args[0]>> 0);
  vertex.position[1] = int16(args[0]>>16);
  vertex.position[2] = int16(args[1]>> 0);
  submitVertex();
  return true;
}

bool GPU::gxVertex3i_10() {
  if(numArgs < 1) return false;
  
  // 4.6, +/- 16 range
  vertex.position[0] = int10(args[0]>> 0) << 6;
  vertex.position[1] = int10(args[0]>>10) << 6;
  vertex.position[2] = int10(args[0]>>20) << 6;
  submitVertex();
  return true;
}

bool GPU::gxVertex2i_XY() {
  if(numArgs < 1) return false;
  
  // Use same Z as before
  vertex.position[0] = int16(args[0]>> 0);
  vertex.position[1] = int16(args[0]>>16);
  submitVertex();
  return true;
}

bool GPU::gxVertex2i_XZ() {
  if(numArgs < 1) return false;
  
  // Use same Y as before
  vertex.position[0] = int16(args[0]>> 0);
  vertex.position[2] = int16(args[0]>>16);
  submitVertex();
  return true;
}

bool GPU::gxVertex2i_YZ() {
  if(numArgs < 1) return false;
  
  // Use same X as before
  vertex.position[1] = int16(args[0]>> 0);
  vertex.position[2] = int16(args[0]>>16);
  submitVertex();
  return true;
}

bool GPU::gxVertex3i_Rel() {
  if(numArgs < 1) return false;
  
  // 0.10, previous vertex +/- 0.125 relative range
  vertex.position[0] = int16(vertex.position[0] + int10(args[0]>> 0));
  vertex.position[1] = int16(vertex.position[1] + int10(args[0]>>10));
  vertex.position[2] = int16(vertex.position[2] + int10(args[0]>>20));
  submitVertex();
  return true;
}



bool GPU::gxAttribute() {
  if(numArgs < 1) return false;
  
  for(unsigned n = 0; n < 4; n++)
    lights[n].enable = args[0] >> n;
  
  attributes = args[0];
  return true;
}

bool GPU::gxTexImage() {
  if(numArgs < 1) return false;
  
  texImage = args[0] & 0x3fffffff;
  primitive.texTransform = args[0] >> 30;
  
  //if(primitive.type == PS::tris || primitive.type == PS::quads)
  //  primitive.texImage = texImage;
  return true;
}

bool GPU::gxTexPalette() {
  if(numArgs < 1) return false;
  
  texPalette = args[0] & 0x1fff;
  
  //if(primitive.type == PS::tris || primitive.type == PS::quads)
  //  primitive.texPalette = texPalette;
  return true;
}



bool GPU::gxLightDiffuseAmbient() {
  if(numArgs < 1) return false;
  
  unpackColor(diffuse, args[0] >> 0);
  unpackColor(ambient, args[0] >> 16);
  
  // This is meant to allow display lists to be ignorant of
  // the lighting state (namely whether it's in use or not).
  if(args[0] & 1<<15)
    unpackColor(vertex.color, args[0]);
  return true;
}

bool GPU::gxLightSpecularEmission() {
  if(numArgs < 1) return false;
  
  unpackColor(specular, args[0] >> 0);
  unpackColor(emission, args[0] >> 16);
  
  useShineTable = args[0] & 1<<15;
  return true;
}

bool GPU::gxLightDirection() {
  if(numArgs < 1) return false;
  
  unsigned no = args[0] >> 30;
  auto &light = lights[no];
  
  light.direction[0] = int10(args[0] >> 0);
  light.direction[1] = int10(args[0] >>10);
  light.direction[2] = int10(args[0] >>20);
  light.direction[3] = 0;
  
  light.eyeVector = light.direction;
  transform(lightMatrix, light.eyeVector);
  
  light.halfVector[0] = (light.eyeVector[0] +      0)/2;
  light.halfVector[1] = (light.eyeVector[1] +      0)/2;
  light.halfVector[2] = (light.eyeVector[2] - 0x1000)/2;
  light.halfVector[3] = 0;
  return true;
}

bool GPU::gxLightColor() {
  if(numArgs < 1) return false;
  
  unsigned no = args[0] >> 30;
  
  unpackColor(lights[no].color, args[0]);
  return true;
}

bool GPU::gxLightShininess() {
  if(numArgs < 32) return false;
  
  for(unsigned n = 0; n < 128; n += 4) {
    shininess[n+0] = args[n/4] >> 0;
    shininess[n+1] = args[n/4] >> 8;
    shininess[n+2] = args[n/4] >> 16;
    shininess[n+3] = args[n/4] >> 24;
  }
  return true;
}



bool GPU::gxBeginPrimitive() {
  if(numArgs < 1) return false;
  
  if(uploadList->numPrims < 2048
    && primitive.size >= (primitive.type==PS::quadStrip? 4 : 3))
      uploadList->numPrims++;  // terminate the preceding strip
  
  primitive.type       = args[0];
  primitive.size       = 0;
  primitive.winding    = 0;
  primitive.attributes = attributes;
  primitive.texImage   = texImage;
  primitive.texPalette = texPalette;
  return true;
}

bool GPU::gxSwapBuffers() {
  if(numArgs < 1) return false;
  
  sceneFinished = true;
  swapArgument = args[0];
  return true;
}

bool GPU::gxViewport() {
  if(numArgs < 1) return false;
  
  viewport.x = (args[0]>> 0 & 0xff) + 0;
  viewport.y = (args[0]>> 8 & 0xff) + 0;
  viewport.w = (args[0]>>16 & 0xff) + 1 - viewport.x;
  viewport.h = (args[0]>>24 & 0xff) + 1 - viewport.y;
  return true;
}



bool GPU::gxCullTest() {
  if(numArgs < 3) return false;
  
  // Box range given as x, y, z, w, h, d
  int16 a[6] = {
    int16(args[0]>> 0), int16(args[0]>>16), int16(args[1]>> 0),
    int16(args[1]>>16), int16(args[2]>> 0), int16(args[2]>>16),
  };
  // -> l, b, n, r, t, f
  a[3] += a[0];  a[4] += a[1];  a[5] += a[2];
  
  // Generate all 8 corners and outcodes
  int32 out[8];
  ClipSpaceVertex vertex[8] = {     // X Y Z
    {{ a[0], a[1], a[2], 0x1000 }}, // l,b,n
    {{ a[0], a[1], a[5], 0x1000 }}, // l,b,f
    {{ a[0], a[4], a[2], 0x1000 }}, // l,t,n
    {{ a[0], a[4], a[5], 0x1000 }}, // l,t,f
    {{ a[3], a[1], a[2], 0x1000 }}, // r,b,n
    {{ a[3], a[1], a[5], 0x1000 }}, // r,b,f
    {{ a[3], a[4], a[2], 0x1000 }}, // r,t,n
    {{ a[3], a[4], a[5], 0x1000 }}, // r,t,f
  };
  for(unsigned n = 0; n < 8; n++) {
    transform(clipMatrix, vertex[n].position);
    out[n] = outcode(vertex[n]);
  }
  
  // Find out which sides of the box would be drawn
  bool front  = (out[0] & out[2] & out[4] & out[6]) == 0;
  bool back   = (out[1] & out[3] & out[5] & out[7]) == 0;
  bool bottom = (out[0] & out[1] & out[4] & out[5]) == 0;
  bool top    = (out[2] & out[3] & out[6] & out[7]) == 0;
  bool left   = (out[0] & out[1] & out[2] & out[3]) == 0;
  bool right  = (out[4] & out[5] & out[6] & out[7]) == 0;
  
  // True if at least one face was accepted
  boxResult = front || back || bottom || top || left || right;
  return true;
}

bool GPU::gxPositionTest() {
  if(numArgs < 2) return false;

  vertex.position[0] = int16(args[0]>> 0);
  vertex.position[1] = int16(args[0]>>16);
  vertex.position[2] = int16(args[1]>> 0);
  
  vertexResult = vertex.position;
  transform(clipMatrix, vertexResult);
  return true;
}

bool GPU::gxDirectionTest() {
  if(numArgs < 1) return false;
  
  normalResult[0] = int10(args[0]>> 0) << 3;
  normalResult[1] = int10(args[0]>>10) << 3;
  normalResult[2] = int10(args[0]>>20) << 3;
  normalResult[3] = 0;
  
  transform(lightMatrix, normalResult);
  return true;
}
