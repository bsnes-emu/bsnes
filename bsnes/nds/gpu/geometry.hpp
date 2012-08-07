
  uint32 regGeomStatus();
  uint32 regGeomLoad();
  uint32 regGeomPosition(unsigned index);
  uint32 regGeomNormal(unsigned index);
  uint32 regClipMatrix(unsigned index);
  uint32 regLightMatrix(unsigned index);
  
  void regGeomMaxPointDepth(uint32 data, uint32 mask);
  void regGeomStatus(uint32 data, uint32 mask);


  void loadMatrix(const Matrix& m);
  void multMatrix(const Matrix& m);
  
  void unpackColor(int32* color, uint16 rgb);
  
  void submitVertex();
  void submitTriStrip(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2);
  void submitQuadStrip(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2, ClipSpaceVertex& v3);
  void submitTri(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2);
  void submitQuad(ClipSpaceVertex& v0, ClipSpaceVertex& v1, ClipSpaceVertex& v2, ClipSpaceVertex& v3);
  
  bool addPoly(ClipSpaceVertex **v, unsigned numVerts, bool clip);
  bool addVertex(const ClipSpaceVertex &v);
  bool addPrim(unsigned shape);
  
  struct Light {
    uint1  enable;
    Vector color;
    Vector direction;
    
    Vector eyeVector;
    Vector halfVector;
  };
  
  // Matrix state
  uint2    matrixMode;  enum { mmProjection, mmModelView, mmLitView, mmTexture };
  bool     stackOverflow;
  uint1    projSP;
  uint6    viewSP;
  Matrix   clipMatrix;
  Matrix   textureMatrix;
  Matrix   projMatrix,  projStack[1];
  Matrix   viewMatrix,  viewStack[32];
  Matrix   lightMatrix, lightStack[32];
  
  Vector   vertexResult;
  Vector   normalResult;
  bool     boxResult;
  
  // Lighting and material state
  Light    lights[4];
  Vector   normal;
  Vector   diffuse;
  Vector   ambient;
  Vector   specular;
  Vector   emission;
  uint8    shininess[128];
  bool     useShineTable;
  
  // Primitive state
  struct PS {
    unsigned size;
    uint1    winding;
    uint2    type;          enum { tris, quads, triStrip, quadStrip };
    uint32   attributes;    enum { drawFront = 0x80, drawBack = 0x40 };
    
    uint32   texImage;
    uint16   texPalette;
    uint2    texTransform;  enum { ttNone, ttTexCoord, ttNormal, ttVertex };
    
    ClipSpaceVertex v[4];
  } primitive;
  
  uint32 attributes;
  uint32 texImage;
  uint16 texPalette;
  ClipSpaceVertex vertex;
  
  struct {
    unsigned  x, y, w, h;
  } viewport;
