class Colortable {
public:
  enum Format {
    RGB555,
    RGB565,
    RGB888,
  };

  const inline uint32_t operator[](uint16_t index) const { return table[index]; }

  void set_format(Format);
  void set_contrast(signed);
  void set_brightness(signed);
  void set_gamma(signed);
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

  signed contrast;
  signed brightness;
  signed gamma;

  bool gamma_ramp;
  bool sepia;
  bool grayscale;
  bool invert;

  static const uint8_t gamma_ramp_table[32];
  uint8_t contrast_adjust(uint8_t input);
  uint8_t brightness_adjust(uint8_t input);
  uint8_t gamma_adjust(uint8_t input);
};

extern Colortable colortable;
