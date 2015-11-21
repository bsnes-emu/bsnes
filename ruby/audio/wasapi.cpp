#include <avrt.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <devicetopology.h>
#include <endpointvolume.h>

#include <nall/dsp.hpp>

struct AudioWASAPI : Audio {
  ~AudioWASAPI() { term(); }

  struct {
    bool exclusive = false;
    bool synchronize = false;
    uint frequency = 44100;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Exclusive) return true;
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Exclusive) return settings.exclusive;
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Exclusive && value.get<bool>()) {
      settings.exclusive = value.get<bool>();
      return true;
    }

    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    if(name == Audio::Frequency && value.is<uint>()) {
      settings.frequency = value.get<uint>();
      dsp.setFrequency(settings.frequency);
      return true;
    }

    return false;
  }

  auto sample(uint16 left, uint16 right) -> void {
    int samples[] = {(int16)left, (int16)right};
    dsp.sample(samples);
    while(dsp.pending()) {
      dsp.read(samples);
      write(samples);
    }
  }

  auto clear() -> void {
    audioClient->Stop();
    renderClient->GetBuffer(bufferFrameCount, &bufferData);

    renderClient->ReleaseBuffer(bufferFrameCount, 0);
    audioClient->Start();
  }

  auto init() -> bool {
    if(CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&enumerator) != S_OK) return false;
    if(enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device) != S_OK) return false;
    if(device->Activate(IID_IAudioClient, CLSCTX_ALL, nullptr, (void**)&audioClient) != S_OK) return false;

    if(settings.exclusive) {
      if(device->OpenPropertyStore(STGM_READ, &propertyStore) != S_OK) return false;
      if(propertyStore->GetValue(PKEY_AudioEngine_DeviceFormat, &propVariant) != S_OK) return false;
      waveFormat = (WAVEFORMATEX*)propVariant.blob.pBlobData;
      if(audioClient->GetDevicePeriod(nullptr, &devicePeriod) != S_OK) return false;
      if(audioClient->Initialize(AUDCLNT_SHAREMODE_EXCLUSIVE, 0, devicePeriod, devicePeriod, waveFormat, nullptr) != S_OK) return false;
      taskHandle = AvSetMmThreadCharacteristics(L"Pro Audio", &taskIndex);
    } else {
      if(audioClient->GetMixFormat(&waveFormat) != S_OK) return false;
      if(audioClient->GetDevicePeriod(&devicePeriod, nullptr)) return false;
      if(audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, devicePeriod, 0, waveFormat, nullptr) != S_OK) return false;
    }

    if(audioClient->GetService(IID_IAudioRenderClient, (void**)&renderClient) != S_OK) return false;
    if(audioClient->GetBufferSize(&bufferFrameCount) != S_OK) return false;

    switch(((WAVEFORMATEXTENSIBLE*)waveFormat)->SubFormat.Data1) {
    case 1: ieee = false; break;  //fixed point
    case 3: ieee = true; break;   //floating point
    default: return false;        //unknown format; abort
    }

    dsp.setChannels(2);
    dsp.setPrecision(16);
    dsp.setFrequency(settings.frequency);

    dsp.setResampler(DSP::ResampleEngine::Linear);
    dsp.setResamplerFrequency(waveFormat->nSamplesPerSec);
    dsp.setChannels(waveFormat->nChannels);
    dsp.setPrecision(waveFormat->wBitsPerSample);

    print("[WASAPI]\n");
    print("Channels:  ", waveFormat->nChannels, "\n");
    print("Precision: ", waveFormat->wBitsPerSample, "\n");
    print("Frequency: ", waveFormat->nSamplesPerSec, "\n");
    print("IEEE-754:  ", ieee, "\n");
    print("Exclusive: ", settings.exclusive, "\n\n");

    audioClient->Start();
    return true;
  }

  auto term() -> void {
    if(audioClient) {
      audioClient->Stop();
    }

    if(taskHandle) {
      AvRevertMmThreadCharacteristics(taskHandle);
      taskHandle = nullptr;
    }
  }

private:
  auto write(int samples[]) -> void {
    while(true) {
      uint32 padding = 0;
      audioClient->GetCurrentPadding(&padding);
      if(bufferFrameCount - padding < 1) {
        if(!settings.synchronize) return;
        continue;
      }
      break;
    }

    renderClient->GetBuffer(1, &bufferData);

    if(ieee) {
      auto buffer = (float*)bufferData;
      buffer[0] = (int16)samples[0] / 32768.0;
      buffer[1] = (int16)samples[1] / 32768.0;
    } else {
      auto buffer = (int16*)bufferData;
      buffer[0] = (int16)samples[0];
      buffer[1] = (int16)samples[1];
    }

    renderClient->ReleaseBuffer(1, 0);
  }

  DSP dsp;
  IMMDeviceEnumerator* enumerator = nullptr;
  IMMDevice* device = nullptr;
  IPropertyStore* propertyStore = nullptr;
  IAudioClient* audioClient = nullptr;
  IAudioRenderClient* renderClient = nullptr;
  WAVEFORMATEX* waveFormat = nullptr;
  PROPVARIANT propVariant;
  HANDLE taskHandle = nullptr;
  DWORD taskIndex = 0;
  REFERENCE_TIME devicePeriod = 0;
  uint32 bufferFrameCount = 0;
  uint8* bufferData = nullptr;
  bool ieee = false;
};
