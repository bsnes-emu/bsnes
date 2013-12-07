struct Video {
  unsigned* palette = nullptr;
  void generate_palette(bool color_emulation);

  Video();
  ~Video();

private:
  uint32_t generate_color(unsigned, double, double, double, double, double);
};

extern Video video;
