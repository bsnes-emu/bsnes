#ifdef SNES_CPP

//TODO: move audio logging code to SNESInterface class

void SNES::Audio::update(uint16 l_sample, uint16 r_sample) {
  if(pcmfp) {
    fputc(l_sample >> 0, pcmfp);
    fputc(l_sample >> 8, pcmfp);
    fputc(r_sample >> 0, pcmfp);
    fputc(r_sample >> 8, pcmfp);
  }

  snesinterface.audio_sample(l_sample, r_sample);
}

void SNES::Audio::log_enable(const char *fn) {
  if(pcmfp) log_disable();

  char tfn[256];
  if(!fn) {
  int i = 0;
    while(i < 1000) {
      sprintf(tfn, "audio%0.3d.wav", i);
      pcmfp = fopen(tfn, "rb");
      if(!pcmfp) break;
      fclose(pcmfp);
      pcmfp = 0;
      i++;
    }
    if(i >= 1000) return;
  } else {
    strcpy(tfn, fn);
  }

  pcmfp = fopen(tfn, "wb");
  if(!pcmfp)return;

  //header
  fwrite("RIFF", 1, 4, pcmfp);

  //file size
  fputc(0, pcmfp);
  fputc(0, pcmfp);
  fputc(0, pcmfp);
  fputc(0, pcmfp);

  //format
  fwrite("WAVE", 1, 4, pcmfp);
  fwrite("fmt ", 1, 4, pcmfp);

  //fmt size
  fputc(0x12, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);

  //fmt type (PCM)
  fputc(1, pcmfp);
  fputc(0, pcmfp);

  //channels
  fputc(2, pcmfp);
  fputc(0, pcmfp);

  //sample rate (32000hz)
  fputc(0x00, pcmfp);
  fputc(0x7d, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);

  //byte rate (32000 * 2 * (16 / 8)
  fputc(0x00, pcmfp);
  fputc(0xf4, pcmfp);
  fputc(0x01, pcmfp);
  fputc(0x00, pcmfp);

  //block align (bytes per sample) (4)
  fputc(4, pcmfp);
  fputc(0, pcmfp);

  //???
  fputc(0x10, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);
  fwrite("fact", 1, 4, pcmfp);
  fputc(0x04, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x00, pcmfp);
  fputc(0x0b, pcmfp);
  fputc(0xf4, pcmfp);
  fputc(0x01, pcmfp);
  fputc(0x00, pcmfp);

  //data
  fwrite("data", 1, 4, pcmfp);

  //data size
  fputc(0, pcmfp);
  fputc(0, pcmfp);
  fputc(0, pcmfp);
  fputc(0, pcmfp);
}

void SNES::Audio::log_disable() {
  if(pcmfp) {
    int fsize, t;
    fseek(pcmfp, 0, SEEK_END);
    fsize = ftell(pcmfp);
    fseek(pcmfp, 4, SEEK_SET);
    t = fsize - 2;
    fputc(t,       pcmfp);
    fputc(t >> 8,  pcmfp);
    fputc(t >> 16, pcmfp);
    fputc(t >> 24, pcmfp);
    fseek(pcmfp, 0x36, SEEK_SET);
    t = fsize - 0x3a;
    fputc(t,       pcmfp);
    fputc(t >> 8,  pcmfp);
    fputc(t >> 16, pcmfp);
    fputc(t >> 24, pcmfp);
    fclose(pcmfp);
    pcmfp = 0;
  }
}

void SNES::Audio::init() {
  pcmfp = 0;
}

void SNES::Audio::term() {
  log_disable();
}

#endif //ifdef SNES_CPP
