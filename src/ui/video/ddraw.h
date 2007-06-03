#include <ddraw.h>

class VideoDD : public Video {
private:
HWND hwnd;
LPDIRECTDRAW         lpdd;
LPDIRECTDRAW7        lpdd7;
LPDIRECTDRAWSURFACE7 screen, raster;
LPDIRECTDRAWCLIPPER  clipper;
DDSURFACEDESC2       ddsd;
DDSCAPS2             ddscaps;
  void create_raster();
  void clear_video();

public:
  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  void refresh(uint width, uint height);
  void init();
  void term();

  VideoDD(HWND handle);
};
