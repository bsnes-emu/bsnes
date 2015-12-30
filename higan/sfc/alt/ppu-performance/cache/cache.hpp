struct Cache {
  Cache(PPU& self);

  auto tile_2bpp(uint tile) -> uint8*;
  auto tile_4bpp(uint tile) -> uint8*;
  auto tile_8bpp(uint tile) -> uint8*;
  auto tile(uint bpp, uint tile) -> uint8*;

  auto serialize(serializer&) -> void;

  PPU& self;
  uint8* tiledata[3];
  uint8* tilevalid[3];

  friend class PPU;
};
