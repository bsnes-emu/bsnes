Colortable colortable;

void Colortable::set_format(Format format_) { format = format_; }
void Colortable::set_contrast(int32_t contrast_) { contrast = contrast_; }
void Colortable::set_brightness(int32_t brightness_) { brightness = brightness_; }
void Colortable::set_gamma(int32_t gamma_) { gamma = gamma_; }

void Colortable::enable_gamma_ramp(bool value) { gamma_ramp = value; }
void Colortable::enable_sepia(bool value) { sepia = value; }
void Colortable::enable_grayscale(bool value) { grayscale = value; }
void Colortable::enable_invert(bool value) { invert = value; }

void Colortable::update() {
  int32_t l, r, g, b;
  double kr = 0.2126, kb = 0.0722, kg = (1.0 - kr - kb); //luminance
  uint32_t col;
  for(unsigned i = 0; i < 32768; i++) {
    //bgr555->rgb888
    col = ((i & 0x001f) << 19) | ((i & 0x001c) << 14)
        | ((i & 0x03e0) <<  6) | ((i & 0x0380) <<  1)
        | ((i & 0x7c00) >>  7) | ((i & 0x7000) >> 12);

    r = (col >> 16) & 0xff;
    g = (col >>  8) & 0xff;
    b = (col      ) & 0xff;

    if(gamma_ramp == true) {
      r = gamma_ramp_table[r >> 3];
      g = gamma_ramp_table[g >> 3];
      b = gamma_ramp_table[b >> 3];
    }

    contrast_adjust(r); brightness_adjust(r); gamma_adjust(r);
    contrast_adjust(g); brightness_adjust(g); gamma_adjust(g);
    contrast_adjust(b); brightness_adjust(b); gamma_adjust(b);

    if(sepia == true) {
      l = (int32_t)((double)r * kr + (double)g * kg + (double)b * kb);
      l = max(0, min(255, l));

      r = (int32_t)((double)l * (1.0 + 0.300));
      g = (int32_t)((double)l * (1.0 - 0.055));
      b = (int32_t)((double)l * (1.0 - 0.225));

      r = max(0, min(255, r));
      g = max(0, min(255, g));
      b = max(0, min(255, b));
    }

    if(grayscale == true) {
      l = (int32_t)((double)r * kr + (double)g * kg + (double)b * kb);
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
        table[i] = -1U;
      } break;
    }
  }
}

Colortable::Colortable() {
  table = new uint32_t[32768];
  contrast = 0;
  brightness = 0;
  gamma = 100;
}

Colortable::~Colortable() {
  delete[] table;
}

/* internal */

const uint8_t Colortable::gamma_ramp_table[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};

void Colortable::contrast_adjust(int32_t &input) {
  double lmin =   0.0 - (double)contrast;
  double lmax = 255.0 + (double)contrast;
  int32_t result = (int32_t)(lmin + (double)input * ((lmax - lmin) / 256.0));
  input = max(0, min(255, result));
}

void Colortable::brightness_adjust(int32_t &input) {
  int32_t result = input + brightness;
  input = max(0, min(255, result));
}

void Colortable::gamma_adjust(int32_t &input) {
  int32_t result = (int32_t)(pow(((double)(input + 1) / 256.0), (double)gamma / 100.0) * 256.0);
  input = max(0, min(255, result));
}
