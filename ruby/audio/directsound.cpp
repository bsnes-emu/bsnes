#include <dsound.h>

struct AudioDS : Audio {
  ~AudioDS() { term(); }

  LPDIRECTSOUND ds = nullptr;
  LPDIRECTSOUNDBUFFER dsb_p = nullptr;
  LPDIRECTSOUNDBUFFER dsb_b = nullptr;
  DSBUFFERDESC dsbd;
  WAVEFORMATEX wfx;

  struct {
    uint rings = 0;
    uint latency = 0;

    uint32_t* buffer = nullptr;
    uint bufferoffset = 0;

    uint readring = 0;
    uint writering = 0;
    int distance = 0;
  } device;

  struct {
    HWND handle = nullptr;
    bool synchronize = false;
    uint frequency = 48000;
    uint latency = 120;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Handle) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    if(name == Audio::Latency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Handle) return (uintptr_t)settings.handle;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    if(name == Audio::Latency) return settings.latency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Handle && value.is<uintptr_t>()) {
      settings.handle = (HWND)value.get<uintptr_t>();
      return true;
    }

    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      if(ds) clear();
      return true;
    }

    if(name == Audio::Frequency && value.is<uint>()) {
      settings.frequency = value.get<uint>();
      if(ds) init();
      return true;
    }

    if(name == Audio::Latency && value.is<uint>()) {
      //latency settings below 40ms causes DirectSound to hang
      settings.latency = max(40u, value.get<uint>());
      if(ds) init();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
    device.buffer[device.bufferoffset++] = (uint16_t)left << 0 | (uint16_t)right << 16;
    if(device.bufferoffset < device.latency) return;
    device.bufferoffset = 0;

    DWORD pos, size;
    void* output;

    if(settings.synchronize) {
      //wait until playback buffer has an empty ring to write new audio data to
      while(device.distance >= device.rings - 1) {
        dsb_b->GetCurrentPosition(&pos, 0);
        uint activering = pos / (device.latency * 4);
        if(activering == device.readring) continue;

        //subtract number of played rings from ring distance counter
        device.distance -= (device.rings + activering - device.readring) % device.rings;
        device.readring = activering;

        if(device.distance < 2) {
          //buffer underflow; set max distance to recover quickly
          device.distance  = device.rings - 1;
          device.writering = (device.rings + device.readring - 1) % device.rings;
          break;
        }
      }
    }

    device.writering = (device.writering + 1) % device.rings;
    device.distance  = (device.distance  + 1) % device.rings;

    if(dsb_b->Lock(device.writering * device.latency * 4, device.latency * 4, &output, &size, 0, 0, 0) == DS_OK) {
      memcpy(output, device.buffer, device.latency * 4);
      dsb_b->Unlock(output, size, 0, 0);
    }
  }

  auto clear() -> void {
    device.readring  = 0;
    device.writering = device.rings - 1;
    device.distance  = device.rings - 1;

    device.bufferoffset = 0;
    if(device.buffer) memset(device.buffer, 0, device.latency * device.rings * 4);

    if(!dsb_b) return;
    dsb_b->Stop();
    dsb_b->SetCurrentPosition(0);

    DWORD size;
    void* output;
    dsb_b->Lock(0, device.latency * device.rings * 4, &output, &size, 0, 0, 0);
    memset(output, 0, size);
    dsb_b->Unlock(output, size, 0, 0);

    dsb_b->Play(0, 0, DSBPLAY_LOOPING);
  }

  auto init() -> bool {
    settings.handle = GetDesktopWindow();

    device.rings   = 8;
    device.latency = settings.frequency * settings.latency / device.rings / 1000.0 + 0.5;
    device.buffer  = new uint32_t[device.latency * device.rings];
    device.bufferoffset = 0;

    if(DirectSoundCreate(0, &ds, 0) != DS_OK) return term(), false;
    ds->SetCooperativeLevel((HWND)settings.handle, DSSCL_PRIORITY);

    memory::fill(&dsbd, sizeof(dsbd));
    dsbd.dwSize        = sizeof(dsbd);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = 0;
    ds->CreateSoundBuffer(&dsbd, &dsb_p, 0);

    memory::fill(&wfx, sizeof(wfx));
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nChannels       = 2;
    wfx.nSamplesPerSec  = settings.frequency;
    wfx.wBitsPerSample  = 16;
    wfx.nBlockAlign     = wfx.wBitsPerSample / 8 * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    dsb_p->SetFormat(&wfx);

    memory::fill(&dsbd, sizeof(dsbd));
    dsbd.dwSize  = sizeof(dsbd);
    dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes   = device.latency * device.rings * sizeof(uint32_t);
    dsbd.guid3DAlgorithm = GUID_NULL;
    dsbd.lpwfxFormat     = &wfx;
    ds->CreateSoundBuffer(&dsbd, &dsb_b, 0);
    dsb_b->SetFrequency(settings.frequency);
    dsb_b->SetCurrentPosition(0);

    clear();
    return true;
  }

  auto term() -> void {
    if(device.buffer) {
      delete[] device.buffer;
      device.buffer = nullptr;
    }

    if(dsb_b) { dsb_b->Stop(); dsb_b->Release(); dsb_b = nullptr; }
    if(dsb_p) { dsb_p->Stop(); dsb_p->Release(); dsb_p = nullptr; }
    if(ds) { ds->Release(); ds = nullptr; }
  }
};
