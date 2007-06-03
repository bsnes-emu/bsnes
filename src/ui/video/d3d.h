#include <d3d9.h>

class VideoD3D : public Video {
public:
HWND hwnd;
LPDIRECT3D9             lpd3d;
LPDIRECT3DDEVICE9       device;
LPDIRECT3DVERTEXBUFFER9 vertex_buffer, *vertex_ptr;
D3DPRESENT_PARAMETERS   presentation;
D3DSURFACE_DESC         d3dsd;
D3DLOCKED_RECT          d3dlr;
D3DRASTER_STATUS        d3drs;
D3DCAPS9                d3dcaps;
LPDIRECT3DTEXTURE9      texture;
LPDIRECT3DSURFACE9      surface;

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

  void set_vertex(uint32 px, uint32 py, uint32 pw, uint32 ph,
                  uint32 tw, uint32 th, uint32 x, uint32 y, uint32 w, uint32 h);

  bool lock(uint16 *&data, uint &pitch);
  void unlock();

  uint screen_width()  { return GetSystemMetrics(SM_CXSCREEN); }
  uint screen_height() { return GetSystemMetrics(SM_CYSCREEN); }

  bool update_video_mode();
  void update_hardware_filter();

  bool capture_screenshot();

  void init();
  void term();

  void clear_video();
  void refresh(uint width, uint height);

  VideoD3D(HWND handle);
  ~VideoD3D();
};
