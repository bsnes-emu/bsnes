#include "snes_ntsc/snes_ntsc.c"

NTSCFilter filter_ntsc;

void NTSCFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = SNES_NTSC_OUT_WIDTH(256);
  outheight = height;
}

void NTSCFilter::render(
  uint32_t *output, unsigned outpitch, uint16_t *input, unsigned pitch,
  unsigned *line, unsigned width, unsigned height
) {
  if(!ntsc) return;

  width = SNES_NTSC_OUT_WIDTH(256);
  burst ^= burst_toggle;

  pitch >>= 1;
  outpitch >>= 2;

  unsigned line_burst = burst;
  for(unsigned y = 0; y < height; y++) {
    uint16_t *in  = input  + y * pitch;
    uint32_t *out = output + y * outpitch;

    //render as many lines in one snes_ntsc_blit as possible:
    //do this by determining for how many lines the width stays the same
    unsigned rheight = 1;
    unsigned rwidth  = line[y];
    while(y + rheight < height && rwidth == line[y + rheight]) rheight++;

    if(rwidth == 256) {
      snes_ntsc_blit      (ntsc, in, pitch, line_burst, rwidth, rheight, out, outpitch << 2);
    } else {
      snes_ntsc_blit_hires(ntsc, in, pitch, line_burst, rwidth, rheight, out, outpitch << 2);
    }

    line_burst = (line_burst + rheight) % 3;
    y += rheight;
  }
}

void NTSCFilter::adjust(
  float hue, float saturation, float contrast,
  float brightness, float sharpness, bool merge_fields
) {
  static snes_ntsc_setup_t defaults;
  snes_ntsc_setup_t setup = defaults;
  setup.hue            = hue;
  setup.saturation     = saturation;
  setup.contrast       = contrast;
  setup.brightness     = brightness;
  setup.sharpness      = sharpness;
  setup.resolution     = sharpness;
  setup.merge_fields   = merge_fields;
  setup.bsnes_colortbl = 0;

  if(!ntsc) {
    ntsc = (snes_ntsc_t*)malloc(sizeof *ntsc);
    if(!ntsc) {
      fprintf(stderr, "error: snes_ntsc: out of memory\n");
      return;
    }
  }

  burst = 0;
  burst_toggle = (merge_fields ? 0 : 1);  //don't toggle burst when fields are merged
  snes_ntsc_init(ntsc, &setup);
}

NTSCFilter::NTSCFilter() {
  ntsc = 0;
  adjust(0, 0, 0, 0, 0, false);
}

NTSCFilter::~NTSCFilter() {
  if(ntsc) free(ntsc);
}
