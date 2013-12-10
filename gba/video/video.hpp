struct Video {
  unsigned* palette;
  void generate_palette(bool color_emulation);

  Video();
  ~Video();

private:
  static const uint8 curve[32];
};

extern Video video;
