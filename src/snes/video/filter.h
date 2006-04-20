enum {
  VIDEOFILTER_DIRECT,
  VIDEOFILTER_NTSC,
  VIDEOFILTER_HQ2X,
  VIDEOFILTER_SCALE2X,
};

//all video filters must derive from this class
class VideoFilter {
public:
/*[run]
 * uint32 *colortbl      - Color lookup table to convert BGR555 to current pixel format
 * uint16 *data          - Input data buffer (always in system memory)
 * uint32  width         - Input width (always 256 or 512)
 * uint32  height        - Input height (<=240 = progressive, >240 = interlace)
 * uint32  pitch         - Input data buffer bytes per scanline (always 1024)
 * uint16 *output        - Output data buffer (sometimes in video memory, never read from here!)
 * uint32  max_width     - Output data buffer width limit (do not write past buffer)
 * uint32  max_height    - Output data buffer height limit (do not write past buffer)
 * uint32  output_pitch  - Output data buffer bytes per scanline
 * uint32  req_width     - Requested output width (can be ignored if filter resizes to fixed size)
 * uint32  req_height    - Requested output height (can be ignored if filter resizes to fixed size)
 * uint32 &result_width  - Actual rendered width by filter (input is undefined, write only)
 * uint32 &result_height - Actual rendered height by filter (input is undefined, write only)
 */
  virtual void run(uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
                   uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
                   uint32 req_width, uint32 req_height, uint32 &result_width, uint32 &result_height, uint16 *scanline_widths = 0) = 0;
  virtual ~VideoFilter() {}
};

#include "filter_direct.h"
#include "filter_ntsc.h"
#include "filter_hq2x.h"
#include "filter_scale2x.h"
