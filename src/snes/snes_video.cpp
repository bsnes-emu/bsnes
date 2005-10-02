#include "snes_video_ex.cpp"

const uint8 SNES::color_curve_table[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff
};

void SNES::update_color_lookup_table() {
int i, l, r, g, b;
double lr = 0.2126, lg = 0.7152, lb = 0.0722; //luminance
  switch(video.depth) {
  case 15: //rgb565
    for(i=0;i<32768;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;

      if((bool)config::snes.video_color_curve == true) {
        r = color_curve_table[r] >> 3;
        g = color_curve_table[g] >> 3;
        b = color_curve_table[b] >> 3;
      }

      if((int)config::snes.video_color_adjust_mode == VCA_GRAYSCALE) {
        r = (r << 3) | (r >> 2);
        g = (g << 3) | (g >> 2);
        b = (b << 3) | (b >> 2);

        l = int((double)r * lr) + ((double)g * lg) + ((double)b * lb);
        if(l <   0)l =   0;
        if(l > 255)l = 255;
        r = g = b = l;

        r >>= 3;
        g >>= 3;
        b >>= 3;
      } else if((int)config::snes.video_color_adjust_mode == VCA_VGA) {
      //rgb555->rgb332
        r >>= 2;
        g >>= 2;
        b >>= 3;

        r = (r << 2) | (r >> 1);
        g = (g << 2) | (g >> 1);
        b = (b << 3) | (b << 1) | (b >> 1);
      } else if((int)config::snes.video_color_adjust_mode == VCA_GENESIS) {
      //rgb555->rgb333
        r >>= 2;
        g >>= 2;
        b >>= 2;

        r = (r << 2) | (r >> 1);
        g = (g << 2) | (g >> 1);
        b = (b << 2) | (b >> 1);
      }

      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
    }
    break;
  case 16: //rgb565
    for(i=0;i<32768;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;

      if((bool)config::snes.video_color_curve == true) {
        r = color_curve_table[r] >> 3;
        g = color_curve_table[g] >> 2;
        b = color_curve_table[b] >> 3;
      } else {
        g = (g << 1) | (g >> 4);
      }

      if((int)config::snes.video_color_adjust_mode == VCA_GRAYSCALE) {
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);

        l = int((double)r * lr) + ((double)g * lg) + ((double)b * lb);
        if(l <   0)l =   0;
        if(l > 255)l = 255;
        r = g = b = l;

        r >>= 3;
        g >>= 2;
        b >>= 3;
      } else if((int)config::snes.video_color_adjust_mode == VCA_VGA) {
      //rgb565->rgb332
        r >>= 2;
        g >>= 3;
        b >>= 3;

        r = (r << 2) | (r >> 1);
        g = (g << 3) | (g);
        b = (b << 3) | (b << 1) | (b >> 1);
      } else if((int)config::snes.video_color_adjust_mode == VCA_GENESIS) {
      //rgb565->rgb333
        r >>= 2;
        g >>= 3;
        b >>= 2;

        r = (r << 2) | (r >> 1);
        g = (g << 3) | (g);
        b = (b << 2) | (b >> 1);
      }

      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
    }
    break;
  default:
    alert("Error: SNES::update_color_lookup_table() -- color depth %d not supported", video.depth);
    break;
  }
}

void SNES::set_video_format(uint8 mode, uint8 depth) {
//only make changes at the start of a new frame
  video.format_changed = true;

  video_changed.mode  = mode;
  video_changed.depth = depth;
}

//internal function called at the start of the frame
//after SNES::set_video_format() is called
void SNES::update_video_format() {
  video.mode  = video_changed.mode;
  video.depth = video_changed.depth;

  update_color_lookup_table();
  video.format_changed = false;
}

void SNES::get_video_info(video_info *info) {
  info->data = video.data;
  info->mode = video.mode;

  switch(video.mode) {
  case VM_256x224:
    info->width  = 256;
    info->height = 224;
    break;
  case VM_512x224:
    info->width  = 512;
    info->height = 224;
    break;
  case VM_256x448:
    info->width  = 256;
    info->height = 448;
    break;
  case VM_512x448:
    info->width  = 512;
    info->height = 448;
    break;
  case VM_VARIABLE:
    if(video.frame_hires == false) {
      info->width  = 256;
    } else {
      info->width  = 512;
    }

    if(video.frame_interlace == false) {
      info->height = 224;
    } else {
      info->height = 448;
    }

    break;
  }
}

void SNES::video_update_256x224(uint16 *src) {
int x, y;
uint16 *dest;
  dest = video.data;

  for(y=0;y<224;y++) {
    if(video_frame[y].hires == false) {
      for(x=0;x<256;x++) {
        *dest++ = color_lookup_table[*src++];
      }
      src += 768;
    } else {
      for(x=0;x<256;x++) {
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
      src += 512;
    }
  }
}

void SNES::video_update_512x224(uint16 *src) {
int x, y;
uint16 *dest;
  dest = video.data;

  for(y=0;y<224;y++) {
    if(video_frame[y].hires == false) {
      for(x=0;x<256;x++) {
        *dest++ = color_lookup_table[*src];
        *dest++ = color_lookup_table[*src++];
      }
      src += 768;
    } else {
      for(x=0;x<512;x++) {
        *dest++ = color_lookup_table[*src++];
      }
      src += 512;
    }
  }
}

void SNES::video_update_256x448(uint16 *src) {
int x, y;
uint16 *dest;
bool field = !cpu->interlace_field();
  dest = video.data;

  for(y=0;y<224;y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*(src + (uint8)x)];
        }
        src += 1024;
      } else {
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*(src + ((x & 255) << 1))];
        }
        src += 1024;
      }
    } else {
      if(field) {
        dest += 256;
        src  += 512;
      }

      if(video_frame[y].hires == false) {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*src++];
        }
        src += 256;
      } else {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*src];
          src += 2;
        }
      }

      if(!field) {
        dest += 256;
        src  += 512;
      }
    }
  }
}

void SNES::video_update_512x448(uint16 *src) {
int x, y;
uint16 *dest;
bool field = !cpu->interlace_field();
  dest = video.data;

  for(y=0;y<224;y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*(src + (uint8)x)];
          *dest++ = color_lookup_table[*(src + (uint8)x)];
        }
        src += 1024;
      } else {
        for(x=0;x<1024;x++) {
          *dest++ = color_lookup_table[*(src + (x & 511))];
        }
        src += 1024;
      }
    } else {
      if(field) {
        dest += 512;
        src  += 512;
      }

      if(video_frame[y].hires == false) {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + x)];
          *dest++ = color_lookup_table[*(src + x)];
        }
        src += 512;
      } else {
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*src++];
        }
      }

      if(!field) {
        dest += 512;
        src  += 512;
      }
    }
  }
}

void SNES::video_update() {
  if(!ppu->render_frame())return;

  if(video.format_changed == true) {
    update_video_format();
  }

uint16 *src = (uint16*)video.ppu_data + ((int(cpu->overscan()) << 3) * 1024);
  switch(video.mode) {
  case VM_256x224:video_update_256x224(src);break;
  case VM_512x224:video_update_512x224(src);break;
  case VM_256x448:video_update_256x448(src);break;
  case VM_512x448:video_update_512x448(src);break;
  case VM_VARIABLE:
    switch(int(video.frame_hires) | (int(video.frame_interlace) << 1)) {
    case 0:video_update_256x224(src);break;
    case 1:video_update_512x224(src);break;
    case 2:video_update_256x448(src);break;
    case 3:video_update_512x448(src);break;
    }
    break;
  }

//SNES::capture_screenshot() was called by emulation interface
  if(flag_output_screenshot == true) {
    output_screenshot();
    flag_output_screenshot = false;
  }

  video_run();

  video.frame_hires     = false;
  video.frame_interlace = false;
}

void SNES::video_scanline() {
int y = cpu->vcounter();
int o = int(cpu->overscan()) << 3;
  if(y <= (0 + o) || y >= (224 + o))return;
  y -= o;

PPU::scanline_info si;
  ppu->get_scanline_info(&si);

  video_frame[y].hires     = si.hires;
  video_frame[y].interlace = si.interlace;

  video.frame_hires       |= si.hires;
  video.frame_interlace   |= si.interlace;
}

uint16 *SNES::get_ppu_output_handle() {
  return (uint16*)(video.ppu_data +
    (cpu->vcounter() * 1024) +
    ((cpu->interlace() && cpu->interlace_field())?512:0));
}

void SNES::video_init() {
int i, c;
  video.format_changed = false;

  video.data     = (uint16*)malloc(512 * 448 * sizeof(uint32));
  video.ppu_data = (uint16*)malloc(512 * 480 * sizeof(uint16));
  memset(video.data,     0, 512 * 448 * sizeof(uint32));
  memset(video.ppu_data, 0, 512 * 480 * sizeof(uint16));

  for(i=0;i<224;i++) {
    video_frame[i].hires     = false;
    video_frame[i].interlace = false;
  }

  video.frame_hires     = false;
  video.frame_interlace = false;

  set_video_format(VM_VARIABLE, 16);
  update_video_format();

  flag_output_screenshot = false;
}
