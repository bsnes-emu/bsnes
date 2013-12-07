struct Video {
  unsigned* palette;
  void generate_palette(bool color_emulation);

  Video();
  ~Video();
};

extern Video video;
