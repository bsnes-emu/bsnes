#include "snes_ntsc/snes_ntsc.h"

class NTSCFilter : public Filter {
public:
  void render(uint32_t*, unsigned, unsigned&, unsigned&, uint16_t*, unsigned, unsigned*, unsigned, unsigned);

  NTSCFilter();
  ~NTSCFilter();

private:
  struct snes_ntsc_t *ntsc;
  int burst, burst_toggle;

  void adjust(float hue, float saturation, float contrast, float brightness, float sharpness, bool merge_fields);
};

extern NTSCFilter filter_ntsc;
