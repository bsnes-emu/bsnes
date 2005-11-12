#include <ddraw.h>

class DDRenderer {
public:
LPDIRECTDRAW        lpdd;
LPDIRECTDRAW7       lpdd7;
LPDIRECTDRAWSURFACE lpdds, lpddsb;
LPDIRECTDRAWCLIPPER lpddc;
DDSURFACEDESC       ddsd;
DDSCAPS             ddscaps;
RECT lpddrc;
HWND hwnd;
bool  fullscreen;
int   width, height; //used for fullscreen mode clipping only
uint8 color_depth;
SNES::video_info vi; //initialized each frame at start of update()
  uint16 *lock(uint32 &pitch);
  void    unlock();

  void set_window(HWND hwnd_handle);
  void create_backbuffer();
  void to_windowed();
  void to_fullscreen(int _width, int _height);
  void set_source_window();
  void redraw();
  void update();
  void destroy();

  DDRenderer();
};

DDRenderer *dd_renderer;
