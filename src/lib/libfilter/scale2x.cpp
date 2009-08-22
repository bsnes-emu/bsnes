Scale2xFilter filter_scale2x;

void Scale2xFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height;

  if(width <= 256 && height <= 240) {
    outwidth  *= 2;
    outheight *= 2;
  }
}

void Scale2xFilter::render(
  uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
  unsigned *line, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, input, pitch, line, width, height);
    return;
  }

  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    int prevline = (y == 0) ? 0 : pitch;
    int nextline = (y == height - 1) ? 0 : pitch;

    for(unsigned x = 0; x < 256; x++) {
      uint16_t A = *(input - prevline);
      uint16_t B = (x >   0) ? *(input - 1) : *input;
      uint16_t C = *input;
      uint16_t D = (x < 255) ? *(input + 1) : *input;
      uint16_t E = *(input++ + nextline);
      uint32_t c = colortable[C];

      if(A != E && B != D) {
        *out0++ = (A == B ? colortable[A] : c);
        *out0++ = (A == D ? colortable[A] : c);
        *out1++ = (E == B ? colortable[E] : c);
        *out1++ = (E == D ? colortable[E] : c);
      } else {
        *out0++ = c;
        *out0++ = c;
        *out1++ = c;
        *out1++ = c;
      }
    }

    input += pitch - 256;
    out0 += outpitch + outpitch - 512;
    out1 += outpitch + outpitch - 512;
  }
}
