DSSound::DSSound() {
  data.buffer = 0;
  data.lpos = data.lsample = data.lbuffer = 0;
}

void DSSound::run() {
  if(snes->get_playback_buffer_pos() != 0)return;

uint32 pos, status;

//dsb_b[0]->SetFrequency(22050);

//do {
//  dsb_b[0]->GetStatus(&status);
//} while(status & DSBSTATUS_PLAYING);

  dsb_b[0]->Lock(0, DSP_BUFFER_SIZE * 4, &dslb, &dslbs, 0, 0, 0);
  memcpy(dslb, snes->get_playback_buffer(), DSP_BUFFER_SIZE * 4);
  dsb_b[0]->Unlock(dslb, dslbs, 0, 0);

  dsb_b[0]->SetCurrentPosition(0);

//has the buffer stopped (possibly due to running too fast)?
  dsb_b[0]->GetStatus(&status);
  if(!(status & DSBSTATUS_PLAYING)) {
    dsb_b[0]->Play(0, 0, 0);
  }
}

void DSSound::init() {
  DirectSoundCreate(0, &ds, 0);
  ds->SetCooperativeLevel(w_main->hwnd, DSSCL_PRIORITY);

  memset(&dsbd, 0, sizeof(dsbd));
  dsbd.dwSize = sizeof(dsbd);
  dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
  dsbd.dwBufferBytes = 0;
  dsbd.lpwfxFormat = 0;
  ds->CreateSoundBuffer(&dsbd, &dsb_p, 0);

  memset(&wfx, 0, sizeof(wfx));
  wfx.wFormatTag = WAVE_FORMAT_PCM;
  wfx.nChannels = 2;
  wfx.nSamplesPerSec = 32000;
  wfx.wBitsPerSample = 16;
  wfx.nBlockAlign = wfx.wBitsPerSample / 8 * wfx.nChannels;
  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
  dsb_p->SetFormat(&wfx);

  sample_size = (16 / 8) * 2;
  buffer_size = DSP_BUFFER_SIZE * sample_size;
  buffer_pos  = 0;

  dsb_b = new LPDIRECTSOUNDBUFFER[1];
  memset(&dsbd, 0, sizeof(dsbd));
  dsbd.dwSize = sizeof(dsbd);
  dsbd.dwFlags = DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS;
  dsbd.dwBufferBytes = buffer_size;
  dsbd.guid3DAlgorithm = GUID_NULL;
  dsbd.lpwfxFormat = &wfx;
  ds->CreateSoundBuffer(&dsbd, &dsb_b[0], 0);
  ds->CreateSoundBuffer(&dsbd, &dsb_b[1], 0);
  dsb_b[0]->SetFrequency(32000);
  dsb_b[1]->SetFrequency(32000);

  dsb_b[0]->Lock(0, buffer_size, &dslb, &dslbs, 0, 0, 0);
  memset(dslb, 0, buffer_size);
  dsb_b[0]->Unlock(dslb, dslbs, 0, 0);

  dsb_b[1]->Lock(0, buffer_size, &dslb, &dslbs, 0, 0, 0);
  memset(dslb, 0, buffer_size);
  dsb_b[1]->Unlock(dslb, dslbs, 0, 0);

  dsb_b[0]->Play(0, 0, 0);

  buffer_pos = 0;
}
