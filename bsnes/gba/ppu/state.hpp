struct Pixel {
  bool enable;
  bool translucent;
  unsigned priority;
  unsigned color;
} layer[6][240];

bool windowmask[3][240];

struct Object {
  uint8  y;
  uint1  affine;
  uint1  affinesize;
  uint2  mode;
  uint1  mosaic;
  uint1  colors;      //0 = 16, 1 = 256
  uint2  shape;       //0 = square, 1 = horizontal, 2 = vertical

  uint9  x;
  uint5  affineparam;
  uint1  hflip;
  uint1  vflip;
  uint2  size;

  uint10 character;
  uint2  priority;
  uint4  palette;

  //ancillary data
  unsigned width;
  unsigned height;
} object[128];

struct ObjectParam {
  int16 pa;
  int16 pb;
  int16 pc;
  int16 pd;
} objectparam[32];

struct Tile {
  uint10 character;
  uint1  hflip;
  uint1  vflip;
  uint4  palette;
};
