class DirectVideoFilter : public VideoFilter {
public:
  void run(uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
           uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
           uint32 req_width, uint32 req_height, uint &result_width, uint &result_height, uint16 *scanline_widths = 0);
};
