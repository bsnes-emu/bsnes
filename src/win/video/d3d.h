#include <d3d9.h>
#include <d3dx9.h>

class VideoD3D : public Video {
public:
LPDIRECT3D9             lpd3d;
LPDIRECT3DDEVICE9       device;
LPDIRECT3DVERTEXBUFFER9 vertex_buffer, *vertex_ptr;
D3DPRESENT_PARAMETERS   presentation;
D3DSURFACE_DESC         d3dsd;
D3DLOCKED_RECT          d3dlr;
D3DRASTER_STATUS        d3drs;
D3DCAPS9                d3dcaps;
LPDIRECT3DTEXTURE9      texture, scanline[3];
LPDIRECT3DSURFACE9      surface, temp_surface;

struct d3dvertex {
  float  x, y, z, rhw; //screen coords
  uint32 color;        //diffuse color
  float  u, v;         //texture coords
};

struct {
  uint32 t_usage, v_usage;
  uint32 t_pool,  v_pool;
  uint32 lock;
  uint32 filter;
} flags;

struct {
  bool dynamic;     //device supports dynamic textures
  bool addresswrap; //device supports texture address (u,v) wrapping
  bool stretchrect; //device supports StretchRect
} caps;

  void    set_vertex(uint32 px, uint32 py, uint32 pw, uint32 ph,
            uint32 tw, uint32 th, uint32 x, uint32 y, uint32 w, uint32 h);

  uint16 *lock(uint32 &pitch);
  void    unlock();

  void    pause_enable();
  void    pause_disable();

  bool    update_video_profile();
  void    update_hardware_filter();
  void    update_scanlines();

  bool    capture_screenshot();

  void    init();
  void    term();

  void    clear_video();
  void    redraw();
  void    update();

  VideoD3D();
};
