struct Video {
  Video();
  ~Video();

  auto reset() -> void;
  auto refresh() -> void;

  uint32* output = nullptr;
  uint32* paletteStandard = nullptr;
  uint32* paletteEmulation = nullptr;

private:
  auto drawCursor(uint32 color, int x, int y) -> void;
  auto drawCursors() -> void;

  static const uint8 gammaRamp[32];
  static const uint8 cursor[15 * 15];

  friend class System;
};

extern Video video;
