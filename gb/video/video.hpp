struct Video {
  Video();
  ~Video();

  auto generate_palette(Emulator::Interface::PaletteMode mode) -> void;

  uint32* palette = nullptr;

private:
  Emulator::Interface::PaletteMode mode;
  static const uint16 monochrome[4][3];
  auto paletteDMG(uint color) const -> uint;
  auto paletteSGB(uint color) const -> uint;
  auto paletteCGB(uint color) const -> uint;
};

extern Video video;
