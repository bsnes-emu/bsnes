#include "xaudio2.hpp"
#include <windows.h>

struct AudioXAudio2 : Audio, public IXAudio2VoiceCallback {
  AudioXAudio2() { initialize(); }
  ~AudioXAudio2() { terminate(); }

  auto availableDevices() -> string_vector {
    return {"Default"};
  }

  auto availableFrequencies() -> vector<double> {
    return {44100.0, 48000.0, 96000.0};
  }

  auto availableLatencies() -> vector<uint> {
    return {20, 40, 60, 80, 100};
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
    if(!_sourceVoice) return;
    _sourceVoice->Stop(0);
    _sourceVoice->FlushSourceBuffers();  //calls OnBufferEnd for all currently submitted buffers

    _bufferIndex = 0;

    _bufferOffset = 0;
    if(_buffer) memory::fill<uint32_t>(_buffer, _period * _bufferCount);

    _sourceVoice->Start(0);
  }

  auto output(const double samples[]) -> void {
    _buffer[_bufferIndex * _period + _bufferOffset]  = (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    _buffer[_bufferIndex * _period + _bufferOffset] |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    if(++_bufferOffset < _period) return;
    _bufferOffset = 0;

    if(_bufferQueue == _bufferCount - 1) {
      if(_blocking) {
        //wait until there is at least one other free buffer for the next sample
        while(_bufferQueue == _bufferCount - 1);
      } else {  //we need one free buffer for the next sample, so ignore the current contents
        return;
      }
    }

    pushBuffer(_period * 4, _buffer + _bufferIndex * _period);
    _bufferIndex = (_bufferIndex + 1) % _bufferCount;
  }

private:
  auto initialize() -> bool {
    terminate();

    _bufferCount = 8;
    _period = _frequency * _latency / _bufferCount / 1000.0 + 0.5;
    _buffer = new uint32_t[_period * _bufferCount];
    _bufferOffset = 0;
    _bufferIndex = 0;
    _bufferQueue = 0;

    if(FAILED(XAudio2Create(&_interface, 0 , XAUDIO2_DEFAULT_PROCESSOR))) return false;

    uint deviceCount = 0;
    _interface->GetDeviceCount(&deviceCount);
    if(deviceCount == 0) return terminate(), false;

    uint deviceID = 0;
    for(uint deviceIndex : range(deviceCount)) {
      XAUDIO2_DEVICE_DETAILS deviceDetails = {};
      _interface->GetDeviceDetails(deviceIndex, &deviceDetails);
      if(deviceDetails.Role & DefaultGameDevice) deviceID = deviceIndex;
    }

    if(FAILED(_interface->CreateMasteringVoice(&_masterVoice, _channels, (uint)_frequency, 0, deviceID, nullptr))) return terminate(), false;

    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = _channels;
    waveFormat.nSamplesPerSec = (uint)_frequency;
    waveFormat.nBlockAlign = 4;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    if(FAILED(_interface->CreateSourceVoice(&_sourceVoice, (WAVEFORMATEX*)&waveFormat, XAUDIO2_VOICE_NOSRC, XAUDIO2_DEFAULT_FREQ_RATIO, this, nullptr, nullptr))) return terminate(), false;

    clear();
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;

    if(_sourceVoice) {
      _sourceVoice->Stop(0);
      _sourceVoice->DestroyVoice();
      _sourceVoice = nullptr;
    }

    if(_masterVoice) {
      _masterVoice->DestroyVoice();
      _masterVoice = nullptr;
    }

    if(_interface) {
      _interface->Release();
      _interface = nullptr;
    }

    delete[] _buffer;
    _buffer = nullptr;
  }

  auto pushBuffer(uint bytes, uint32_t* _audioData) -> void {
    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = bytes;
    buffer.pAudioData = reinterpret_cast<BYTE*>(_audioData);
    buffer.pContext = 0;
    InterlockedIncrement(&_bufferQueue);
    _sourceVoice->SubmitSourceBuffer(&buffer);
  }

  bool _ready = false;
  bool _blocking = true;
  uint _channels = 2;
  double _frequency = 48000.0;
  uint _latency = 80;

  uint32_t* _buffer = nullptr;
  uint _period = 0;
  uint _bufferCount = 0;
  uint _bufferOffset = 0;
  uint _bufferIndex = 0;
  volatile long _bufferQueue = 0;  //how many buffers are queued and ready for playback

  IXAudio2* _interface = nullptr;
  IXAudio2MasteringVoice* _masterVoice = nullptr;
  IXAudio2SourceVoice* _sourceVoice = nullptr;

  //inherited from IXAudio2VoiceCallback
  STDMETHODIMP_(void) OnBufferStart(void* pBufferContext){}
  STDMETHODIMP_(void) OnLoopEnd(void* pBufferContext){}
  STDMETHODIMP_(void) OnStreamEnd() {}
  STDMETHODIMP_(void) OnVoiceError(void* pBufferContext, HRESULT Error) {}
  STDMETHODIMP_(void) OnVoiceProcessingPassEnd() {}
  STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) {}

  STDMETHODIMP_(void) OnBufferEnd(void* pBufferContext) {
    InterlockedDecrement(&_bufferQueue);
  }
};
