Palette palette;

unsigned Palette::operator()(unsigned r, unsigned g, unsigned b) const {
  return red[r] + green[g] + blue[b];
}

/* 5-bit -> 8-bit
const uint8_t Palette::gammaRamp[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
};
*/

void Palette::update() {
  double exponent = 1.0 + (double)config->video.gamma * 0.01;
  for(unsigned n = 0; n < 1024; n++) {
    unsigned result = (n < 512 ? 511 * pow(((double)n / 511), exponent) : n);
    color[n] = result;
  }

  double contrast = config->video.contrast * 0.01;
  for(unsigned n = 0; n < 1024; n++) {
    signed result = color[n] * contrast;
    color[n] = max(0, min(1023, result));
  }

  signed brightness = (config->video.brightness - 100) * 4;
  for(unsigned n = 0; n < 1024; n++) {
    signed result = color[n] + brightness;
    color[n] = max(0, min(1023, result));
  }

  if(config->video.depth == 30) {
    for(unsigned n = 0; n < 1024; n++) {
      red[n] = color[n] << 20;
      green[n] = color[n] << 10;
      blue[n] = color[n] << 0;
    }
  }

  if(config->video.depth == 24) {
    for(unsigned n = 0; n < 1024; n++) {
      red[n] = (color[n] >> 2) << 16;
      green[n] = (color[n] >> 2) << 8;
      blue[n] = (color[n] >> 2) << 0;
    }
  }
}
