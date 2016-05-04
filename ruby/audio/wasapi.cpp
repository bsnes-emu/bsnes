#include <avrt.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>

struct AudioWASAPI : Audio {
  ~AudioWASAPI() { term(); }

  struct {
    bool exclusive = false;
    uint latency = 80;
    bool synchronize = true;
  } settings;

  struct {
    uint channels = 0;
    uint frequency = 0;
    uint mode = 0;
    uint precision = 0;
  } device;

  auto cap(const string& name) -> bool {
    if(name == Audio::Exclusive) return true;
    if(name == Audio::Latency) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Exclusive) return settings.exclusive;
    if(name == Audio::Latency) return settings.latency;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return device.frequency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Exclusive && value.get<bool>()) {
      if(audioDevice) term(), init();
      settings.exclusive = value.get<bool>();
      return true;
    }

    if(name == Audio::Latency && value.get<uint>()) {
      if(audioDevice) term(), init();
      settings.latency = value.get<uint>();
      return true;
    }

    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    return false;
  }

  auto sample(int16_t left, int16_t right) -> void {
    queuedFrames.append((uint16_t)left << 0 | (uint16_t)right << 16);

    if(!available() && queuedFrames.size() >= bufferSize) {
      if(settings.synchronize) while(!available());  //wait for free sample slot
      else queuedFrames.takeLeft();                  //drop sample (run ahead)
    }

    uint32_t cachedFrame = 0;
    for(auto n : range(available())) {
      if(queuedFrames) cachedFrame = queuedFrames.takeLeft();
      write(cachedFrame >> 0, cachedFrame >> 16);
    }
  }

  auto clear() -> void {
    audioClient->Stop();
    audioClient->Reset();
    for(auto n : range(available())) write(0, 0);
    audioClient->Start();
  }

  auto init() -> bool {
    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&enumerator) != S_OK) return false;
    if(enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice) != S_OK) return false;
    if(audioDevice->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&audioClient) != S_OK) return false;

    if(settings.exclusive) {
      if(audioDevice->OpenPropertyStore(STGM_READ, &propertyStore) != S_OK) return false;
      if(propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &propVariant) != S_OK) return false;
      waveFormat = (WAVEFORMATEX*)propVariant.blob.pBlobData;
      if(audioClient->GetDevicePeriod(nullptr, &devicePeriod) != S_OK) return false;
      auto latency = max(devicePeriod, (REFERENCE_TIME)settings.latency * 10'000);  //1ms to 100ns units
      if(audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, 0, latency, latency, waveFormat, nullptr) != S_OK) return false;
      DWORD taskIndex = 0;
      taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      if(audioClient->GetMixFormat(&waveFormat) != S_OK) return false;
      if(audioClient->GetDevicePeriod(&devicePeriod, nullptr)) return false;
      auto latency = max(devicePeriod, (REFERENCE_TIME)settings.latency * 10'000);  //1ms to 100ns units
      if(audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, latency, 0, waveFormat, nullptr) != S_OK) return false;
    }

    if(audioClient->GetService(IID_IAudioRenderClient, (void**)&renderClient) != S_OK) return false;
    if(audioClient->GetBufferSize(&bufferSize) != S_OK) return false;

    device.channels = waveFormat->nChannels;
    device.frequency = waveFormat->nSamplesPerSec;
    device.mode = ((WAVEFORMATEXTENSIBLE*)waveFormat)->SubFormat.Data1;
    device.precision = waveFormat->wBitsPerSample;

    audioClient->Start();
    return true;
  }

  auto term() -> void {
    if(audioClient) audioClient->Stop();
    if(renderClient) renderClient->Release(), renderClient = nullptr;
    if(waveFormat) CoTaskMemFree(waveFormat), waveFormat = nullptr;
    if(audioClient) audioClient->Release(), audioClient = nullptr;
    if(audioDevice) audioDevice->Release(), audioDevice = nullptr;
    if(taskHandle) AvRevertMmThreadCharacteristics(taskHandle), taskHandle = nullptr;
  }

private:
  auto available() -> uint {
    uint32_t padding = 0;
    audioClient->GetCurrentPadding(&padding);
    return bufferSize - padding;
  }

  auto write(int16_t left, int16_t right) -> void {
    if(renderClient->GetBuffer(1, &bufferData) != S_OK) return;

    if(device.channels >= 2 && device.mode == 1 && device.precision == 16) {
      auto buffer = (int16_t*)bufferData;
      buffer[0] = left;
      buffer[1] = right;
    }

    if(device.channels >= 2 && device.mode == 3 && device.precision == 32) {
      auto buffer = (float*)bufferData;
      buffer[0] = left  / 32768.0;
      buffer[1] = right / 32768.0;
    }

    renderClient->ReleaseBuffer(1, 0);
  }

  IMMDeviceEnumerator* enumerator = nullptr;
  IMMDevice* audioDevice = nullptr;
  IPropertyStore* propertyStore = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioRenderClient* renderClient = nullptr;
  WAVEFORMATEX* waveFormat = nullptr;
  PROPVARIANT propVariant;
  HANDLE taskHandle = nullptr;
  REFERENCE_TIME devicePeriod = 0;
  uint32_t bufferSize = 0;  //in frames
  uint8_t* bufferData = nullptr;
  vector<uint32_t> queuedFrames;
};
