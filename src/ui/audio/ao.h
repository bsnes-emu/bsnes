#include <ao/ao.h>

class AudioAO : public Audio {
private:
static uint object_count;

int driver_id;
ao_sample_format driver_format;
ao_device *audio_device;

public:
  void sample(uint16 l_sample, uint16 r_sample);
  void update_frequency();
  void init();
  void term();

  AudioAO(const char *driver = 0);
  ~AudioAO();
};
