struct Video {
  uint32_t* palette = nullptr;
  void generate_palette(Emulator::Interface::PaletteMode mode);

  Video();
  ~Video();

private:
  Emulator::Interface::PaletteMode mode;
  static const uint16 monochrome[4][3];
  uint32_t palette_dmg(unsigned color) const;
  uint32_t palette_sgb(unsigned color) const;
  uint32_t palette_cgb(unsigned color) const;
};

extern Video video;
