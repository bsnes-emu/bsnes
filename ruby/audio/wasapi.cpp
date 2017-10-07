#include <avrt.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>

struct AudioWASAPI : Audio {
  AudioWASAPI() { initialize(); }
  ~AudioWASAPI() { terminate(); }

  auto availableDevices() -> string_vector {
    return _devices;
  }

  auto availableFrequencies() -> vector<double> {
    return {(double)_frequency};
  }

  auto availableLatencies() -> vector<uint> {
    return {0, 20, 40, 60, 80, 100};
  }

  auto availableChannels() -> vector<uint> {
    return {2};
  }

  auto ready() -> bool { return _ready; }
  auto exclusive() -> bool { return _exclusive; }
  auto device() -> string { return _device; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return (double)_frequency; }
  auto latency() -> uint { return _latency; }

  auto setExclusive(bool exclusive) -> bool {
    if(_exclusive == exclusive) return true;
    _exclusive = exclusive;
    return initialize();
  }

  auto setDevice(string device) -> bool {
    if(_device == device) return true;
    _device = device;
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
    if(!ready()) return;
    _queue.read = 0;
    _queue.write = 0;
    _queue.count = 0;
    _audioClient->Stop();
    _audioClient->Reset();
    _audioClient->Start();
  }

  auto output(const double samples[]) -> void {
    if(!ready()) return;

    for(uint n : range(_channels)) {
      _queue.samples[_queue.write][n] = samples[n];
    }
    _queue.write++;
    _queue.count++;

    if(_queue.count >= _bufferSize) {
      if(WaitForSingleObject(_eventHandle, _blocking ? INFINITE : 0) == WAIT_OBJECT_0) {
        write();
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&_enumerator) != S_OK) return false;

    //enumerate all audio endpoint devices, and select the first to match the device() name
    IMMDeviceCollection* deviceCollection = nullptr;
    if(_enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection) != S_OK) return false;
    uint deviceCount = 0;
    if(deviceCollection->GetCount(&deviceCount) != S_OK) return false;
    for(uint deviceIndex : range(deviceCount)) {
      IMMDevice* device = nullptr;
      if(deviceCollection->Item(deviceIndex, &device) != S_OK) return false;
      IPropertyStore* propertyStore = nullptr;
      device->OpenPropertyStore(STGM_READ, &propertyStore);
      PROPVARIANT propVariant;
      propertyStore->GetValue(PKEY_Device_FriendlyName, &propVariant);
      _devices.append((const char*)utf8_t(propVariant.pwszVal));
      propertyStore->Release();
      if(!_audioDevice && _devices.right() == _device) {
        _audioDevice = device;
      } else {
        device->Release();
      }
    }
    deviceCollection->Release();

    //if no match is found, choose the default audio endpoint for the device()
    if(!_audioDevice) {
      if(_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_audioDevice) != S_OK) return false;
    }

    if(_audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_audioClient) != S_OK) return false;

    WAVEFORMATEXTENSIBLE waveFormat = {};
    if(_exclusive) {
      IPropertyStore* propertyStore = nullptr;
      if(_audioDevice->OpenPropertyStore(STGM_READ, &propertyStore) != S_OK) return false;
      PROPVARIANT propVariant;
      if(propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &propVariant) != S_OK) return false;
      waveFormat = *(WAVEFORMATEXTENSIBLE*)propVariant.blob.pBlobData;
      propertyStore->Release();
      if(_audioClient->GetDevicePeriod(nullptr, &_devicePeriod) != S_OK) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      auto result = _audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, &waveFormat.Format, nullptr);
      if(result == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
        if(_audioClient->GetBufferSize(&_bufferSize) != S_OK) return false;
        _audioClient->Release();
        latency = (REFERENCE_TIME)(10'000 * 1'000 * _bufferSize / waveFormat.Format.nSamplesPerSec);
        if(_audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&_audioClient) != S_OK) return false;
        result = _audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, &waveFormat.Format, nullptr);
      }
      if(result != S_OK) return false;
      DWORD taskIndex = 0;
      _taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      WAVEFORMATEX* waveFormatEx = nullptr;
      if(_audioClient->GetMixFormat(&waveFormatEx) != S_OK) return false;
      waveFormat = *(WAVEFORMATEXTENSIBLE*)waveFormatEx;
      CoTaskMemFree(waveFormatEx);
      if(_audioClient->GetDevicePeriod(&_devicePeriod, nullptr)) return false;
      auto latency = max(_devicePeriod, (REFERENCE_TIME)_latency * 10'000);  //1ms to 100ns units
      if(_audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, 0, &waveFormat.Format, nullptr) != S_OK) return false;
    }

    _eventHandle = CreateEvent(nullptr, false, false, nullptr);
    if(_audioClient->SetEventHandle(_eventHandle) != S_OK) return false;
    if(_audioClient->GetService(IID_IAudioRenderClient, (void**)&_renderClient) != S_OK) return false;
    if(_audioClient->GetBufferSize(&_bufferSize) != S_OK) return false;

    _channels = waveFormat.Format.nChannels;
    _frequency = waveFormat.Format.nSamplesPerSec;
    _mode = waveFormat.SubFormat.Data1;
    _precision = waveFormat.Format.wBitsPerSample;

    _ready = true;
    clear();
    return true;
  }

  auto terminate() -> void {
    _ready = false;
    _devices.reset();
    if(_audioClient) _audioClient->Stop();
    if(_renderClient) _renderClient->Release(), _renderClient = nullptr;
    if(_audioClient) _audioClient->Release(), _audioClient = nullptr;
    if(_audioDevice) _audioDevice->Release(), _audioDevice = nullptr;
    if(_eventHandle) CloseHandle(_eventHandle), _eventHandle = nullptr;
    if(_taskHandle) AvRevertMmThreadCharacteristics(_taskHandle), _taskHandle = nullptr;
  }

  auto write() -> void {
    uint32_t available = _bufferSize;
    if(!_exclusive) {
      uint32_t padding = 0;
      _audioClient->GetCurrentPadding(&padding);
      available = _bufferSize - padding;
    }
    uint32_t length = min(available, _queue.count);

    uint8_t* buffer = nullptr;
    if(_renderClient->GetBuffer(length, &buffer) == S_OK) {
      uint bufferFlags = 0;
      for(uint _ : range(length)) {
        double samples[8] = {};
        if(_queue.count) {
          for(uint n : range(_channels)) {
            samples[n] = _queue.samples[_queue.read][n];
          }
          _queue.read++;
          _queue.count--;
        }

        if(_mode == 1 && _precision == 16) {
          auto output = (uint16_t*)buffer;
          for(uint n : range(_channels)) *output++ = (uint16_t)sclamp<16>(samples[n] * (32768.0 - 1.0));
          buffer = (uint8_t*)output;
        } else if(_mode == 1 && _precision == 32) {
          auto output = (uint32_t*)buffer;
          for(uint n : range(_channels)) *output++ = (uint32_t)sclamp<32>(samples[n] * (65536.0 * 32768.0 - 1.0));
          buffer = (uint8_t*)output;
        } else if(_mode == 3 && _precision == 32) {
          auto output = (float*)buffer;
          for(uint n : range(_channels)) *output++ = float(max(-1.0, min(+1.0, samples[n])));
          buffer = (uint8_t*)output;
        } else {
          //output silence for unsupported sample formats
          bufferFlags = AUDCLNT_BUFFERFLAGS_SILENT;
          break;
        }
      }
      _renderClient->ReleaseBuffer(length, bufferFlags);
    }
  }

  bool _ready = false;
  bool _exclusive = false;
  string _device;
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
  string_vector _devices;
  IMMDevice* _audioDevice = nullptr;
  IAudioClient* _audioClient = nullptr;
  IAudioRenderClient* _renderClient = nullptr;
  HANDLE _eventHandle = nullptr;
  HANDLE _taskHandle = nullptr;
  REFERENCE_TIME _devicePeriod = 0;
  uint32_t _bufferSize = 0;  //in frames
};
