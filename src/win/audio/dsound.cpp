void AudioDS::run(uint32 sample) {
  data.buffer[data.buffer_pos++] = sample;

  if(data.buffer_pos >= data.samples_per_frame) {
  uint32 pos, size;
  void  *buffer;
    if((bool)config::system.regulate_speed == true) {
      do {
        dsb_b->GetCurrentPosition(&pos, 0);
        data.read_buffer = pos / data.buffer_size;
      } while(data.read_buffer == data.prev_buffer);
    }

    data.prev_buffer = data.read_buffer;
    data.read_buffer++;
    data.read_buffer &= 7;

    pos = (data.read_buffer + 1) & 7;

    if(dsb_b->Lock(pos * data.buffer_size,
    data.buffer_size, &buffer, &size, 0, 0, 0) == DS_OK) {
      memcpy(buffer, data.buffer, data.buffer_size);
      dsb_b->Unlock(buffer, size, 0, 0);
    }

    data.buffer_pos = 0;
  }
}

void AudioDS::set_frequency(uint32 new_freq) {
  frequency = new_freq;
  init();
}

void AudioDS::clear_audio() {
  data.read_buffer = 0;
  data.prev_buffer = 0;
  data.buffer_pos  = 0;
  memset(data.buffer, 0, 2048 * 4);

  if(!dsb_b)return;

  dsb_b->Stop();
  dsb_b->SetCurrentPosition(0);

uint32 size;
void  *buffer;
  dsb_b->Lock(0, data.buffer_size * 8, &buffer, &size, 0, 0, 0);
  memset(buffer, 0, data.buffer_size * 8);
  dsb_b->Unlock(buffer, size, 0, 0);

  dsb_b->Play(0, 0, DSBPLAY_LOOPING);
}

void AudioDS::init() {
  clear_audio();
  term();

  data.samples_per_frame = frequency / ((snes->region() == SNES::NTSC) ? 60 : 50);
  data.buffer_size       = data.samples_per_frame * 4;

  DirectSoundCreate(0, &ds, 0);
  ds->SetCooperativeLevel(wMain.hwnd, DSSCL_PRIORITY);

  memset(&dsbd, 0, sizeof(dsbd));
  dsbd.dwSize        = sizeof(dsbd);
  dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
  dsbd.dwBufferBytes = 0;
  dsbd.lpwfxFormat   = 0;
  ds->CreateSoundBuffer(&dsbd, &dsb_p, 0);

  memset(&wfx, 0, sizeof(wfx));
  wfx.wFormatTag      = WAVE_FORMAT_PCM;
  wfx.nChannels       = 2;
  wfx.nSamplesPerSec  = frequency;
  wfx.wBitsPerSample  = 16;
  wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
  dsb_p->SetFormat(&wfx);

  memset(&dsbd, 0, sizeof(dsbd));
  dsbd.dwSize          = sizeof(dsbd);
  dsbd.dwFlags         = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLFREQUENCY |
                         DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE;
  dsbd.dwBufferBytes   = data.buffer_size * 8;
  dsbd.guid3DAlgorithm = GUID_NULL;
  dsbd.lpwfxFormat     = &wfx;
  ds->CreateSoundBuffer(&dsbd, &dsb_b, 0);
  dsb_b->SetFrequency(frequency);
  dsb_b->SetCurrentPosition(0);

uint32 size;
void  *buffer;
  dsb_b->Lock(0, data.buffer_size * 8, &buffer, &size, 0, 0, 0);
  memset(buffer, 0, data.buffer_size * 8);
  dsb_b->Unlock(buffer, size, 0, 0);

  data.read_buffer = 0;
  dsb_b->Play(0, 0, DSBPLAY_LOOPING);
}

void AudioDS::term() {
  if(dsb_b) { dsb_b->Stop(); dsb_b->Release(); dsb_b = 0; }
  if(dsb_p) { dsb_p->Stop(); dsb_p->Release(); dsb_p = 0; }
  if(ds) { ds->Release(); ds = 0; }
}
