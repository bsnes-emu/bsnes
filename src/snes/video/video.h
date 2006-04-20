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
  uint32  raster_width, raster_height;
  uint32  width, height;
  uint32  filter, pixel_format;
  uint32  pitch;

  bool    frame_hires, frame_interlace;
} video;

struct {
  bool    modified;
  uint32  filter, pixel_format;
} video_format;

uint16 pline_width[240], iline_width[480];

struct video_info {
  uint32  filter, pixel_format, width, height;
};

  void contrast_adjust(int32 &input);
  void brightness_adjust(int32 &input);
  void gamma_adjust(int32 &input);
//public functions
  void update_color_lookup_table();

  virtual void    set_video_format(uint32 filter, uint32 pixel_format);
  virtual void    get_video_info(video_info *info);
  virtual void    video_run() = 0;
  virtual uint16 *video_lock(uint32 &pitch) = 0;
  virtual void    video_unlock() = 0;

//private functions
private:
  void   update_video_format();
  void   video_normalize();
  void   video_update();
  void   video_scanline();
  void   video_init();
public:
