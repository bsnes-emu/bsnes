class Video {
public:
SNES::video_info vi;
struct {
  int  width, height, refresh_rate;
  int  output_width, output_height;
  int  screen_height;
  bool fullscreen;
} settings;
  virtual uint16 *lock(uint32 &pitch) = 0;
  virtual void    unlock() = 0;

  virtual void    clear_video() = 0;
  virtual bool    set_video_mode(int width, int height, bool fullscreen,
                    int refresh_rate, int output_width, int output_height) = 0;

  virtual void    scanline() {}
  virtual void    redraw() = 0;
  virtual void    update() = 0;
  virtual void    init() = 0;
  virtual void    term() = 0;
};

#include "ddraw.h"
