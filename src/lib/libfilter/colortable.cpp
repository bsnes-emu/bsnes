Colortable colortable;

void Colortable::set_format(Format format_) { format = format_; }
void Colortable::set_contrast(signed contrast_) { contrast = contrast_; }
void Colortable::set_brightness(signed brightness_) { brightness = brightness_; }
void Colortable::set_gamma(signed gamma_) { gamma = gamma_; }

void Colortable::enable_gamma_ramp(bool value) { gamma_ramp = value; }
void Colortable::enable_sepia(bool value) { sepia = value; }
void Colortable::enable_grayscale(bool value) { grayscale = value; }
void Colortable::enable_invert(bool value) { invert = value; }

void Colortable::update() {
  double kr = 0.2126, kb = 0.0722, kg = (1.0 - kr - kb);  //luminance weights

  for(unsigned i = 0; i < 32768; i++) {
    unsigned color  //bgr555->rgb888 conversion
    = ((i & 0x001f) << 19) | ((i & 0x001c) << 14)
    | ((i & 0x03e0) <<  6) | ((i & 0x0380) <<  1)
    | ((i & 0x7c00) >>  7) | ((i & 0x7000) >> 12);

    signed l;
    signed r = (color >> 16) & 0xff;
    signed g = (color >>  8) & 0xff;
    signed b = (color      ) & 0xff;

    if(gamma_ramp == true) {
      r = gamma_ramp_table[r >> 3];
      g = gamma_ramp_table[g >> 3];
      b = gamma_ramp_table[b >> 3];
    }

    if(contrast != 0) {
      r = contrast_adjust(r);
      g = contrast_adjust(g);
      b = contrast_adjust(b);
    }

    if(brightness != 0) {
      r = brightness_adjust(r);
      g = brightness_adjust(g);
      b = brightness_adjust(b);
    }

    if(gamma != 100) {
      r = gamma_adjust(r);
      g = gamma_adjust(g);
      b = gamma_adjust(b);
    }

    if(sepia == true) {
      l = (signed)((double)r * kr + (double)g * kg + (double)b * kb);
      l = max(0, min(255, l));

      r = (signed)((double)l * (1.0 + 0.300));
      g = (signed)((double)l * (1.0 - 0.055));
      b = (signed)((double)l * (1.0 - 0.225));

      r = max(0, min(255, r));
      g = max(0, min(255, g));
      b = max(0, min(255, b));
    }

    if(grayscale == true) {
      l = (signed)((double)r * kr + (double)g * kg + (double)b * kb);
      l = max(0, min(255, l));
      r = g = b = l;
    }

    if(invert == true) {
      r ^= 0xff;
      g ^= 0xff;
      b ^= 0xff;
    }

    switch(format) {
      case RGB555: {
        r >>= 3;
        g >>= 3;
        b >>= 3;
        table[i] = (r << 10) | (g << 5) | (b);
      } break;

      case RGB565: {
        r >>= 3;
        g >>= 2;
        b >>= 3;
        table[i] = (r << 11) | (g << 5) | (b);
      } break;

      case RGB888: {
        table[i] = (r << 16) | (g << 8) | (b);
      } break;

      default: {
        table[i] = ~0;
      } break;
    }
  }
}

Colortable::Colortable() {
  table = new uint32_t[32768];
  contrast = 0;
  brightness = 0;
  gamma = 100;

  gamma_ramp = false;
  sepia = false;
  grayscale = false;
  invert = false;
}

Colortable::~Colortable() {
  delete[] table;
}

const uint8_t Colortable::gamma_ramp_table[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

uint8_t Colortable::contrast_adjust(uint8_t input) {
  signed result = input - contrast + (2 * contrast * input + 127) / 255;
  return max(0, min(255, result));
}

uint8_t Colortable::brightness_adjust(uint8_t input) {
  signed result = input + brightness;
  return max(0, min(255, result));
}

uint8_t Colortable::gamma_adjust(uint8_t input) {
  signed result = (signed)(pow(((double)input / 255.0), (double)gamma / 100.0) * 255.0 + 0.5);
  return max(0, min(255, result));
}
