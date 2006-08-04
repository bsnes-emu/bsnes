#include "filter_ntsc_core.h"

class NtscVideoFilter : public VideoFilter {
private:
struct snes_ntsc_t *ntsc;
int burst, burst_toggle;

public:
// - Use 0 for any parameter to use default value
// - Colortbl must map to 16-bit RGB
  void adjust(float hue, float saturation, float contrast, float brightness,
              float sharpness, bool merge_fields, uint32 *colortbl);

// - Output is currently always 16-bit RGB
// - Scanline_widths [i] contains the number of source pixels for scanline i,
//   where 0 < i < height
  void run(uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
           uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
           uint32 req_width, uint32 req_height, uint &result_width, uint &result_height,
           uint16 *scanline_widths = 0);

  NtscVideoFilter();
  ~NtscVideoFilter();
};
