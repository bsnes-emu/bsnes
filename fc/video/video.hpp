struct Video {
  uint32_t* palette = nullptr;
  void generate_palette(Emulator::Interface::PaletteMode mode);

  Video();
  ~Video();

private:
  uint32_t generate_color(unsigned, double, double, double, double, double);
};

extern Video video;
