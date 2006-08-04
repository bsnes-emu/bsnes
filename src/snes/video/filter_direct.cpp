void DirectVideoFilter::run(
uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
uint32 req_width, uint32 req_height, uint &result_width, uint &result_height,
uint16 *scanline_widths
) {
  pitch >>= 1;
  output_pitch >>= 1;

  for(int y = 0; y < height; y++) {
    if(width == 512 && scanline_widths[y] == 256) {
      for(int x = 0; x < 256; x++) {
        *output++ = colortbl[*data];
        *output++ = colortbl[*data++];
      }
      data += 256;
    } else {
      for(int x = 0; x < width; x++) {
        *output++ = colortbl[*data++];
      }
    }
    data   += pitch - width;
    output += output_pitch - width;
  }
  result_width  = width;
  result_height = height;
}
