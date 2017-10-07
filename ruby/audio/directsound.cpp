#include <dsound.h>

struct AudioDirectSound : Audio {
  AudioDirectSound() { initialize(); }
  ~AudioDirectSound() { terminate(); }

  auto availableDevices() -> string_vector {
    return {"Default"};
  }

  auto availableFrequencies() -> vector<double> {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> {
    return {40, 60, 80, 100};
  }

  auto availableChannels() -> vector<uint> {
    return {2};
  }

  auto ready() -> bool { return _ready; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return _frequency; }
  auto latency() -> uint { return _latency; }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;
    return true;
  }

  auto setFrequency(double frequency) -> bool {
    if(_frequency == frequency) return true;
    _frequency = frequency;
    return initialize();
  }

  auto setLatency(uint latency) -> bool {
    if(_latency == latency) return true;
    _latency = latency;
    return initialize();
  }

  auto clear() -> void {
    if(!ready()) return;

    _ringRead = 0;
    _ringWrite = _rings - 1;
    _ringDistance = _rings - 1;

    if(_buffer) memory::fill(_buffer, _period * _rings * 4);
    _offset = 0;

    if(!_secondary) return;
    _secondary->Stop();
    _secondary->SetCurrentPosition(0);

    void* output;
    DWORD size;
    _secondary->Lock(0, _period * _rings * 4, &output, &size, 0, 0, 0);
    memory::fill(output, size);
    _secondary->Unlock(output, size, 0, 0);

    _secondary->Play(0, 0, DSBPLAY_LOOPING);
  }

  auto output(const double samples[]) -> void {
    if(!ready()) return;

    _buffer[_offset]  = (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    _buffer[_offset] |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    if(++_offset < _period) return;
    _offset = 0;

    if(_blocking) {
      //wait until playback buffer has an empty ring to write new audio data to
      while(_ringDistance >= _rings - 1) {
        DWORD position;
        _secondary->GetCurrentPosition(&position, 0);
        uint ringActive = position / (_period * 4);
        if(ringActive == _ringRead) continue;

        //subtract number of played rings from ring distance counter
        _ringDistance -= (_rings + ringActive - _ringRead) % _rings;
        _ringRead = ringActive;

        if(_ringDistance < 2) {
          //buffer underflow; set max distance to recover quickly
          _ringDistance = _rings - 1;
          _ringWrite = (_rings + _ringRead - 1) % _rings;
          break;
        }
      }
    }

    _ringWrite = (_ringWrite + 1) % _rings;
    _ringDistance = (_ringDistance + 1) % _rings;

    void* output;
    DWORD size;
    if(_secondary->Lock(_ringWrite * _period * 4, _period * 4, &output, &size, 0, 0, 0) == DS_OK) {
      memory::copy(output, _buffer, _period * 4);
      _secondary->Unlock(output, size, 0, 0);
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    _rings = 8;
    _period = _frequency * _latency / _rings / 1000.0 + 0.5;
    _buffer = new uint32_t[_period * _rings];
    _offset = 0;

    if(DirectSoundCreate(0, &_interface, 0) != DS_OK) return terminate(), false;
    _interface->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY);

    DSBUFFERDESC primaryDescription = {};
    primaryDescription.dwSize = sizeof(DSBUFFERDESC);
    primaryDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
    primaryDescription.dwBufferBytes = 0;
    primaryDescription.lpwfxFormat = 0;
    _interface->CreateSoundBuffer(&primaryDescription, &_primary, 0);

    WAVEFORMATEX waveFormat = {};
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = _channels;
    waveFormat.nSamplesPerSec = (uint)_frequency;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    _primary->SetFormat(&waveFormat);

    DSBUFFERDESC secondaryDescription = {};
    secondaryDescription.dwSize = sizeof(DSBUFFERDESC);
    secondaryDescription.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE;
    secondaryDescription.dwBufferBytes = _period * _rings * 4;
    secondaryDescription.guid3DAlgorithm = GUID_NULL;
    secondaryDescription.lpwfxFormat = &waveFormat;
    _interface->CreateSoundBuffer(&secondaryDescription, &_secondary, 0);
    _secondary->SetFrequency((uint)_frequency);
    _secondary->SetCurrentPosition(0);

    _ready = true;
    clear();
    return true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_buffer) { delete[] _buffer; _buffer = nullptr; }
    if(_secondary) { _secondary->Stop(); _secondary->Release(); _secondary = nullptr; }
    if(_primary) { _primary->Stop(); _primary->Release(); _primary = nullptr; }
    if(_interface) { _interface->Release(); _interface = nullptr; }
  }

  bool _ready = false;
  bool _blocking = true;
  uint _channels = 2;
  double _frequency = 48000.0;
  uint _latency = 40;

  LPDIRECTSOUND _interface = nullptr;
  LPDIRECTSOUNDBUFFER _primary = nullptr;
  LPDIRECTSOUNDBUFFER _secondary = nullptr;

  uint32_t* _buffer = nullptr;
  uint _offset = 0;

  uint _period = 0;
  uint _rings = 0;
  uint _ringRead = 0;
  uint _ringWrite = 0;
  int _ringDistance = 0;
};
