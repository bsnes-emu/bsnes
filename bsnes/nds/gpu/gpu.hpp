
struct GPU {
  GPU();
  
  void power();
  void scanline();
  void swapBuffers();
  
  void sendGeomBuffered(uint32 data);
  void sendGeomImmediate(uint8 command, uint32 data);
  bool geomCommand(uint8 command);
  
  // Once the geometry submitted by the program gets through transform,
  // lighting, culling, and clipping, we've built a SceneList with:
  struct Vertex {
    // A single point within a primitive.
    int64  x, y, z;  // normalized device coordinates (xyz/w => -1..+1)
    int32  u, v;     // texture coordinates (u/w, v/w)
    int32  r, g, b;  // color (r/w, g/w, b/w)
    int32  w;        //
  };
  struct Prim {
    // A strip of triangles or quads - or a single, clipped n-gon.
    // AFAIK, none of these attributes can change between primitives.
    uint16 firstVert, numVerts;
    uint8  minY, maxY;
    uint1  blend, wire;
    uint8  drawMode;      enum { shadeMask=0x03, normal=0, decal=1, toon=2, stencil=3, 
                                 poly=0, triStrip=0x04, quadStrip=0x08,
                                 colorKey=0x10, zequal=0x20, zwrite=0x40, fog=0x80 };
    uint8  id, alpha;
    uint8  texFormat;     enum { untextured, A3_I5, I2,I4,I8, packed, A5_I3, A1_RGB15 };
    uint8  texBorder[2];  enum { clamp=0, repeat=1, clamp_=4, mirror=5 };
    uint8  texSize[2];    // 8 << texSize
    
    uint16 texPalette;
    uint32 texImage;
  };
  
  struct SceneList {
    unsigned numVerts;
    unsigned numPrims;
    unsigned firstAlpha;
    
    Vertex   verts[6144];
    Prim     prims[2048];
    uint16   order[2048];  // by solid first, top Y, bottom Y, then index
    
    uint1    alphaSort;  // Sort transparent geometry by Y coordinate?
    uint1    depthUseW;  // Use Z or W for depth buffering?
  };
  
  uint1     powered;
  uint2     swapArgument;
  SceneList sceneList[2];
  SceneList *uploadList;
  SceneList *renderList;
  
  bool     sceneFinished;
  uint2    commandBufIrq;
  int32    renderedLines;
  uint32   packedCommands;
  uint32   args[32], numArgs;
  
  uint32   output[256*192];  // ARGB5666
  
  struct Matrix;
  struct Vector;
  struct ClipSpaceVertex;
  
  #include "math.hpp"
  #include "commands.hpp"
  #include "geometry.hpp"
  #include "render.hpp"
};

extern GPU gpu;
