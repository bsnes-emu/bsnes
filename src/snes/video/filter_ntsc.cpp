#define SNES_NTSC_IN_FORMAT SNES_NTSC_BGR15
#include "filter_ntsc_core.cpp"

NtscVideoFilter::NtscVideoFilter() {
  ntsc = 0;

//to do: defer initialization?
  adjust(0, 0, 0, 0, 0, bool(config::snes.ntsc_merge_fields), 0);
}

NtscVideoFilter::~NtscVideoFilter() {
  SafeFree(ntsc);
}

void NtscVideoFilter::adjust(float hue, float saturation, float contrast,
float brightness, float sharpness, bool merge_fields, uint32 *colortbl) {
static snes_ntsc_setup_t defaults;
snes_ntsc_setup_t setup = defaults;
  setup.hue            = hue;
  setup.saturation     = saturation;
  setup.contrast       = contrast;
  setup.brightness     = brightness;
  setup.sharpness      = sharpness;
  setup.resolution     = sharpness;
  setup.merge_fields   = merge_fields;
  setup.bsnes_colortbl = colortbl;

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

void NtscVideoFilter::run(
uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
uint32 req_width, uint32 req_height, uint32 &result_width, uint32 &result_height,
uint16 *scanline_widths)
{
  if(!ntsc)return;

int const out_width  = SNES_NTSC_OUT_WIDTH(256);
int const out_height = height;
  result_width  = out_width;
  result_height = out_height;
  burst ^= burst_toggle;

  if(!scanline_widths) {
    if(width == 256) {
      snes_ntsc_blit(ntsc, data, (pitch >> 1), burst, out_width, out_height, output, output_pitch);
    } else {
      snes_ntsc_blit_hires(ntsc, data, (pitch >> 1), burst, out_width, out_height, output, output_pitch);
    }
  } else {
  //blit multiple scanlines of same width, rather than one at a time
  int run_start  = 0;
  int run_width  = scanline_widths[0];
  int line       = 0;

    while(1) {
      if(run_width != scanline_widths[line] || line >= out_height) {
      uint16 const *in = (uint16*)((uint8*)data + pitch * run_start);
      uint16 *out      = (uint16*)((uint8*)output + output_pitch * run_start);
      int height       = line - run_start;
      int line_burst   = (burst + run_start) % 3;
        if(run_width == 256) {
          snes_ntsc_blit(ntsc, in, (pitch >> 1), line_burst, out_width, height, out, output_pitch);
        } else {
          snes_ntsc_blit_hires(ntsc, in, (pitch >> 1), line_burst, out_width, height, out, output_pitch);
        }
        if(line >= out_height)break;
        run_width = scanline_widths[line];
        run_start = line;
      }
      line++;
    }
  }
}
