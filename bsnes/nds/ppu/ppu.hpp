
struct PPU {
  PPU();
  
  void power();
  void scanline();
  
  struct objinfo;
  
  void renderBgs(unsigned y);
  void renderTiledBg(unsigned no, unsigned mapW, unsigned mapH, unsigned y);
  void renderAffineBg(unsigned no, unsigned mapDepth, unsigned mapW, unsigned mapH, unsigned y);
  void renderBitmapBg(unsigned no, unsigned mapDepth, unsigned mapW, unsigned mapH, unsigned y);
  
  void renderObjs(unsigned y);
  void renderTiledObj (objinfo& obj, unsigned objw, unsigned objh, unsigned y);
  void renderBitmapObj(objinfo& obj, unsigned objw, unsigned objh, unsigned y);
  
  uint32 readPalette(uint32 addr);
  void writePalette(uint32 addr, uint32 size, uint32 data);

  uint32 readOam(uint32 addr);
  void writeOam(uint32 addr, uint32 size, uint32 data);
  
  uint32 regControl();
  void regControl(uint32 data, uint32 mask);

  uint32 regBg(unsigned no);
  void regBg(unsigned no, uint32 data, uint32 mask);
  
  uint32 regBgOffs(unsigned no);
  void regBgOffs(unsigned no, uint32 data, uint32 mask);
  
  void regBgAffine(unsigned no, unsigned index, uint32 data, uint32 mask);
  
  void regWinDims(unsigned index, uint32 data, uint32 mask);
  
  uint32 regWinArea();
  void regWinArea(uint32 data, uint32 mask);
  
  uint32 regBlend();
  void regBlend(unsigned index, uint32 data, uint32 mask);
  
  // PPU #0:                              PPU #1:
  //   Has access to VRAM banks 0..6        More limited features
  //   BG0 can source 3D GPU output         Has access to VRAM banks 2,3,7,8
  //   Can use frame- and render-buffers    Max 128K BG, 128K OBJ
  //   Max 512K BG, 256K OBJ
  int   which;
  uint1 powered;
  
  // Internal pixel format: pplllMBAF aaaaa bbbbbb gggggg rrrrrr
  enum {
    pxPriority    = 1<<30,  // p - from BG control
    pxLayer       = 1<<27,  // l - 1: OBJ, 2..5:BG, 6:backdrop
    pxMosaic      = 1<<26,  // M
    pxBlendBelow  = 1<<25,  // B
    pxBlendAbove  = 1<<24,  // A
    pxBlendForce  = 1<<23,  // F - for 3D/OBJ pixel
    pxAlpha       = 1<<18,  // a - alpha (0=solid)
  };
  
  uint32 output[256];
  
  uint32 above[256 + 16];
  uint32 below[256 + 16];
  uint32 objLayer[256 + 16];
  
  uint32 bgPal[256];
  uint32 objPal[256];
  
  union affineparam {
    struct {
      int16 dx_dh, dx_dv;  // A, B
      int16 dy_dh, dy_dv;  // C, D
    };
    int16 m[4];
  };
  
  struct bginfo {
    uint1 enable;
    uint1 mosaic, depth;
    uint1 affineWrap;
    uint2 priority;
    uint2 size;
    uint5 map;               // 2K offset for maps / 16K for bitmaps
    uint4 tiles;             // 16K offset for tiles
    uint2 palette;           // 8K offset for large palette mode
    
    uint9 hoffs, voffs;      // Tiled scrolling
    int32 originx, originy;  // Affine settings
    int32 linex,   liney;
    affineparam transform;
  };
  
  struct objinfo {
    uint2  kind;        enum { none=0,   blend=1,  window=2, bitmap=3 };
    uint2  renderMode;  enum { normal=0, affine=1, hidden=2, affineDouble=3 };
    uint1  mosaic;
    uint1  depth;
    uint2  size;
    uint2  shape;       enum { square=0, wide=1,   tall=2 };
    uint5  transform;   enum { hflip=8,  vflip=16 };
    int9   x;
    uint8  y;
    uint10 index;
    uint2  priority;
    uint4  palette;        // subpalette for tiled; alpha for bitmap sprites
  };
  
  uint1   forceBlank;
  
  uint1   bgLargePal;      // 256-color BGs use system.vmap.bgpal
  uint1   bg0FromGPU;      // Display 3D rendering on BG0
  uint3   bgMode;          // Determines affine vs. tiled layers
  uint3   bgTileBase;      // 64K starting base for tiles
  uint3   bgMapBase;       // 64K starting base for maps
  uint4   bgMosaicX;
  uint4   bgMosaicY;
  bginfo  bg[4];
  
  uint1   objEnable;
  uint1   objInHBlank;
  uint1   objLargePal;     // 256-color OBJs use system.vmap.objpal
  uint1   objTileMode;     // 0 = 256x256 px sprite sheet, 1 = linear
  uint2   objBitmapMode;   // 0 = 128x256, 1 = 256x256, 2..3 = linear
  uint2   objTileStep;     //   tile index << in linear mode
  uint1   objBitmapStep;   //
  uint4   objMosaicX;
  uint4   objMosaicY;
  objinfo obj[128];
  
  affineparam objTransform[32];
  
  uint1   winEnable[3];    // [win0, win1, winobj, winout]
  uint8   winArea[4];      //   BLD, OBJ, BG3..BG0
  uint8   winX[2][2];      //
  uint8   winY[2][2];      // not uint9 - Nintendo's oversight
  uint8   window[256+16];  //   window no.<<6 | area
  
  uint2   blendMode;       enum { alphaBlend=1, lighten=2, darken=3 };
  uint6   blendAbove;       // BG0..BG3, OBJ, lignten/darken BD
  uint6   blendBelow;       // BG0..BG3, OBJ, blend over BD
  uint5   blendAf, blendBf; // alphaBlend coefficients
  uint5   blendYf;          // lighten/darken coefficient
};

extern PPU ppu[2];
