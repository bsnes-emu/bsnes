#include <dsound.h>

class DSSound {
public:
LPDIRECTSOUND ds;
LPDIRECTSOUNDBUFFER dsb_p, *dsb_b;
DSBUFFERDESC dsbd;
WAVEFORMATEX wfx;
uint32 sample_size;
uint32 buffer_size;
uint32 buffer_pos;

void  *dslb;
uint32 dslbs;

public:
  void run();
  void init();

  DSSound();
};

struct {
  uint32 *buffer;
  uint32 lpos;
  uint32 lsample;
  bool   lbuffer;
} data;

DSSound *ds_sound;
