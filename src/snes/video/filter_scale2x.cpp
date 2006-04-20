void Scale2xVideoFilter::run(
uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
uint32 req_width, uint32 req_height, uint32 &result_width, uint32 &result_height,
uint16 *scanline_widths
) {
  pitch >>= 1;
  output_pitch >>= 1;
  if(width == 256 && height <= 240) {
    lores_progressive(colortbl, data, height, pitch, output, output_pitch);
    result_width  = 512;
    result_height = height * 2;
  } else {
  int w = (width  <= max_width)  ? width  : max_width;
  int h = (height <= max_height) ? height : max_height;
    for(int y = 0; y < h; y++) {
      for(int x = 0; x < w; x++) {
        *output++ = colortbl[*data++];
      }
      data   += pitch - w;
      output += output_pitch - w;
    }
    result_width  = width;
    result_height = height;
  }
}

void Scale2xVideoFilter::lores_progressive(
uint32 *colortbl, uint16 *data, uint32 height, uint32 pitch,
uint16 *output, uint32 output_pitch
) {
uint16 A, B, C, D, P;
int32  prevline, nextline;
//.A. ->.AA.
//BpC ->BppC
//.D. ->BppC
//    ->.DD.
  for(int y = 0; y < height; y++) {
    prevline = (y > 0)          ? -pitch : 0;
    nextline = (y < height - 1) ?  pitch : 0;
    for(int x = 0; x < 256; x++) {
      A = *(data + prevline);
      B = (x >   0) ? *(data - 1) : *data;
      C = (x < 255) ? *(data + 1) : *data;
      D = *(data + nextline);
      P = colortbl[*(data)];
      if(A != D && B != C) {
        *(output)                    = A == B ? colortbl[A] : P;
        *(output + 1)                = A == C ? colortbl[A] : P;
        *(output + output_pitch)     = D == B ? colortbl[D] : P;
        *(output + output_pitch + 1) = D == C ? colortbl[D] : P;
      } else {
        *(output)                    = P;
        *(output + 1)                = P;
        *(output + output_pitch)     = P;
        *(output + output_pitch + 1) = P;
      }
      data++;
      output += 2;
    }
    data   += pitch - 256;
    output += output_pitch + output_pitch - 512;
  }
}
