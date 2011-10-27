struct Video {
  enum class Format : unsigned { RGB30, RGB24, RGB16, RGB15 };
  unsigned *palette;

  unsigned palette30(unsigned, double, double, double, double, double);
  void generate(Format format);
  Video();
  ~Video();
};

extern Video video;
