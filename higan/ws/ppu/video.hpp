struct Video {
  Video();

  auto power() -> void;
  auto refresh() -> void;

private:
  unique_pointer<uint32[]> output;
  unique_pointer<uint32[]> paletteLiteral;
  unique_pointer<uint32[]> paletteStandard;
};

extern Video video;
