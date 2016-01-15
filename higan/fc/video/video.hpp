struct Video {
  Video();
  ~Video();

  auto reset() -> void;
  auto refresh() -> void;

  uint32* output = nullptr;
  uint32* paletteStandard = nullptr;
  uint32* paletteEmulation = nullptr;

private:
  auto generateColor(uint, double, double, double, double, double) -> uint32;
};

extern Video video;
