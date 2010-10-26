#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

uint32_t *colortable;

#include "snes_ntsc/snes_ntsc.h"
#include "snes_ntsc/snes_ntsc.c"

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};


struct snes_ntsc_t *ntsc;
snes_ntsc_setup_t setup;
int burst;
int burst_toggle;

void initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

  ntsc = (snes_ntsc_t*)malloc(sizeof *ntsc);
  setup = snes_ntsc_composite;
  setup.merge_fields = 1;
  snes_ntsc_init(ntsc, &setup);

  burst = 0;
  burst_toggle = (setup.merge_fields ? 0 : 1);
}

void terminate() {
  if(ntsc) free(ntsc);
}

dllexport void filter_size(unsigned &width, unsigned &height) {
  initialize();
  width  = SNES_NTSC_OUT_WIDTH(256);
  height = height;
}

dllexport void filter_render(
  uint32_t *colortable_, uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  initialize();
  if(!ntsc) return;
  colortable = colortable_;

  pitch >>= 1;
  outpitch >>= 2;

  if(width <= 256) {
    snes_ntsc_blit      (ntsc, input, pitch, burst, width, height, output, outpitch << 2);
  } else {
    snes_ntsc_blit_hires(ntsc, input, pitch, burst, width, height, output, outpitch << 2);
  }

  burst ^= burst_toggle;
}
