ScanlineFilter filter_scanline;

void ScanlineFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height * 2;
}

void ScanlineFilter::render(
  uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
  unsigned *line, unsigned width, unsigned height
) {
  if(height > 240) {
    filter_direct.render(output, outpitch, input, pitch, line, width, height);
    return;
  }

  pitch >>= 1;
  outpitch >>= 2;

  for(unsigned y = 0; y < height; y++) {
    uint32_t *out0 = output;
    uint32_t *out1 = output + outpitch;
    if(width == 512 && line[y] == 256) {
      for(unsigned x = 0; x < 256; x++) {
        uint16_t p = *input++;
        *out0++ = colortable[p];
        *out0++ = colortable[p];
        *out1++ = (colortable[p] >> 1) & 0x7f7f7f;
        *out1++ = (colortable[p] >> 1) & 0x7f7f7f;
      }
      input += 256;
    } else {
      for(unsigned x = 0; x < width; x++) {
        uint16_t p = *input++;
        *out0++ = colortable[p];
        *out1++ = (colortable[p] >> 1) & 0x7f7f7f;
      }
    }
    input += pitch - width;
    output += outpitch * 2;
  }
}
