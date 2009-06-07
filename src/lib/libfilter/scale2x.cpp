Scale2xFilter filter_scale2x;

void Scale2xFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width  * 2;
  outheight = height * 2;
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

  uint32_t A, B, C, D, P;
  int prevline, nextline;

  for(int y = 0; y < height; y++) {
    prevline = (y > 0)          ? -pitch : 0;
    nextline = (y < height - 1) ?  pitch : 0;
    for(int x = 0; x < 256; x++) {
      A = *(input + prevline);
      B = (x >   0) ? *(input - 1) : *input;
      C = (x < 255) ? *(input + 1) : *input;
      D = *(input + nextline);
      P = colortable[*input];
      if(A != D && B != C) {
        *(output)                = A == B ? colortable[A] : P;
        *(output + 1)            = A == C ? colortable[A] : P;
        *(output + outpitch)     = D == B ? colortable[D] : P;
        *(output + outpitch + 1) = D == C ? colortable[D] : P;
      } else {
        *(output)                = P;
        *(output + 1)            = P;
        *(output + outpitch)     = P;
        *(output + outpitch + 1) = P;
      }
      input++;
      output += 2;
    }
    input += pitch - 256;
    output += outpitch + outpitch - 512;
  }
}
