#include <ddraw.h>

class VideoDD : public Video {
private:
LPDIRECTDRAW         lpdd;
LPDIRECTDRAW7        lpdd7;
LPDIRECTDRAWSURFACE7 screen, backbuffer;
LPDIRECTDRAWSURFACE7 surface;
LPDIRECTDRAWCLIPPER  clipper;
DDSURFACEDESC2       ddsd;
DDSCAPS2             ddscaps;

public:
  uint16 *lock(uint32 &pitch);
  void    unlock();

  bool    update_video_profile();
  void    init();
  void    term();

  void    create_render_target();
  void    clear_video();

//ddraw_present.cpp
  void    create_presentation();
  void    redraw();
  void    update();

  VideoDD();
};
