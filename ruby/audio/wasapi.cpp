#include <avrt.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>

struct AudioWASAPI : AudioDriver {
  AudioWASAPI& self = *this;
  AudioWASAPI(Audio& super) : AudioDriver(super) { enumerate(); }
  ~AudioWASAPI() { terminate(); }

  auto create() -> bool override {
    IMMDevice* defaultDevice = nullptr;
    if(self.enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice) != S_OK) return false;
    for(auto& device : self.devices) {
      if(device.device != defaultDevice) continue;
      super.setDevice(device.name);
      super.setLatency(40);
      return initialize();
    }
    return false;
  }

  auto driver() -> string override { return "WASAPI"; }
  auto ready() -> bool override { return self.isReady; }

  auto hasExclusive() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    vector<string> devices;
    for(auto& device : self.devices) devices.append(device.name);
    return devices;
  }

  auto hasChannels() -> vector<uint> override {
    return {self.channels};
  }

  auto hasFrequencies() -> vector<uint> override {
    return {self.frequency};
  }

  auto hasLatencies() -> vector<uint> override {
    return {0, 20, 40, 60, 80, 100};
  }

  auto setExclusive(bool exclusive) -> bool override { return initialize(); }
  auto setDevice(string device) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setFrequency(uint frequency) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return initialize(); }

  auto clear() -> void override {
    self.queue.read = 0;
    self.queue.write = 0;
    self.queue.count = 0;
    self.audioClient->Stop();
    self.audioClient->Reset();
    self.audioClient->Start();
  }

  auto output(const double samples[]) -> void override {
    for(uint n : range(self.channels)) {
      self.queue.samples[self.queue.write][n] = samples[n];
    }
    self.queue.write++;
    self.queue.count++;

    if(self.queue.count >= self.bufferSize) {
      if(WaitForSingleObject(self.eventHandle, self.blocking ? INFINITE : 0) == WAIT_OBJECT_0) {
        write();
      }
    }
  }

private:
  auto enumerate() -> bool {
    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&self.enumerator) != S_OK) return false;

    IMMDeviceCollection* deviceCollection = nullptr;
    if(self.enumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection) != S_OK) return false;
    uint deviceCount = 0;
    if(deviceCollection->GetCount(&deviceCount) != S_OK) return false;
    for(uint deviceIndex : range(deviceCount)) {
      IMMDevice* device = nullptr;
      if(deviceCollection->Item(deviceIndex, &device) != S_OK) continue;
      IPropertyStore* propertyStore = nullptr;
      device->OpenPropertyStore(STGM_READ, &propertyStore);
      PROPVARIANT propVariant;
      propertyStore->GetValue(PKEY_Device_FriendlyName, &propVariant);
      Device item;
      item.name = (const char*)utf8_t(propVariant.pwszVal);
      item.device = device;
      self.devices.append(item);
      propertyStore->Release();
    }
    deviceCollection->Release();
    return true;
  }

  auto initialize() -> bool {
    terminate();
    if(auto index = self.devices.find([&](auto& device) { return device.name == self.device; })) {
      self.audioDevice = self.devices[*index].device;
    } else {
      return false;
    }

    if(self.audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&self.audioClient) != S_OK) return false;

    WAVEFORMATEXTENSIBLE waveFormat = {};
    if(self.exclusive) {
      IPropertyStore* propertyStore = nullptr;
      if(self.audioDevice->OpenPropertyStore(STGM_READ, &propertyStore) != S_OK) return false;
      PROPVARIANT propVariant;
      if(propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &propVariant) != S_OK) return false;
      waveFormat = *(WAVEFORMATEXTENSIBLE*)propVariant.blob.pBlobData;
      propertyStore->Release();
      if(self.audioClient->GetDevicePeriod(nullptr, &self.devicePeriod) != S_OK) return false;
      auto latency = max(self.devicePeriod, (REFERENCE_TIME)self.latency * 10'000);  //1ms to 100ns units
      auto result = self.audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, &waveFormat.Format, nullptr);
      if(result == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
        if(self.audioClient->GetBufferSize(&self.bufferSize) != S_OK) return false;
        self.audioClient->Release();
        latency = (REFERENCE_TIME)(10'000 * 1'000 * self.bufferSize / waveFormat.Format.nSamplesPerSec);
        if(self.audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&self.audioClient) != S_OK) return false;
        result = self.audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, latency, &waveFormat.Format, nullptr);
      }
      if(result != S_OK) return false;
      DWORD taskIndex = 0;
      self.taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      WAVEFORMATEX* waveFormatEx = nullptr;
      if(self.audioClient->GetMixFormat(&waveFormatEx) != S_OK) return false;
      waveFormat = *(WAVEFORMATEXTENSIBLE*)waveFormatEx;
      CoTaskMemFree(waveFormatEx);
      if(self.audioClient->GetDevicePeriod(&self.devicePeriod, nullptr)) return false;
      auto latency = max(self.devicePeriod, (REFERENCE_TIME)self.latency * 10'000);  //1ms to 100ns units
      if(self.audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, latency, 0, &waveFormat.Format, nullptr) != S_OK) return false;
    }

    self.eventHandle = CreateEvent(nullptr, false, false, nullptr);
    if(self.audioClient->SetEventHandle(self.eventHandle) != S_OK) return false;
    if(self.audioClient->GetService(IID_IAudioRenderClient, (void**)&self.renderClient) != S_OK) return false;
    if(self.audioClient->GetBufferSize(&self.bufferSize) != S_OK) return false;

    self.channels = waveFormat.Format.nChannels;
    self.frequency = waveFormat.Format.nSamplesPerSec;
    self.mode = waveFormat.SubFormat.Data1;
    self.precision = waveFormat.Format.wBitsPerSample;

    self.isReady = true;
    clear();
    return true;
  }

  auto terminate() -> void {
    self.isReady = false;
    if(self.audioClient) self.audioClient->Stop();
    if(self.renderClient) self.renderClient->Release(), self.renderClient = nullptr;
    if(self.audioClient) self.audioClient->Release(), self.audioClient = nullptr;
    if(self.audioDevice) self.audioDevice->Release(), self.audioDevice = nullptr;
    if(self.eventHandle) CloseHandle(self.eventHandle), self.eventHandle = nullptr;
    if(self.taskHandle) AvRevertMmThreadCharacteristics(self.taskHandle), self.taskHandle = nullptr;
  }

  auto write() -> void {
    uint32_t available = self.bufferSize;
    if(!self.exclusive) {
      uint32_t padding = 0;
      self.audioClient->GetCurrentPadding(&padding);
      available = self.bufferSize - padding;
    }
    uint32_t length = min(available, self.queue.count);

    uint8_t* buffer = nullptr;
    if(self.renderClient->GetBuffer(length, &buffer) == S_OK) {
      uint bufferFlags = 0;
      for(uint _ : range(length)) {
        double samples[8] = {};
        if(self.queue.count) {
          for(uint n : range(self.channels)) {
            samples[n] = self.queue.samples[self.queue.read][n];
          }
          self.queue.read++;
          self.queue.count--;
        }

        if(self.mode == 1 && self.precision == 16) {
          auto output = (uint16_t*)buffer;
          for(uint n : range(self.channels)) *output++ = (uint16_t)sclamp<16>(samples[n] * (32768.0 - 1.0));
          buffer = (uint8_t*)output;
        } else if(self.mode == 1 && self.precision == 32) {
          auto output = (uint32_t*)buffer;
          for(uint n : range(self.channels)) *output++ = (uint32_t)sclamp<32>(samples[n] * (65536.0 * 32768.0 - 1.0));
          buffer = (uint8_t*)output;
        } else if(self.mode == 3 && self.precision == 32) {
          auto output = (float*)buffer;
          for(uint n : range(self.channels)) *output++ = float(max(-1.0, min(+1.0, samples[n])));
          buffer = (uint8_t*)output;
        } else {
          //output silence for unsupported sample formats
          bufferFlags = AUDCLNT_BUFFERFLAGS_SILENT;
          break;
        }
      }
      self.renderClient->ReleaseBuffer(length, bufferFlags);
    }
  }

  bool isReady = false;

  uint mode = 0;
  uint precision = 0;

  struct Device {
    string name;
    IMMDevice* device;
  };

  struct Queue {
    double samples[65536][8];
    uint16_t read;
    uint16_t write;
    uint16_t count;
  } queue;

  IMMDeviceEnumerator* enumerator = nullptr;
  vector<Device> devices;
  IMMDevice* audioDevice = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioRenderClient* renderClient = nullptr;
  HANDLE eventHandle = nullptr;
  HANDLE taskHandle = nullptr;
  REFERENCE_TIME devicePeriod = 0;
  uint32_t bufferSize = 0;  //in frames
};
