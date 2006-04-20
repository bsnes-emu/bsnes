class HQ2xVideoFilter : public VideoFilter {
public:
uint32 yuvtable[32768];
uint32 diff_offset, diff_mask;
  void run(uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
           uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
           uint32 req_width, uint32 req_height, uint32 &result_width, uint32 &result_height, uint16 *scanline_widths = 0);

  void lores_progressive(uint32 *colortbl, uint16 *data, uint32 height, uint32 pitch,
                         uint16 *output, uint32 output_pitch);

  HQ2xVideoFilter();
};
