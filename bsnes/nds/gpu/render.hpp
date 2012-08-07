  
  struct ActivePoly;
  union Interpolants;
  struct DrawList;
  
  void updateDrawList(int y, DrawList& list, bool sorted);
  void setupPoly(ActivePoly& poly, Prim* prim, Vertex *vs, unsigned numVerts);
  void setupNextEdge(ActivePoly& poly, unsigned& vi, unsigned& next, int side, Interpolants& edge, Interpolants& gradient);
  void assignEdge(Interpolants& edge, Vertex* v);
  
  void renderScanline();
  void renderDrawList(int y, DrawList& list, bool blend);
  
  uint32 regRenderOptions();
  uint32 regRenderLoad();
  
  void regRenderOptions(uint32 data, uint32 mask);
  void regRenderMinAlpha(uint32 data, uint32 mask);
  void regRenderClearColor(uint32 data, uint32 mask);
  void regRenderClearCoord(uint32 data, uint32 mask);
  void regRenderFogColor(uint32 data, uint32 mask);
  void regRenderFogCoord(uint32 data, uint32 mask);
  void regRenderFogTable(unsigned index, uint32 data, uint32 mask);
  void regRenderEdgeTable(unsigned index, uint32 data, uint32 mask);
  void regRenderToonTable(unsigned index, uint32 data, uint32 mask);
  
  struct Texture {
    // Lookup data - 44 significant bits
    uint64 key;         // paladdr<<32 | format<<16 | texaddr<<0
    uint32 imageBase, indexBase, colorBase;
    
    // Mirrored textures:  clamp at 2*size, wrap at 2*size, cache all 4 mirrors
    // Repeating textures: clamp dt 1*size, wrap at 1*size
    // Clamped textures:   clamp at 1*size, wrap at UINT_MAX
    int32 width, height;
    int32 umask, vmask;
    uint32 *image;
    
    // Address ranges for dirty checks
    uint32 texelRange[2];  // pixel data
    uint32 indexRange[2];  // for compressed format
    uint32 colorRange[2];  // for paletted formats
    
    ~Texture() { delete[] image; }
    Texture(uint64 key, int width, int height) : key(key),
      width(width), height(height), umask(~0), vmask(~0), image(nullptr) {}
  };
  
  struct TexCache {
    void reset();
    void flushDirty(unsigned bank);
    bool checkDirty(Texture *texture, uint32 *range, VRAMMapping *map, uint32 mask);
    
    void add(Texture *t);
    Texture *get(uint64 key);
    
    enum { tableSize = 1<<10 };
    vector<Texture*> table[tableSize];
    int size;
  } texCache;
  
  Texture *getTexture(uint32 texImage, uint16 texPalette);
  void mirrorTexture(Texture *tex, int width, int height);
  void convertPacked(Texture *tex, int width, int height);
  void convertI2(Texture *tex, int width, int height, bool colorKey);
  void convertI3(Texture *tex, int width, int height);
  void convertI4(Texture *tex, int width, int height, bool colorKey);
  void convertI5(Texture *tex, int width, int height);
  void convertI8(Texture *tex, int width, int height, bool colorKey);
  void convertARGB(Texture *tex, int width, int height);
  
  
  union Interpolants {
    int64& operator[](unsigned i) { return val[i]; }
    
    int64 val[8];  enum { count=8 };
    struct {
      int64 r, g, b, u, v, z, inv_w, x;
    };
  };
  
  struct ActivePoly {
    Prim         *p;
    uint8        numVerts;
    int8         side[8];            // is vertex on left or right?
    Vertex       *v[8];
    Interpolants left,    right;     // current values at l, r
    Interpolants lnext,   rnext;
    Interpolants dl_dv,   dr_dv;     // gradient down each side
    unsigned     lv,      rv;        // index of current vertex
    unsigned     next_lv, next_rv;   // index of next vertex
    int32        prev_lx, prev_rx;   // x span on previous line
  };
  
  struct Pixel {
    // Z for depth sorting. We need two layers (!) to make edge smoothing
    // independent of draw order. It also saves the trouble of blending every
    // pixel drawn - the worst case is a single pass at the end.
    int32 az, bz;     // at least 24 bits? 15 bit had too much Z fighting :/
    int32 sz;         // shadow buffer z
    struct {
      uint8 r, g, b;  // 6 bits
      uint8 a;        // 5 bits
      
      // Object ID is a kludge Nintendo came up with to deal with edge fill.
      // They chose to make all polygon edges overlap, so their antialiasing
      // looks right. However, this means translucent edge pixels would be
      // drawn twice. The object ID is used to suppress that.
      uint8 id;
    } a, b;
    
    // Some miscellaneous bits, only one layer here.
    // There have been comments to the effect that multiple stencil bits are
    // needed, but I don't think that's true - shadow volumes are drawn one
    // at a time with the stencil cleared in-between, and the object ID
    // prevents shadows blending more than once per pixel per light.
    uint8 flags; enum {
      fog     = 1<<7,  // apply post pass Z-based fogging
      edge    = 1<<6,  // apply post pass toon edge filter
      stencil = 1<<5,  // pixels can be drawn here in stencil mode
      blends  = 1<<4,  // if true, treat a as alpha; else as coverage
      };
  };
  
  struct RC {
    uint1  texturing;
    uint1  toonShading;  enum { darken, lighten };
    uint2  edgeMode;     enum { solid, smooth, toon, blended };
    uint2  fogMode;      enum { color=2, alpha=3 };
    uint4  fogLevel;
    uint1  alphaTest;
    uint1  alphaBlend;
    uint1  backImage;
  } renderControl;
  
  struct DrawList {
    unsigned   first,    last;
    unsigned   nextPrim, lastPrim;
    ActivePoly polys[6144];
  } drawList[2]; // 2 passes - solid and translucent
  
  uint5 minAlpha;
  
  uint6 backColor[3];
  uint5 backAlpha;
  uint6 backId;
  uint8 backX, backY;
  uint15 backZ;
  uint1 backFog;
  
  int8  fogTable[32+1];
  uint6 fogColor[3];
  uint5 fogAlpha;
  int16 fogZ;
  
  bool  stencilMode;
  
  Vector toonTable[2][32];
  Vector edgeTable[8];
  
  // Need two buffers here for edge transparency
  // Need three for toon edges, argh. Will redo later.
  Pixel pixels[4][256+2];
