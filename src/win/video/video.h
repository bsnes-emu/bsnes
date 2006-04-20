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
  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;
  bool triple_buffering;
} video_settings[10];

void load_video_settings(uint profile);
void save_video_settings(uint profile);

class Video {
public:
SNES::video_info vi;

struct {
  uint render_width;
  uint render_height;

  uint hardware_filter;
  bool fullscreen;
  bool triple_buffering;
  bool enable_scanlines;

  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;

  uint internal_height_normal;
  uint internal_height_overscan;

  uint rx, ry, rw, rh;
} settings;
  void update_video_settings();
  void update_window();
  bool active_profile_is_fullscreen() { return settings.fullscreen; }

  virtual uint16 *lock(uint32 &pitch) = 0;
  virtual void    unlock() = 0;

  virtual void    pause_enable() {}
  virtual void    pause_disable() {}
  virtual void    clear_video() = 0;
  virtual bool    update_video_profile() = 0;
  virtual void    update_hardware_filter() {}
  virtual void    update_scanlines() {}

  virtual bool    capture_screenshot() { return false; }

  virtual void    redraw() = 0;
  virtual void    update() = 0;
  virtual void    init() = 0;
  virtual void    term() = 0;
};

#include "d3d.h"
#include "ddraw.h"
