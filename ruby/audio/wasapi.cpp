#include <avrt.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>

struct AudioWASAPI : Audio {
  AudioWASAPI() { initialize(); }
  ~AudioWASAPI() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto information() -> Information {
    Information information;
    information.devices = {"Default"};
    information.channels = {2};
    information.frequencies = {(double)_frequency};
    information.latencies = {20, 40, 60, 80, 100};
    return information;
  }

  auto exclusive() -> bool { return _exclusive; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return (double)_frequency; }
  auto latency() -> uint { return _latency; }

  auto setExclusive(bool exclusive) -> bool {
    if(_exclusive == exclusive) return true;
    _exclusive = exclusive;
    return initialize();
  }

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
    _queue.read = 0;
    _queue.write = 0;
    _queue.count = 0;
    if(!_audioClient) return;
    _audioClient->Stop();
    _audioClient->Reset();
    _audioClient->Start();
  }

  auto output(const double samples[]) -> void {
    if(_queue.count < _bufferSize) {
      for(uint n : range(_channels)) {
        _queue.samples[_queue.write][n] = samples[n];
      }
      _queue.write++;
      _queue.count++;
    } else if(WaitForSingleObject(_eventHandle, _blocking ? INFINITE : 0) == WAIT_OBJECT_0) {
      write();
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&_enumerator) != S_OK) return false;
    if(_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_audioDevice) != S_OK) return false;
    if(_audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_audioClient) != S_OK) return false;

    if(_exclusive) {
      if(_audioDevice->OpenPropertyStore(STGM_READ, &_propertyStore) != S_OK) return false;
      if(_propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &_propVariant) != S_OK) return false;
      _waveFormat = (WAVEFORMATEX*)_propVariant.blob.pBlobData;
      if(_audioClient->GetDevicePeriod(nullptr, &_devicePeriod) != S_OK) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      auto result = _audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, _waveFormat, nullptr);
      if(result == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
        if(_audioClient->GetBufferSize(&_bufferSize) != S_OK) return false;
        _audioClient->Release();
        latency = (REFERENCE_TIME)(10'000 * 1'000 * _bufferSize / _waveFormat->nSamplesPerSec);
        if(_audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_audioClient) != S_OK) return false;
        result = _audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, _waveFormat, nullptr);
      }
      if(result != S_OK) return false;
      DWORD taskIndex = 0;
      _taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      if(_audioClient->GetMixFormat(&_waveFormat) != S_OK) return false;
      if(_audioClient->GetDevicePeriod(&_devicePeriod, nullptr)) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      if(_audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, 0, _waveFormat, nullptr) != S_OK) return false;
    }

    _eventHandle = CreateEvent(nullptr, false, false, nullptr);
    if(_audioClient->SetEventHandle(_eventHandle) != S_OK) return false;
    if(_audioClient->GetService(IID_IAudioRenderClient, (void**)&_renderClient) != S_OK) return false;
    if(_audioClient->GetBufferSize(&_bufferSize) != S_OK) return false;

    _channels = _waveFormat->nChannels;
    _frequency = _waveFormat->nSamplesPerSec;
    _mode = ((WAVEFORMATEXTENSIBLE*)_waveFormat)->SubFormat.Data1;
    _precision = _waveFormat->wBitsPerSample;

    clear();
    return _ready = true;
  }

  auto terminate() -> void {
    if(_audioClient) _audioClient->Stop();
    if(_renderClient) _renderClient->Release(), _renderClient = nullptr;
    if(_waveFormat) CoTaskMemFree(_waveFormat), _waveFormat = nullptr;
    if(_audioClient) _audioClient->Release(), _audioClient = nullptr;
    if(_audioDevice) _audioDevice->Release(), _audioDevice = nullptr;
    if(_eventHandle) CloseHandle(_eventHandle), _eventHandle = nullptr;
    if(_taskHandle) AvRevertMmThreadCharacteristics(_taskHandle), _taskHandle = nullptr;
  }

  auto write() -> void {
    uint32_t padding = 0;
    _audioClient->GetCurrentPadding(&padding);
    uint32_t available = _bufferSize - padding;

    uint8_t* buffer = nullptr;
    if(_renderClient->GetBuffer(available, &buffer) == S_OK) {
      uint bufferFlags = 0;
      for(uint _ : range(available)) {
        //if more samples are available than we have queued, fill remainder with silence
        double samples[8] = {};
        if(_queue.count) {
          for(uint n : range(_channels)) {
            samples[n] = _queue.samples[_queue.read][n];
          }
          _queue.read++;
          _queue.count--;
        }

        if(_mode == 1 && _precision == 16) {
          auto output = (int16_t*)buffer;
          for(uint n : range(_channels)) *output++ = int16_t(samples[n] * 32768.0);
          buffer = (uint8_t*)output;
        } else if(_mode == 1 && _precision == 32) {
          auto output = (int32_t*)buffer;
          for(uint n : range(_channels)) *output++ = int32_t(samples[n] * 65536.0 * 32768.0);
          buffer = (uint8_t*)output;
        } else if(_mode == 3 && _precision == 32) {
          auto output = (float*)buffer;
          for(uint n : range(_channels)) *output++ = float(samples[n]);
          buffer = (uint8_t*)output;
        } else {
          //output silence for unsupported sample formats
          bufferFlags = AUDCLNT_BUFFERFLAGS_SILENT;
          break;
        }
      }
      _renderClient->ReleaseBuffer(available, bufferFlags);
    }
  }

  bool _ready = false;
  bool _exclusive = false;
  bool _blocking = true;
  uint _channels = 2;
  uint _frequency = 48000;
  uint _latency = 20;

  uint _mode = 0;
  uint _precision = 0;

  struct Queue {
    double samples[65536][8];
    uint16_t read;
    uint16_t write;
    uint16_t count;
  } _queue;

  IMMDeviceEnumerator* _enumerator = nullptr;
  IMMDevice* _audioDevice = nullptr;
  IPropertyStore* _propertyStore = nullptr;
  IAudioClient* _audioClient = nullptr;
  IAudioRenderClient* _renderClient = nullptr;
  WAVEFORMATEX* _waveFormat = nullptr;
  PROPVARIANT _propVariant;
  HANDLE _eventHandle = nullptr;
  HANDLE _taskHandle = nullptr;
  REFERENCE_TIME _devicePeriod = 0;
  uint32_t _bufferSize = 0;  //in frames
};
