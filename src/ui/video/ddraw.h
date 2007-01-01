#include <ddraw.h>

class VideoDD : public Video {
private:
HWND hwnd;
LPDIRECTDRAW         lpdd;
LPDIRECTDRAW7        lpdd7;
LPDIRECTDRAWSURFACE7 screen, backbuffer;
LPDIRECTDRAWSURFACE7 surface;
LPDIRECTDRAWCLIPPER  clipper;
DDSURFACEDESC2       ddsd;
DDSCAPS2             ddscaps;

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return GetScreenWidth();  }
  uint screen_height() { return GetScreenHeight(); }

  bool update_video_profile();
  void init();
  void term();

  void create_render_target();
  void clear_video();

  void create_presentation();
  void redraw();
  void update();

  VideoDD(HWND handle);
};
