#include "snes_video_ex.cpp"

const uint8 SNES::color_curve_table[32] = {
  0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
  0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
  0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
  0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff
};

void SNES::update_color_lookup_table() {
int32 i, l, r, g, b;
double lr = 0.2126, lg = 0.7152, lb = 0.0722; //luminance
uint32 col;

  for(i=0;i<32768;i++) {
  //bgr555->rgb888
    col = ((i & 0x001f) << 19) | ((i & 0x001c) << 14) |
          ((i & 0x03e0) <<  6) | ((i & 0x0380) <<  1) |
          ((i & 0x7c00) >>  7) | ((i & 0x7000) >> 12);

    r = (col >> 16) & 0xff;
    g = (col >>  8) & 0xff;
    b = (col      ) & 0xff;

    if((bool)config::snes.video_color_curve == true) {
      r = color_curve_table[r >> 3];
      g = color_curve_table[g >> 3];
      b = color_curve_table[b >> 3];
    }

    if((int)config::snes.video_color_adjust_mode == VCA_GRAYSCALE) {
      l = int32(((double)r * lr) + ((double)g * lg) + ((double)b * lb));
      if(l <   0)l =   0;
      if(l > 255)l = 255;
      r = g = b = l;
    } else if((int)config::snes.video_color_adjust_mode == VCA_VGA) {
      r &= 0xe0;
      g &= 0xe0;
      b &= 0xc0;
      r |= (r >> 3) | (r >> 6);
      g |= (g >> 3) | (g >> 6);
      b |= (b >> 2) | (b >> 4) | (b >> 6);
    } else if((int)config::snes.video_color_adjust_mode == VCA_GENESIS) {
      r &= 0xe0;
      g &= 0xe0;
      b &= 0xe0;
      r |= (r >> 3) | (r >> 6);
      g |= (g >> 3) | (g >> 6);
      b |= (b >> 3) | (b >> 6);
    }

    switch(video.depth) {
    case 15:
      r >>= 3;
      g >>= 3;
      b >>= 3;
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
      break;
    case 16:
      r >>= 3;
      g >>= 2;
      b >>= 3;
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
      break;
    case 24:
    case 32:
      color_lookup_table[i] = (r << 16) | (g << 8) | (b);
      break;
    default:
      alert("Error: SNES::update_color_lookup_table() -- color depth %d not supported", video.depth);
      break;
    }
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

void SNES::video_update_256x224() {
int x, y;
uint16 *src  = video.ppu_data;
uint16 *dest = video.data;

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
    dest += video.pitch - 256;
  }
}

void SNES::video_update_512x224() {
int x, y;
uint16 *src  = video.ppu_data;
uint16 *dest = video.data;

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
    dest += video.pitch - 512;
  }
}

void SNES::video_update_256x448() {
int x, y;
uint16 *src   = video.ppu_data;
uint16 *dest  = video.data;
bool    field = !r_cpu->interlace_field();

  for(y=0;y<224;y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 256;
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 256;
      } else {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + (x << 1))];
        }
        dest += video.pitch - 256;
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + (x << 1))];
        }
        dest += video.pitch - 256;
      }
      src += 1024;
    } else {
      if(field) {
        dest += video.pitch;
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
      dest += video.pitch - 256;

      if(!field) {
        dest += video.pitch;
        src  += 512;
      }
    }
  }
}

void SNES::video_update_512x448() {
int x, y;
uint16 *src   = video.ppu_data;
uint16 *dest  = video.data;
bool    field = !r_cpu->interlace_field();

  for(y=0;y<224;y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + x)];
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 512;
        for(x=0;x<256;x++) {
          *dest++ = color_lookup_table[*(src + x)];
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 512;
      } else {
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 512;
        for(x=0;x<512;x++) {
          *dest++ = color_lookup_table[*(src + x)];
        }
        dest += video.pitch - 512;
      }
      src += 1024;
    } else {
      if(field) {
        dest += video.pitch;
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
      dest += video.pitch - 512;

      if(!field) {
        dest += video.pitch;
        src  += 512;
      }
    }
  }
}

void SNES::video_update() {
  if(r_ppu->renderer_enabled()) {
    if(video.format_changed == true) {
      update_video_format();
    }

    video.data     = (uint16*)video_lock(video.pitch);
    video.ppu_data = (uint16*)r_ppu->output + (int(r_cpu->overscan()) << 13);
    video.pitch  >>= 1;
    if(video.data) {
      switch(video.mode) {
      case VM_256x224:video_update_256x224();break;
      case VM_512x224:video_update_512x224();break;
      case VM_256x448:video_update_256x448();break;
      case VM_512x448:video_update_512x448();break;
      case VM_VARIABLE:
        switch(int(video.frame_hires) | (int(video.frame_interlace) << 1)) {
        case 0:video_update_256x224();break;
        case 1:video_update_512x224();break;
        case 2:video_update_256x448();break;
        case 3:video_update_512x448();break;
        }
        break;
      }
      video_unlock();
    }

  //SNES::capture_screenshot() was called by emulation interface
    if(flag_output_screenshot == true) {
      output_screenshot();
      flag_output_screenshot = false;
    }
  }

  video_run();

  video.frame_hires     = false;
  video.frame_interlace = false;
}

void SNES::video_scanline() {
int y = r_cpu->vcounter();
int o = int(r_cpu->overscan()) << 3;
  if(y <= (0 + o) || y >= (224 + o))return;
  y -= o;

PPU::scanline_info si;
  r_ppu->get_scanline_info(&si);

  video_frame[y].hires     = si.hires;
  video_frame[y].interlace = si.interlace;

  video.frame_hires       |= si.hires;
  video.frame_interlace   |= si.interlace;
}

void SNES::video_init() {
int i, c;
  video.format_changed = false;

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
