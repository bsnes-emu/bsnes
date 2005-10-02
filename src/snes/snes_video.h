enum {
  VM_256x224, //video data will be scaled to 256x224
  VM_512x224, //..
  VM_256x448, //..
  VM_512x448, //..
  VM_VARIABLE //video data can be 256x224 - 512x448
};

enum {
  VMF_NORMAL     = 0,
  VMF_HIRES      = 1,
  VMF_INTERLACE  = 2,
  VMF_HINTERLACE = 3
};

//video color adjustment
//via config::snes.video_color_adjust_mode
enum {
  VCA_NORMAL    = 0,
  VCA_GRAYSCALE = 1,
  VCA_VGA       = 2,
  VCA_GENESIS   = 3
};

static const uint8 color_curve_table[32];
uint32 color_lookup_table[32768];

struct {
  uint16 *data, *ppu_data;
  uint8   mode;
  uint8   depth;

  bool    frame_hires, frame_interlace;

  bool    format_changed;
} video, video_changed;

struct {
  bool hires, interlace;
} video_frame[224];

struct video_info {
  uint16 *data;
  uint8   mode;
  uint32  width, height;
};

//public functions
  void capture_screenshot();
  void update_color_lookup_table();

  virtual void  set_video_format(uint8 mode, uint8 depth);
  virtual void  get_video_info(video_info *info);
  virtual void  video_run() = 0;

//private functions
  uint16 *get_ppu_output_handle(); //used by PPU only
private:
//when a screenshot is requested, wait until the frame
//has finished rendering, so we can tell the image size
bool flag_output_screenshot;
  uint16 to_rgb555(uint32 color);
  void   output_screenshot();
  void   update_video_format();
  void   video_update_256x224(uint16 *src);
  void   video_update_512x224(uint16 *src);
  void   video_update_256x448(uint16 *src);
  void   video_update_512x448(uint16 *src);
  void   video_update();
  void   video_scanline();
  void   video_init();
public:
