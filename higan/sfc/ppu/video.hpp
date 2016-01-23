struct Video {
  Video();
  ~Video();

  auto reset() -> void;
  auto refresh() -> void;

private:
  auto drawCursor(uint32 color, int x, int y) -> void;
  auto drawCursors() -> void;

  uint32* output = nullptr;
  uint32* paletteLiteral = nullptr;
  uint32* paletteStandard = nullptr;
  uint32* paletteEmulation = nullptr;
};
