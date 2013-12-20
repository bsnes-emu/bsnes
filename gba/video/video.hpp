struct Video {
  uint32_t* palette = nullptr;
  void generate_palette(Emulator::Interface::PaletteMode mode);

  Video();
  ~Video();

private:
  static const uint8 curve[32];
};

extern Video video;
