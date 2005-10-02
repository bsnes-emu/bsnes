void SNES::set_playback_buffer_size(uint32 buffer_size) {
  if(dsp_buffer.data) {
    free(dsp_buffer.data);
    dsp_buffer.data = 0;
  }

//* 2 is for left/right channel data
  dsp_buffer.data = (uint16*)malloc(buffer_size * sizeof(uint16) * 2);
  memset(dsp_buffer.data, 0, buffer_size * sizeof(uint16) * 2);

  dsp_buffer.size = buffer_size;
  dsp_buffer.pos  = 0;
}

uint32 SNES::get_playback_buffer_pos() {
  return dsp_buffer.pos;
}

uint16 *SNES::get_playback_buffer() {
  return dsp_buffer.data;
}

void SNES::audio_update(uint32 data) {
  if(pcmfp) {
    fputc(data,       pcmfp);
    fputc(data >>  8, pcmfp);
    fputc(data >> 16, pcmfp);
    fputc(data >> 24, pcmfp);
  }

  if((bool)config::snes.mute == true)data = 0x0000;

  dsp_buffer.data[dsp_buffer.pos++] = (data)       & 0xffff;
  dsp_buffer.data[dsp_buffer.pos++] = (data >> 16) & 0xffff;
  dsp_buffer.pos %= dsp_buffer.size;

  sound_run();
}

void SNES::log_audio_enable(const char *fn) {
char tfn[256];
int i;
  if(pcmfp)log_audio_disable();

  if(!fn) {
    for(i=0;i<=999;i++) {
      sprintf(tfn, "audio%0.3d.wav", i);
      pcmfp = fopen(tfn, "rb");
      if(!pcmfp)break;
      fclose(pcmfp);
      pcmfp = 0;
    }
    if(i >= 1000)return;
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

void SNES::log_audio_disable() {
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

void SNES::audio_init() {
}

void SNES::audio_term() {
  log_audio_disable();
}
