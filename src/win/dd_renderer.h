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
bool   fullscreen;
int    width, height; //used for fullscreen mode clipping only
uint8  color_depth;
uint8  color_curve_table[32];
uint32 color_lookup_table[32768];
  void set_window(HWND hwnd_handle);
  void create_backbuffer();
  void to_windowed();
  void to_fullscreen(int _width, int _height);
  void set_source_window();
  void redraw();
  inline void update16_256x224();
  inline void update16_256x448();
  inline void update16_512x224();
  inline void update16_512x448();
  void update16();
  inline void update32_256x224();
  inline void update32_256x448();
  inline void update32_512x224();
  inline void update32_512x448();
  void update32();
  void update();
  void destroy();
  void update_color_lookup_table();

  DDRenderer();
};

DDRenderer *dd_renderer;
