//SPC7110 decompressor
//original implementation: neviksti
//optimized implementation: cydrak

struct Decompressor {
  SPC7110 &spc7110;

  Decompressor(SPC7110 &spc7110) : spc7110(spc7110) {}

  uint8 input() {
    return spc7110.datarom_read(offset++);
  }

  uint8 output() {
    if(cursor == 0) decompress();
    uint8 data = tile[cursor++];
    cursor &= 8 * bpp - 1;
    return data;
  }

  uint32 deinterleave(uint64 data, unsigned bits) {
    data = data & (1ull << bits) - 1;
    data = 0x5555555555555555ull & (data << bits | data >> 1);
    data = 0x3333333333333333ull & (data | data >> 1);
    data = 0x0f0f0f0f0f0f0f0full & (data | data >> 2);
    data = 0x00ff00ff00ff00ffull & (data | data >> 4);
    data = 0x0000ffff0000ffffull & (data | data >> 8);
    return data | data >> 16;
  }

  uint64 moveToFront(uint64 list, unsigned nibble) {
    for(uint64 n = 0, mask = ~15; n < 64; n += 4, mask <<= 4) {
      if((list >> n & 15) != nibble) continue;
      return list = (list & mask) + (list << 4 & ~mask) + nibble;
    }
    return list;
  }

  void initialize(unsigned mode, unsigned origin) {
    for(auto &root : context) for(auto &node : root) node = {0, 0};
    base = 0;
    range = Max + 1;
    bpp = 1 << mode;
    offset = origin;
    cursor = 0;
    bits = 8;
    read = input();
    read = read << 8 | input();
    write = 0;
    pixels = 0;
    colormap = 0xfedcba9876543210ull;
  }

  void decompress() {
    for(unsigned row = 0; row < 8; row++) {
      for(unsigned pixel = 0; pixel < 8; pixel++) {
        uint64 map = colormap;
        unsigned diff = 0;

        if(bpp > 1) {
          unsigned pa = (bpp == 2 ? pixels >>  2 & 3 : pixels >>  0 & 15);
          unsigned pb = (bpp == 2 ? pixels >> 14 & 3 : pixels >> 28 & 15);
          unsigned pc = (bpp == 2 ? pixels >> 16 & 3 : pixels >> 32 & 15);

          if(pa != pb || pb != pc) {
            unsigned match = pa ^ pb ^ pc;
            diff = 4;
            if((match ^ pc) == 0) diff = 3;
            if((match ^ pb) == 0) diff = 2;
            if((match ^ pa) == 0) diff = 1;
          }

          colormap = moveToFront(colormap, pa);

          map = moveToFront(map, pc);
          map = moveToFront(map, pb);
          map = moveToFront(map, pa);
        }

        for(unsigned plane = 0; plane < bpp; plane++) {
          unsigned bit = bpp > 1 ? 1 << plane : 1 << (pixel & 3);
          unsigned history = bit - 1 & write;
          unsigned set = 0;

          if(bpp == 1) set = pixel >= 4;
          if(bpp == 2) set = diff;
          if(plane >= 2 && history <= 1) set = diff;

          auto &ctx = context[set][bit + history - 1];
          auto &model = evolution[ctx.prediction];
          uint8 lps_offset = range - model.probability;
          bool symbol = read >= (lps_offset << 8);

          write = write << 1 | (symbol ^ ctx.swap);

          if(symbol == MPS) {
            range = lps_offset;
          } else {
            range -= lps_offset;
            read -= lps_offset << 8;
            base += lps_offset;
          }

          while(range <= Max / 2) {
            ctx.prediction = model.next[symbol];

            range <<= 1;
            read <<= 1;
            base <<= 1;

            if(--bits == 0) {
              bits = 8;
              read += input();
            }
          }

          if(symbol == LPS && model.probability > Half) ctx.swap ^= 1;
        }

        unsigned index = write & (1 << bpp) - 1;
        if(bpp == 1) index ^= pixels >> 15 & 1;

        pixels = pixels << bpp | (map >> 4 * index & 15);
      }

      if(bpp == 1) {
        tile[row] = pixels;
      }

      if(bpp == 2) {
        uint32 slice = deinterleave(pixels, 16);
        tile[row * 2 + 0] = slice >> 0;
        tile[row * 2 + 1] = slice >> 8;
      }

      if(bpp == 4) {
        uint32 slice = deinterleave(deinterleave(pixels, 32), 32);
        tile[row * 2 + 0x00] = slice >>  0;
        tile[row * 2 + 0x01] = slice >>  8;
        tile[row * 2 + 0x10] = slice >> 16;
        tile[row * 2 + 0x11] = slice >> 24;
      }
    }
  }

  void serialize(serializer &s) {
    for(auto &root : context) {
      for(auto &node : root) {
        s.integer(node.prediction);
        s.integer(node.swap);
      }
    }

    s.integer(base);
    s.integer(range);
    s.integer(bpp);
    s.integer(offset);
    s.integer(cursor);
    s.integer(bits);
    s.integer(read);
    s.integer(write);
    s.integer(pixels);
    s.integer(colormap);
    s.array(tile);
  }

  enum : unsigned { MPS = 0, LPS = 1 };
  enum : unsigned { One = 0xaa, Half = 0x55, Max = 0xff };

  struct ModelState {
    uint8 probability;
    uint8 next[2];
  };
  static ModelState evolution[53];

  struct Context {
    uint8 prediction;
    uint8 swap;
  } context[5][15];

  uint8 base;
  uint16 range;
  unsigned bpp;
  unsigned offset;
  unsigned cursor;
  unsigned bits;
  uint16 read;
  uint8 write;
  uint64 pixels;
  uint64 colormap;
  uint8 tile[32];
};

Decompressor::ModelState Decompressor::evolution[53] = {
  {0x5a, { 1, 1}}, {0x25, { 2, 6}}, {0x11, { 3, 8}},
  {0x08, { 4,10}}, {0x03, { 5,12}}, {0x01, { 5,15}},

  {0x5a, { 7, 7}}, {0x3f, { 8,19}}, {0x2c, { 9,21}},
  {0x20, {10,22}}, {0x17, {11,23}}, {0x11, {12,25}},
  {0x0c, {13,26}}, {0x09, {14,28}}, {0x07, {15,29}},
  {0x05, {16,31}}, {0x04, {17,32}}, {0x03, {18,34}},
  {0x02, { 5,35}},

  {0x5a, {20,20}}, {0x48, {21,39}}, {0x3a, {22,40}},
  {0x2e, {23,42}}, {0x26, {24,44}}, {0x1f, {25,45}},
  {0x19, {26,46}}, {0x15, {27,25}}, {0x11, {28,26}},
  {0x0e, {29,26}}, {0x0b, {30,27}}, {0x09, {31,28}},
  {0x08, {32,29}}, {0x07, {33,30}}, {0x05, {34,31}},
  {0x04, {35,33}}, {0x04, {36,33}}, {0x03, {37,34}},
  {0x02, {38,35}}, {0x02, { 5,36}},

  {0x58, {40,39}}, {0x4d, {41,47}}, {0x43, {42,48}},
  {0x3b, {43,49}}, {0x34, {44,50}}, {0x2e, {45,51}},
  {0x29, {46,44}}, {0x25, {24,45}},

  {0x56, {48,47}}, {0x4f, {49,47}}, {0x47, {50,48}},
  {0x41, {51,49}}, {0x3c, {52,50}}, {0x37, {43,51}},
};
