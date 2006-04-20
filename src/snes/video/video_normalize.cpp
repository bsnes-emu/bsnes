void SNES::video_normalize() {
}

/*
void SNES::video_normalize_256x223() {
uint16 *src  = video.ppu_data + 1024;
uint16 *dest = video.raster_data;
  for(int y = 1; y < 224; y++) {
    if(video_frame[y].hires == false) {
      for(int x = 0; x < 256; x++) {
        *dest++ = *src++;
      }
      src += 768;
    } else {
      for(int x = 0; x < 256; x++) {
        *dest++ = *src;
        src += 2;
      }
    }
    dest += 256;
  }
}

void SNES::video_normalize_512x223() {
uint16 *src  = video.ppu_data + 1024;
uint16 *dest = video.raster_data;
  for(int y = 1; y < 224; y++) {
    if(video_frame[y].hires == false) {
      for(int x = 0; x < 256; x++) {
        *dest++ = *src;
        *dest++ = *src++;
      }
      src += 768;
    } else {
      for(int x = 0; x < 512; x++) {
        *dest++ = *src++;
      }
      src += 512;
    }
  }
}

void SNES::video_normalize_256x446() {
uint16 *src   = video.ppu_data + 1024;
uint16 *dest  = video.raster_data;
bool    field = !r_cpu->interlace_field();
  for(int y = 1; y < 224; y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + x);
        }
        dest += 256;
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + x);
        }
        dest += 256;
      } else {
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + (x << 1));
        }
        dest += 256;
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + (x << 1));
        }
        dest += 256;
      }
      src += 1024;
    } else {
      if(field) {
        dest += 512;
        src  += 512;
      }

      if(video_frame[y].hires == false) {
        for(int x = 0; x < 256; x++) {
          *dest++ = *src++;
        }
        src += 256;
      } else {
        for(int x = 0; x < 256; x++) {
          *dest++ = *src;
          src += 2;
        }
      }
      dest += 256;

      if(!field) {
        dest += 512;
        src  += 512;
      }
    }
  }
}

void SNES::video_normalize_512x446() {
uint16 *src   = video.ppu_data + 1024;
uint16 *dest  = video.raster_data;
bool    field = !r_cpu->interlace_field();
  for(int y = 1; y < 224; y++) {
    if(video_frame[y].interlace == false) {
      if(video_frame[y].hires == false) {
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + x);
          *dest++ = *(src + x);
        }
        dest += 512;
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + x);
          *dest++ = *(src + x);
        }
        dest += 512;
      } else {
        for(int x = 0; x < 512; x++) {
          *dest++ = *(src + x);
        }
        dest += 512;
        for(int x = 0; x < 512; x++) {
          *dest++ = *(src + x);
        }
        dest += 512;
      }
      src += 1024;
    } else {
      if(field) {
        dest += 512;
        src  += 512;
      }

      if(video_frame[y].hires == false) {
        for(int x = 0; x < 256; x++) {
          *dest++ = *(src + x);
          *dest++ = *(src + x);
        }
        src += 512;
      } else {
        for(int x = 0; x < 512; x++) {
          *dest++ = *src++;
        }
      }
      dest += 512;

      if(!field) {
        dest += 512;
        src  += 512;
      }
    }
  }
}
*/
