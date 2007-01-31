void AudioDS::tick() {
}

void AudioDS::run(uint32 sample) {
  data.buffer[data.buffer_pos++] = sample;
  run_audiosync();
}

void AudioDS::run_videosync() {
  if(data.buffer_pos & 255)return;

uint32 ring_pos, pos, size;
  dsb_b->GetCurrentPosition(&pos, 0);
  ring_pos = pos / data.ring_size;
  if(ring_pos == data.ring_pos)return;

  data.ring_pos = ring_pos;
void *output;
  if(dsb_b->Lock(((data.ring_pos + 2) % 3) * data.ring_size,
  data.ring_size, &output, &size, 0, 0, 0) == DS_OK) {
    Audio::resample_point((uint32*)output, data.buffer, latency, data.buffer_pos);
    dsb_b->Unlock(output, size, 0, 0);
  }

  dprintf("AudioDS: resample_point() %d -> %d samples\n", data.buffer_pos, latency);
  data.buffer_pos = 0;
}

void AudioDS::run_audiosync() {
  if(data.buffer_pos < latency)return;

uint32 ring_pos, pos, size;
  do {
    Sleep(1);
    dsb_b->GetCurrentPosition(&pos, 0);
    ring_pos = pos / data.ring_size;
  } while(config::system.regulate_speed == true && ring_pos == data.ring_pos);

  data.ring_pos = ring_pos;
void *output;
  if(dsb_b->Lock(((data.ring_pos + 2) % 3) * data.ring_size,
  data.ring_size, &output, &size, 0, 0, 0) == DS_OK) {
    memcpy(output, data.buffer, data.ring_size);
    dsb_b->Unlock(output, size, 0, 0);
  }

  data.buffer_pos = 0;
}

void AudioDS::set_frequency(uint freq) {
  Audio::set_frequency(freq);
  init();
}

void AudioDS::clear_audio() {
  data.buffer_pos = 0;
  data.ring_pos   = 0;
  if(data.buffer) {
    memset(data.buffer, 0, data.buffer_size);
  }

  if(!dsb_b)return;

  dsb_b->Stop();
  dsb_b->SetCurrentPosition(0);

uint32 size;
void  *output;
  dsb_b->Lock(0, data.ring_size * 3, &output, &size, 0, 0, 0);
  memset(output, 0, size);
  dsb_b->Unlock(output, size, 0, 0);

  dsb_b->Play(0, 0, DSBPLAY_LOOPING);
}

void AudioDS::init() {
  clear_audio();
  term();

  data.ring_size   = latency * sizeof(uint32);
  data.buffer_size = data.ring_size * 16;
  data.buffer      = (uint32*)malloc(data.buffer_size);
  data.buffer_pos  = 0;

  DirectSoundCreate(0, &ds, 0);
  ds->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

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
  dsbd.dwBufferBytes   = data.ring_size * 3;
  dsbd.guid3DAlgorithm = GUID_NULL;
  dsbd.lpwfxFormat     = &wfx;
  ds->CreateSoundBuffer(&dsbd, &dsb_b, 0);
  dsb_b->SetFrequency(frequency);
  dsb_b->SetCurrentPosition(0);

  clear_audio();
}

void AudioDS::term() {
  safe_free(data.buffer);

  if(dsb_b) { dsb_b->Stop(); dsb_b->Release(); dsb_b = 0; }
  if(dsb_p) { dsb_p->Stop(); dsb_p->Release(); dsb_p = 0; }
  if(ds)    { ds->Release(); ds = 0; }
}

AudioDS::AudioDS(HWND handle) {
  hwnd  = handle ? handle : GetDesktopWindow();
  ds    = 0;
  dsb_p = 0;
  dsb_b = 0;

  data.buffer      = 0;
  data.buffer_pos  = 0;
  data.ring_pos    = 0;
  data.buffer_size = 0;
  data.ring_size   = 0;
}

AudioDS::~AudioDS() {
  term();
}
