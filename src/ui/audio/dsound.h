#include <dsound.h>

class AudioDS : public Audio {
public:
HWND hwnd;
LPDIRECTSOUND ds;
LPDIRECTSOUNDBUFFER dsb_p, dsb_b;
DSBUFFERDESC dsbd;
WAVEFORMATEX wfx;

struct {
  uint32 *buffer;
  uint buffer_pos,  ring_pos;
  uint buffer_size, ring_size;
} data;

  void sample(uint16 l_sample, uint16 r_sample);
  void update_frequency();
  void clear_audio();
  void init();
  void term();

  AudioDS(HWND handle = 0);
  ~AudioDS();
};
