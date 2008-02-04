#include <windows.h>
#include <dsound.h>

#include <ruby/ruby.h>

namespace ruby {

#include "directsound.h"

class pAudioDS {
public:
  AudioDS &self;

  LPDIRECTSOUND ds;
  LPDIRECTSOUNDBUFFER dsb_p, dsb_b;
  DSBUFFERDESC dsbd;
  WAVEFORMATEX wfx;

  struct {
    uint32_t *buffer;
    unsigned buffer_pos, ring_pos;
    unsigned buffer_size, ring_size;
  } data;

  struct {
    HWND handle;
    bool synchronize;
    unsigned frequency;
  } settings;

  bool cap(Audio::Setting setting) {
    if(setting == Audio::Handle) return true;
    if(setting == Audio::Synchronize) return true;
    if(setting == Audio::Frequency) return true;
    return false;
  }

  uintptr_t get(Audio::Setting setting) {
    if(setting == Audio::Handle) return (uintptr_t)settings.handle;
    if(setting == Audio::Synchronize) return settings.synchronize;
    if(setting == Audio::Frequency) return settings.frequency;
    return false;
  }

  bool set(Audio::Setting setting, uintptr_t param) {
    if(setting == Audio::Handle) {
      settings.handle = (HWND)param;
      return true;
    }
    if(setting == Audio::Synchronize) {
      settings.synchronize = param;
      return true;
    }
    if(setting == Audio::Frequency) {
      settings.frequency = param;
      if(ds) init();
      return true;
    }
    return false;
  }

  void sample(uint16_t l_sample, uint16_t r_sample) {
    data.buffer[data.buffer_pos++] = (l_sample << 0) + (r_sample << 16);
    if(data.buffer_pos < settings.frequency / 40) return;

  DWORD ring_pos, pos, size;
    for(;;) {
      dsb_b->GetCurrentPosition(&pos, 0);
      ring_pos = pos / data.ring_size;
      if(settings.synchronize == false || ring_pos != data.ring_pos) break;
      Sleep(1);
    }

    data.ring_pos = ring_pos;
  void *output;
    if(dsb_b->Lock(((data.ring_pos + 2) % 3) * data.ring_size, data.ring_size,
      &output, &size, 0, 0, 0) == DS_OK) {
      memcpy(output, data.buffer, data.ring_size);
      dsb_b->Unlock(output, size, 0, 0);
    }

    data.buffer_pos = 0;
  }

  void clear() {
    data.buffer_pos = 0;
    data.ring_pos = 0;
    if(data.buffer) memset(data.buffer, 0, data.buffer_size);
    if(!dsb_b) return;

    dsb_b->Stop();
    dsb_b->SetCurrentPosition(0);

  DWORD size;
  void *output;
    dsb_b->Lock(0, data.ring_size * 3, &output, &size, 0, 0, 0);
    memset(output, 0, size);
    dsb_b->Unlock(output, size, 0, 0);

    dsb_b->Play(0, 0, DSBPLAY_LOOPING);
  }

  bool init() {
    clear();
    term();

    data.ring_size   = settings.frequency / 40 * sizeof(uint32_t);
    data.buffer_size = data.ring_size * 16;
    data.buffer      = (uint32_t*)malloc(data.buffer_size);
    data.buffer_pos  = 0;

    DirectSoundCreate(0, &ds, 0);
    ds->SetCooperativeLevel((HWND)settings.handle, DSSCL_PRIORITY);

    memset(&dsbd, 0, sizeof(dsbd));
    dsbd.dwSize        = sizeof(dsbd);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = 0;
    ds->CreateSoundBuffer(&dsbd, &dsb_p, 0);

    memset(&wfx, 0, sizeof(wfx));
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.nChannels       = 2;
    wfx.nSamplesPerSec  = settings.frequency;
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
    dsb_b->SetFrequency(settings.frequency);
    dsb_b->SetCurrentPosition(0);

    clear();
    return true;
  }

  void term() {
    if(data.buffer) {
      free(data.buffer);
      data.buffer = 0;
    }

    if(dsb_b) { dsb_b->Stop(); dsb_b->Release(); dsb_b = 0; }
    if(dsb_p) { dsb_p->Stop(); dsb_p->Release(); dsb_p = 0; }
    if(ds) { ds->Release(); ds = 0; }
  }

  pAudioDS(AudioDS &self_) : self(self_) {
    ds = 0;
    dsb_p = 0;
    dsb_b = 0;

    data.buffer = 0;
    data.buffer_pos = 0;
    data.ring_pos = 0;
    data.buffer_size = 0;
    data.ring_size = 0;

    settings.handle = GetDesktopWindow();
    settings.synchronize = false;
    settings.frequency = 22050;
  }
};

bool AudioDS::cap(Setting setting) { return p.cap(setting); }
uintptr_t AudioDS::get(Setting setting) { return p.get(setting); }
bool AudioDS::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
void AudioDS::sample(uint16_t l_sample, uint16_t r_sample) { p.sample(l_sample, r_sample); }
void AudioDS::clear() { p.clear(); }
bool AudioDS::init() { return p.init(); }
void AudioDS::term() { p.term(); }
AudioDS::AudioDS() : p(*new pAudioDS(*this)) {}
AudioDS::~AudioDS() { delete &p; }

} //namespace ruby
