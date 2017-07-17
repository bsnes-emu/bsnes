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
    information.frequencies = {};
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
    _audioClient->Stop();
    _audioClient->Reset();
    for(auto n : range(available())) write(0, 0);
    _audioClient->Start();
  }

  auto output(const double samples[]) -> void {
    _queuedFrames.append(int16_t(samples[0] * 32768.0) << 0 | int16_t(samples[1] * 32768.0) << 16);

    if(!available() && _queuedFrames.size() >= _bufferSize) {
      if(_blocking) {
        while(!available());  //wait for free sample slot
      } else {
        _queuedFrames.takeLeft();  //drop sample (run ahead)
      }
    }

    uint32_t cachedFrame = 0;
    for(auto n : range(available())) {
      if(_queuedFrames) cachedFrame = _queuedFrames.takeLeft();
      write(cachedFrame >> 0, cachedFrame >> 16);
    }
  }

private:
  auto initialize() -> bool {
    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&_enumerator) != S_OK) return false;
    if(_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_audioDevice) != S_OK) return false;
    if(_audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_audioClient) != S_OK) return false;

    if(_exclusive) {
      if(_audioDevice->OpenPropertyStore(STGM_READ, &_propertyStore) != S_OK) return false;
      if(_propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &_propVariant) != S_OK) return false;
      _waveFormat = (WAVEFORMATEX*)_propVariant.blob.pBlobData;
      if(_audioClient->GetDevicePeriod(nullptr, &_devicePeriod) != S_OK) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      if(_audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, 0, latency, latency, _waveFormat, nullptr) != S_OK) return false;
      DWORD taskIndex = 0;
      _taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      if(_audioClient->GetMixFormat(&waveFormat) != S_OK) return false;
      if(_audioClient->GetDevicePeriod(&_devicePeriod, nullptr)) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      if(_audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, latency, 0, _waveFormat, nullptr) != S_OK) return false;
    }

    if(_audioClient->GetService(IID_IAudioRenderClient, (void**)&_renderClient) != S_OK) return false;
    if(_audioClient->GetBufferSize(&_bufferSize) != S_OK) return false;

    _channels = waveFormat->nChannels;
    _frequency = waveFormat->nSamplesPerSec;
    _mode = ((WAVEFORMATEXTENSIBLE*)_waveFormat)->SubFormat.Data1;
    _precision = _waveFormat->wBitsPerSample;

    _audioClient->Start();
    return _ready = true;
  }

  auto terminate() -> void {
    if(_audioClient) _audioClient->Stop();
    if(_renderClient) _renderClient->Release(), _renderClient = nullptr;
    if(_waveFormat) CoTaskMemFree(_waveFormat), _waveFormat = nullptr;
    if(_audioClient) _audioClient->Release(), _audioClient = nullptr;
    if(_audioDevice) _audioDevice->Release(), _audioDevice = nullptr;
    if(_taskHandle) AvRevertMmThreadCharacteristics(_taskHandle), _taskHandle = nullptr;
  }

  auto available() -> uint {
    uint32_t padding = 0;
    _audioClient->GetCurrentPadding(&padding);
    return bufferSize - padding;
  }

  auto write(int16_t left, int16_t right) -> void {
    if(_renderClient->GetBuffer(1, &_bufferData) != S_OK) return;

    if(_channels >= 2 && _mode == 1 && _precision == 16) {
      auto buffer = (int16_t*)_bufferData;
      buffer[0] = left;
      buffer[1] = right;
    }

    if(_channels >= 2 && _mode == 3 && _precision == 32) {
      auto buffer = (float*)_bufferData;
      buffer[0] = left / 32768.0;
      buffer[1] = right / 32768.0;
    }

    _renderClient->ReleaseBuffer(1, 0);
  }

  bool _exclusive = false;
  bool _blocking = true;
  uint _channels = 2;
  uint _frequency = 48000;
  uint _latency = 20;

  uint _mode = 0;
  uint _precision = 0;

  IMMDeviceEnumerator* _enumerator = nullptr;
  IMMDevice* _audioDevice = nullptr;
  IPropertyStore* _propertyStore = nullptr;
  IAudioClient* _audioClient = nullptr;
  IAudioRenderClient* _renderClient = nullptr;
  WAVEFORMATEX* _waveFormat = nullptr;
  PROPVARIANT _propVariant;
  HANDLE _taskHandle = nullptr;
  REFERENCE_TIME _devicePeriod = 0;
  uint32_t _bufferSize = 0;  //in frames
  uint8_t* _bufferData = nullptr;
  vector<uint32_t> _queuedFrames;
};
