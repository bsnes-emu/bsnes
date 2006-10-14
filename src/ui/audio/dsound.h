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
  uint read_buffer, prev_buffer;
  uint samples_per_frame;
  uint buffer_pos, buffer_size, buffer_count;
} data;

  void run(uint32 sample);
  void set_frequency(uint new_freq);
  void clear_audio();
  void init();
  void term();

  AudioDS(HWND handle = 0) {
    hwnd  = (handle) ? handle : GetDesktopWindow();
    ds    = 0;
    dsb_p = 0;
    dsb_b = 0;

    data.buffer            = 0;
    data.read_buffer       = 0;
    data.prev_buffer       = 0;
    data.samples_per_frame = 0;
    data.buffer_pos        = 0;
    data.buffer_size       = 0;
    data.buffer_count      = 0;
  }

  ~AudioDS() { term(); }
};
