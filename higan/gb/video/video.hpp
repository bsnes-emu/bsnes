struct Video {
  Video();
  ~Video();

  auto power() -> void;
  auto refresh() -> void;

  uint32* output = nullptr;
  uint32* paletteStandard = nullptr;
  uint32* paletteEmulation = nullptr;

private:
  static const uint16 monochrome[4][3];
};

extern Video video;
