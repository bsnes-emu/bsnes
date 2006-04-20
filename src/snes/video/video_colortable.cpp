const uint8 SNES::gamma_ramp_table[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff
};

void SNES::contrast_adjust(int32 &input) {
double lmin, lmax;
  lmin =   0.0 - double(int32(config::snes.contrast));
  lmax = 255.0 + double(int32(config::snes.contrast));
int32 result = int32(lmin + double(input) * ((lmax - lmin) / 256.0));
  input = (result > 255) ? 255 : (result < 0) ? 0 : result;
}

void SNES::brightness_adjust(int32 &input) {
int32 result;
  result = input + int32(config::snes.brightness);
  input  = (result > 255) ? 255 : (result < 0) ? 0 : result;
}

void SNES::gamma_adjust(int32 &input) {
int32 result;
  result = int32(pow((double(input + 1) / 256.0), double(config::snes.gamma) / 100.0) * 256.0);
  input  = (result > 255) ? 255 : (result < 0) ? 0 : result;
}

void SNES::update_color_lookup_table() {
int32  l, r, g, b;
double kr = 0.2126, kb = 0.0722, kg = (1.0 - kr - kb); //luminance
uint32 col;
  for(int i = 0; i < 32768; i++) {
  //bgr555->rgb888
    col = ((i & 0x001f) << 19) | ((i & 0x001c) << 14) |
          ((i & 0x03e0) <<  6) | ((i & 0x0380) <<  1) |
          ((i & 0x7c00) >>  7) | ((i & 0x7000) >> 12);

    r = (col >> 16) & 0xff;
    g = (col >>  8) & 0xff;
    b = (col      ) & 0xff;

    if(bool(config::snes.gamma_ramp) == true) {
      r = gamma_ramp_table[r >> 3];
      g = gamma_ramp_table[g >> 3];
      b = gamma_ramp_table[b >> 3];
    }

    contrast_adjust(r); brightness_adjust(r); gamma_adjust(r);
    contrast_adjust(g); brightness_adjust(g); gamma_adjust(g);
    contrast_adjust(b); brightness_adjust(b); gamma_adjust(b);

    if(bool(config::snes.sepia) == true) {
      l = int32(double(r) * kr + double(g) * kg + double(b) * kb);
      l = (l > 255) ? 255 : (l < 0) ? 0 : l;
      r = int32(double(l) * (1.0 + 0.300));
      g = int32(double(l) * (1.0 - 0.055));
      b = int32(double(l) * (1.0 - 0.225));
      r = (r > 255) ? 255 : (r < 0) ? 0 : r;
      g = (g > 255) ? 255 : (g < 0) ? 0 : g;
      b = (b > 255) ? 255 : (b < 0) ? 0 : b;
    }

    if(bool(config::snes.grayscale) == true) {
      l = int32(double(r) * kr + double(g) * kg + double(b) * kb);
      l = (l > 255) ? 255 : (l < 0) ? 0 : l;
      r = g = b = l;
    }

    if(bool(config::snes.invert) == true) {
      r ^= 0xff;
      g ^= 0xff;
      b ^= 0xff;
    }

    switch(video.pixel_format) {
    case PIXELFORMAT_RGB444:
      r >>= 4;
      g >>= 4;
      b >>= 4;
      color_lookup_table[i] = (r << 8) | (g << 4) | (b);
      break;
    case PIXELFORMAT_RGB555:
      r >>= 3;
      g >>= 3;
      b >>= 3;
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
      break;
    case PIXELFORMAT_RGB565:
      r >>= 3;
      g >>= 2;
      b >>= 3;
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
      break;
    case PIXELFORMAT_RGB888:
      color_lookup_table[i] = (r << 16) | (g << 8) | (b);
      break;
    default:
      color_lookup_table[i] = uint(-1);
      break;
    }
  }
}
