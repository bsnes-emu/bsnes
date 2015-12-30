struct Video {
  Video();
  ~Video();

  auto generatePalette(Emulator::Interface::PaletteMode mode) -> void;

  uint32* palette = nullptr;

private:
  static const uint8 curve[32];
};

extern Video video;
