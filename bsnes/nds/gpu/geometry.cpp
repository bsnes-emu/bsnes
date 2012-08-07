
void GPU::submitVertex() {
  if(primitive.texTransform == PS::ttVertex) {
    textureMatrix(0,3) = vertex.texCoord[0];
    textureMatrix(1,3) = vertex.texCoord[1];
    
    vertex.texCoord[0] = vertex.position[0];
    vertex.texCoord[1] = vertex.position[1];
    vertex.texCoord[2] = vertex.position[2];
    vertex.texCoord[3] = 0x1000;
    
    transform(textureMatrix, vertex.texCoord);
  }
  
  auto &v = primitive.v;
  auto input = vertex;
  
  transform(clipMatrix, input.position);

  switch(primitive.type) {
  case PS::tris:
    v[primitive.size] = input;
    
    if(++primitive.size >= 3)
      submitTri(v[0], v[1], v[2]);
    break;
    
  case PS::triStrip:
    v[min(2, primitive.size)] = input;
    
    if(++primitive.size >= 3) {
      submitTriStrip(v[0], v[1], v[2]);
      primitive.winding ^= 1;
      v[0] = v[1];
      v[1] = v[2];
    }
    break;
    
  case PS::quads:
    v[primitive.size] = input;
    
    // Slightly different order than triangles and strips
    if(++primitive.size >= 4)
      submitQuad(v[0], v[1], v[2], v[3]);
    break;
    
  case PS::quadStrip:
    if(primitive.size % 2) v[min(3, primitive.size)] = input;
    else                   v[min(2, primitive.size)] = input;
    
    if(++primitive.size >= 4) {
      if(primitive.size % 2 == 0) {
        submitQuadStrip(v[0], v[1], v[2], v[3]);
        v[0] = v[2];
        v[1] = v[3];
      }
    }
    break;
  }
}



void GPU::submitTri(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2) {
  int32 out[] = { outcode(v0), outcode(v1), outcode(v2) };
  int32 face  = facing(v0, v1, v2);
  
  if(primitive.winding == 1)
    face = -face;  // this can happen when clipping triangle strips
  
  bool front  = primitive.attributes & PS::drawFront;
  bool back   = primitive.attributes & PS::drawBack;
  
  bool culled  = face && !(front && face > 0 || back && face < 0);
  bool clipped = out[0] | out[1] | out[2];
  bool reject  = out[0] & out[1] & out[2] | culled;
  
  if(!reject) {
    ClipSpaceVertex *vs[] = { &v0, &v1, &v2 };
    addPoly(vs, 3, clipped);
  }
  primitive.texImage = texImage;
  primitive.texPalette = texPalette;
  primitive.size = 0;
}

void GPU::submitQuad(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2, ClipSpaceVertex& v3) {
  int32 out[] = { outcode(v0), outcode(v1), outcode(v2), outcode(v3) };
  int32 face  = facing(v0, v1, v2);
  
  bool front  = primitive.attributes & PS::drawFront;
  bool back   = primitive.attributes & PS::drawBack;
  
  bool culled  = face && !(front && face > 0 || back && face < 0);
  bool clipped = out[0] | out[1] | out[2] | out[3];
  bool reject  = out[0] & out[1] & out[2] & out[3] | culled;
  
  if(!reject) {
    ClipSpaceVertex *vs[] = { &v0, &v1, &v2, &v3 };
    addPoly(vs, 4, clipped);
  }
  primitive.texImage = texImage;
  primitive.texPalette = texPalette;
  primitive.size = 0;
}



void GPU::submitTriStrip(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2) {
  int32 face = facing(v0, v1, v2);
  
  if(primitive.winding == 1)
    face = -face;
  
  // Clipping is a pain here. The easiest way that comes to mind is to check
  // whether a vertex was clipped. Then if so, split this polygon off and,
  // after clipping, restart the primitive with the current winding order.
  bool front  = primitive.attributes & PS::drawFront;
  bool back   = primitive.attributes & PS::drawBack;
  
  bool culled  = face != 0 && !(front && face > 0 || back && face < 0);
  bool clipped = outcode(v0) | outcode(v1) | outcode(v2);
  
  if(culled || clipped) {
    if(uploadList->numPrims < 2048 && primitive.size > 3)
      uploadList->numPrims++;  // Terminate the current strip (if any)
    
    submitTri(v0, v1, v2);  // Submit as separate triangle
    primitive.size = 2;     // Next vertex restarts the strip
    return;
  }
  if(primitive.size == 3) {
    addPrim(Prim::triStrip);
    addVertex(v0);
    addVertex(v1);
  }
  addVertex(v2);
}

void GPU::submitQuadStrip(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2, ClipSpaceVertex& v3) {
  int32 face  = facing(v0, v1, v3);
  bool front  = primitive.attributes & PS::drawFront;
  bool back   = primitive.attributes & PS::drawBack;
  
  bool culled  = face != 0 && !(front && face > 0 || back && face < 0);
  bool clipped = outcode(v0) | outcode(v1) | outcode(v2) | outcode(v3);
  
  if(culled || clipped) {
    if(uploadList->numPrims < 2048 && primitive.size > 4)
      uploadList->numPrims++;  // Terminate the current strip (if any)
    
    submitQuad(v0, v1, v3, v2);  // Submit as separate quad
    primitive.size = 2;          // Next two vertices restart the strip
    return;
  }
  if(primitive.size == 4) {
    addPrim(Prim::quadStrip);
    addVertex(v0);
    addVertex(v1);
  }
  addVertex(v2);
  addVertex(v3);
}

bool GPU::addPoly(ClipSpaceVertex **v, unsigned numVerts, bool clip) {
  struct Entry {
    ClipSpaceVertex v;  unsigned out;
    Entry() {}
    Entry(const ClipSpaceVertex& s) : v(s), out(outcode(s)) {}
    operator ClipSpaceVertex() const { return v; }
  };
  
  vector<Entry> list[2], *in = list+0, *out = list+1;
  
  for(unsigned i = 0; i < numVerts; i++)
    out->append({ *(v[i]) });
  
  if(clip) {
    for(unsigned plane = 0; plane < 6; plane++) {
      const int axis = plane/2, side = plane%2? +1 : -1;
      
      std::swap(in, out);
      out->reset();
      
      for(unsigned i = 0; i < in->size(); ) {
        auto &a = (*in)[i++];
        auto &b = (*in)[i % in->size()];
        
        if(1<<plane & a.out & b.out) continue;
        if(1<<plane & a.out) out->append({ clipEdge(b, a, axis, side) });
        if(1<<plane & b.out) out->append({ clipEdge(a, b, axis, side) });
        else                 out->append({ b });
      }
    }
  }
  if(!out->size())
    return true;
  
  if(!addPrim(Prim::poly))
    return false;
  
  for(unsigned i = 0; i < out->size(); i++)
    addVertex((*out)[i]);
  
  uploadList->numPrims++;
  return true;
}

bool GPU::addVertex(const ClipSpaceVertex &v) {
  if(uploadList->numPrims == 2048 || uploadList->numVerts == 6144)
    return false;
  
  // XYZ are all in range +/-w in homogenous coordinates
  uploadList->prims[uploadList->numPrims].numVerts++;
  auto &out = uploadList->verts[uploadList->numVerts++];
  
  // xyzw = 20.12
  // uv   = 20.12 (in texels)
  // rgb  = 20.12 (0 - 63.0)
  int32 w = v.position[3];
  
  // Perspective transformation - divide everything by W.
  // This flattens the perspective and allows linear pixel interpolation.
  out.w = w? w : 1;  // still needed to restore UVZ and RGB.
  out.x = (int64) v.position[0] * 0x40000000/w;  // x/w
  out.y = (int64) v.position[1] * 0x40000000/w;  // y/w
  out.z = (int64) v.position[2] * 0x40000000/w;  // z/w
  out.u = (int64) v.texCoord[0] * 0x4000/w;  // u/w
  out.v = (int64) v.texCoord[1] * 0x4000/w;  // v/w
  out.r = (int64)    v.color[0] * 0x4000/w;  // r/w
  out.g = (int64)    v.color[1] * 0x4000/w;  // g/w
  out.b = (int64)    v.color[2] * 0x4000/w;  // b/w
  
  // Apply viewport scaling. X and Y get rounded off somehow. Vertices and
  // interpolants jump around a lot, despite being perspective correct. :(
  out.x = (out.x + 0x40000000ll) * viewport.w/2 + 0x400000*viewport.x >> 8;
  out.y = (out.y + 0x40000000ll) * viewport.h/2 + 0x400000*viewport.y >> 8;
  
  // With perspective projections, W is a linear function of Z. Default then
  // is to use the nonlinear Z/W, giving more precision close up. This setting
  // gives linear depth values instead.
  // 
  // Note: In orthographic/2D views, Z/W is linear anyway because W=1. In that
  // case this option kills Z buffering (kinda bad, given the auto Y sort).
  //if(uploadList->depthUseW)
  //  out.z = max(-0x8000, min(0x7fff, w));  // ?
  return true;
}

bool GPU::addPrim(unsigned shape) {
  if(uploadList->numPrims == 2048)
    return false;
  
  auto &out = uploadList->prims[uploadList->numPrims];
  
  //primitive.texImage   = texImage;
  //primitive.texPalette = texPalette;
  
  out.numVerts     = 0;
  out.firstVert    = uploadList->numVerts;
  out.id           = primitive.attributes >> 24 & 63;
  out.alpha        = primitive.attributes >> 16 & 31;
  out.drawMode     = shape;
  out.drawMode    += Prim::fog    * uint1(primitive.attributes >> 15);
  out.drawMode    += Prim::zequal * uint1(primitive.attributes >> 14);
  out.drawMode    += Prim::zwrite * uint1(primitive.attributes >> 11);
  out.drawMode    += Prim::shadeMask & uint2(primitive.attributes >> 4);
  out.drawMode    += Prim::colorKey * uint1(primitive.texImage >> 29);
  
  out.texFormat    = primitive.texImage >> 26 & 7;
  //out.texSize[1]   = primitive.texImage >> 23 & 7;
  //out.texSize[0]   = primitive.texImage >> 20 & 7;
  //out.texBorder[1] = primitive.texImage >> 17 & 5;
  //out.texBorder[0] = primitive.texImage >> 16 & 5;
  out.texImage     = primitive.texImage;
  out.texPalette   = primitive.texPalette;
  if(out.texFormat != Prim::I2)
    out.texPalette *= 2;
    
  return true;
}


// Matrices, OpenGL style:
//
//   clip   projection  view..      model..    vertex
//   |Cx|   |0 4 8 c|   |0 4 8 c|   |0 4 8 c|   |Vx|
//   |Cy| = |1 5 9 d| * |1 5 9 d| * |1 5 9 d| * |Vy|
//   |Cz|   |2 6 a e|   |2 6 a e|   |2 6 a e|   |Vz|
//   |Cw|   |3 7 b f|   |3 7 b f|   |3 7 b f|   |Vw|
//   
//   transform() multiplies new matrix on the LEFT, ie. m = t*m.

void GPU::loadMatrix(const Matrix& m) {
  if(matrixMode == mmTexture)    textureMatrix = m;
  else {
    if(matrixMode == mmLitView)    lightMatrix = m;
    if(matrixMode == mmProjection) projMatrix = m;
    else /*mmModelView,mmLitView*/ viewMatrix = m;
    
    clipMatrix = projMatrix;
    transform(clipMatrix, viewMatrix);
  }
}

void GPU::multMatrix(const Matrix& m) {
  if(matrixMode == mmTexture)    transform(textureMatrix, m);
  else {
    if(matrixMode == mmLitView)    transform(lightMatrix, m);
    if(matrixMode == mmProjection) transform(projMatrix, m);
    else /*mmModelView,mmLitView*/ transform(viewMatrix, m);
    
    clipMatrix = projMatrix;
    transform(clipMatrix, viewMatrix);
  }
}
