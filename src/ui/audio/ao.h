#include <ao/ao.h>

class AudioAO : public Audio {
private:
static uint object_count;

int driver_id;
ao_sample_format driver_format;
ao_device *audio_device;

public:
  void run(uint32 sample);
  void set_frequency(uint freq);
  void init();
  void term();

  void set_driver(const char *name);

  AudioAO();
  ~AudioAO();
};
