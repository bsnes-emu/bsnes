struct Video {
  Video();
  ~Video();

  auto generate_palette(Emulator::Interface::PaletteMode mode) -> void;

  uint32* palette = nullptr;

private:
  auto generate_color(uint, double, double, double, double, double) -> uint32;
};

extern Video video;
