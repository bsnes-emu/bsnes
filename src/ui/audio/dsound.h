#include <dsound.h>

class AudioDS : public Audio {
public:
LPDIRECTSOUND ds;
LPDIRECTSOUNDBUFFER dsb_p, dsb_b;
DSBUFFERDESC dsbd;
WAVEFORMATEX wfx;
uint buffer_size;

struct {
  uint32 buffer[16384];
  uint8  read_buffer, prev_buffer;
  uint32 buffer_pos, buffer_size;

  uint32 samples_per_frame;
} data;
  void run(uint32 sample);
  void set_frequency(uint new_freq);
  void clear_audio();
  void init();
  void term();

  AudioDS() {
    ds          = 0;
    dsb_p       = 0;
    dsb_b       = 0;
    buffer_size = 0;
  }

  ~AudioDS() { term(); }
};
