struct Video {
  Video();

  auto reset() -> void;
  auto refresh() -> void;

private:
  auto generateColor(uint, double, double, double, double, double) -> uint32;

  unique_pointer<uint32[]> output;
  unique_pointer<uint32[]> paletteLiteral;
  unique_pointer<uint32[]> paletteStandard;
  unique_pointer<uint32[]> paletteEmulation;
};

extern Video video;
