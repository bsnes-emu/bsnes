struct Video {
  Video();

  auto power() -> void;
  auto refresh() -> void;

  unique_pointer<uint32[]> output;
  unique_pointer<uint32[]> paletteLiteral;
  unique_pointer<uint32[]> paletteStandard;
  unique_pointer<uint32[]> paletteEmulation;
};

extern Video video;
