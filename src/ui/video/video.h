enum { VIDEO_PROFILE_COUNT = 2 };

enum {
  HARDWARE_FILTER_PIXEL,
  HARDWARE_FILTER_BILINEAR,
};

struct VideoSettings {
  uint software_filter;
  uint hardware_filter;
  uint video_standard;
  uint multiplier;
  bool correct_aspect_ratio;
  bool enable_scanlines;
  bool manual_render_size;
  uint render_width;
  uint render_height;
  bool fullscreen;
  bool triple_buffering;
  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;
} video_settings[8];

void load_video_settings(uint profile);
void save_video_settings(uint profile);

class Video {
public:
SNES::video_info vi;

struct {
  uint render_width;
  uint render_height;

  bool fullscreen;
  bool triple_buffering;

  uint hardware_filter;
  bool enable_scanlines;

  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;

  uint rx, ry, rw, rh;
} settings;
  void update_video_settings();

  virtual uint16 *lock(uint &pitch) = 0;
  virtual void    unlock() = 0;

  virtual uint    screen_width()  = 0;
  virtual uint    screen_height() = 0;

  virtual bool    update_video_profile() { return true; }
  virtual void    clear_video() {}

  virtual void    pause_enable() {}
  virtual void    pause_disable() {}
  virtual void    update_hardware_filter() {}
  virtual void    update_scanlines() {}

  virtual bool    capture_screenshot() { return false; }

  virtual void    redraw() = 0;
  virtual void    update() = 0;
  virtual void    init() = 0;
  virtual void    term() = 0;

  Video();
  virtual ~Video();
} *uiVideo;
