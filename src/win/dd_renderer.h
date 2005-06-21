#include <ddraw.h>
class DDRenderer {
public:
LPDIRECTDRAW        lpdd;
LPDIRECTDRAWSURFACE lpdds, lpddsb;
LPDIRECTDRAWCLIPPER lpddc;
DDSURFACEDESC       ddsd;
DDSCAPS             ddscaps;
HWND hwnd;
uint8 color_depth;
uint32 color_lookup_table[65536];
  void set_window(HWND hwnd_handle);
  void create_backbuffer();
  void to_windowed();
  void to_fullscreen();
  void set_source_window(RECT *rs);
  void redraw();
  void update16();
  void update32();
  void update();
  void destroy();
  void update_color_lookup_table();

  DDRenderer();
};

DDRenderer *dd_renderer;
