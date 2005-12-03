#include <ddraw.h>

class VideoDD : public Video {
private:
LPDIRECTDRAW         lpdd;
LPDIRECTDRAW7        lpdd7;
LPDIRECTDRAWSURFACE7 lpdds;
LPDIRECTDRAWCLIPPER  lpddc;

struct {
  LPDIRECTDRAWSURFACE7 lpdds;
  bool valid;
} surface[2];
uint8  active_surface;
bool   in_vblank;
uint32 last_scanline;

DDSURFACEDESC2 ddsd;
DDSCAPS        ddscaps;

RECT lpddrc;
bool fullscreen;

public:
  uint16 *lock(uint32 &pitch);
  void    unlock();

  void    clear_video();
  bool    set_video_mode(int width, int height, bool fullscreen,
            int refresh_rate, int output_width, int output_height);
  void    init();
  void    term();

  void    create_surface(uint8 i);
  void    set_mode_windowed();
  void    set_mode_fullscreen();
  void    set_source_window();
  void    redraw();
  void    scanline();
  void    update();

  VideoDD();
};
