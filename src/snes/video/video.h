enum {
  VIDEOSTANDARD_NTSC,
  VIDEOSTANDARD_PAL,
};

enum {
  PIXELFORMAT_RGB444,
  PIXELFORMAT_RGB555,
  PIXELFORMAT_RGB565,
  PIXELFORMAT_RGB888,
};

enum {
  VIDEOPITCH_LORES = 2048,
  VIDEOPITCH_HIRES = 1024,
};

VideoFilter *video_filter;

static const uint8 gamma_ramp_table[32];
uint32 color_lookup_table[32768];

struct {
  uint16 *data, *raster_data, *ppu_data;
  uint    raster_width, raster_height;
  uint    width, height;
  uint    filter, video_standard, pixel_format;
  uint    pitch;

  bool    frame_hires, frame_interlace;
} video;

struct {
  bool modified;
  uint filter, video_standard, pixel_format;
} video_format;

uint16 pline_width[240], iline_width[480];

struct video_info {
  uint filter, video_standard, pixel_format, width, height;
};

  void contrast_adjust(int32 &input);
  void brightness_adjust(int32 &input);
  void gamma_adjust(int32 &input);
//public functions
  void update_color_lookup_table();

  void set_video_filter(uint video_filter);
  void set_video_standard(uint video_standard);
  void set_video_pixel_format(uint pixel_format);
  void get_video_info(video_info *info);

//private functions
private:
  void update_video_format();
  void video_normalize();
  void video_update();
  void video_scanline();
  void video_init();
public:
