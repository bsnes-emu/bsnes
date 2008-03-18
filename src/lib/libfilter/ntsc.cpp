#include "snes_ntsc/snes_ntsc.c"

NTSCFilter filter_ntsc;

void NTSCFilter::render(
  uint32_t *output, unsigned outpitch, unsigned &outwidth, unsigned &outheight,
  uint16_t *input, unsigned pitch, unsigned *line, unsigned width, unsigned height
) {
  if(!ntsc)return;

  int const out_width = outwidth = SNES_NTSC_OUT_WIDTH(256);
  int const out_height = outheight = height;
  burst ^= burst_toggle;

  //blit multiple scanlines of same width, rather than one at a time
  int run_start = 0;
  int run_width = line[0];
  int l = 0;

  while(1) {
    if(run_width != line[l] || l >= out_height) {
    uint16_t const *in = (uint16_t*)((uint8_t*)input + pitch * run_start);
    uint16_t *out = (uint16_t*)((uint8_t*)output + outpitch * run_start);
    int height = l - run_start;
    int line_burst = (burst + run_start) % 3;
      if(run_width == 256) {
        snes_ntsc_blit(ntsc, in, (pitch >> 1), line_burst, out_width, height, out, outpitch);
      } else {
        snes_ntsc_blit_hires(ntsc, in, (pitch >> 1), line_burst, out_width, height, out, outpitch);
      }
      if(l >= out_height) break;
      run_width = line[l];
      run_start = l;
    }
    l++;
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
      return; //to do: report out of memory error
    }
  }

  burst = 0;
  burst_toggle = (merge_fields ? 0 : 1); // don't toggle burst when fields are merged
  snes_ntsc_init(ntsc, &setup);
}

NTSCFilter::NTSCFilter() {
  ntsc = 0;
  adjust(0, 0, 0, 0, 0, true);
}

NTSCFilter::~NTSCFilter() {
  if(ntsc) free(ntsc);
}
