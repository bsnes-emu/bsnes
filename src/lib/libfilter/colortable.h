class Colortable {
public:
  enum Format {
    RGB555,
    RGB565,
    RGB888,
  };

  const inline uint32_t operator[](uint16_t index) const { return table[index]; }

  void set_format(Format);
  void set_contrast(int32_t);
  void set_brightness(int32_t);
  void set_gamma(int32_t);
  void enable_gamma_ramp(bool);
  void enable_sepia(bool);
  void enable_grayscale(bool);
  void enable_invert(bool);

  void update();

  Colortable();
  ~Colortable();

private:
  uint32_t *table;
  Format format;

  int32_t contrast;
  int32_t brightness;
  int32_t gamma;

  bool gamma_ramp;
  bool sepia;
  bool grayscale;
  bool invert;

  static const uint8_t gamma_ramp_table[32];
  void contrast_adjust(int32_t &input);
  void brightness_adjust(int32_t &input);
  void gamma_adjust(int32_t &input);
};

extern Colortable colortable;
