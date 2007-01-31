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

  void tick();
  void run(uint32 sample);
  void set_frequency(uint freq);
  void clear_audio();
  void init();
  void term();

  void run_videosync();
  void run_audiosync();

  AudioDS(HWND handle = 0);
  ~AudioDS();
};
