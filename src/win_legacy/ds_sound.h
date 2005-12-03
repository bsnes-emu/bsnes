#include <dsound.h>

#define DSP_FREQ 32000

class DSSound {
public:
LPDIRECTSOUND ds;
LPDIRECTSOUNDBUFFER dsb_p, dsb_b;
DSBUFFERDESC dsbd;
WAVEFORMATEX wfx;
uint32 buffer_size;

public:
  void run(uint32 sample);
  void clear();
  void init();
  void term();

  DSSound() {
    ds          = 0;
    dsb_p       = 0;
    dsb_b       = 0;
    buffer_size = 0;
  }

  ~DSSound() {
    term();
  }
};

struct {
  uint32 buffer[2048];
  uint8  read_buffer, prev_buffer;
  uint32 buffer_pos, buffer_size;

  uint32 samples_per_frame;
} data;

DSSound *ds_sound;
