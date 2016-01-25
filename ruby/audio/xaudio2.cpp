#include "xaudio2.hpp"
#include <windows.h>

struct AudioXAudio2 : Audio, public IXAudio2VoiceCallback {
  ~AudioXAudio2() { term(); }

  IXAudio2* pXAudio2 = nullptr;
  IXAudio2MasteringVoice* pMasterVoice = nullptr;
  IXAudio2SourceVoice* pSourceVoice = nullptr;

  //inherited from IXAudio2VoiceCallback
  STDMETHODIMP_(void) OnBufferStart(void* pBufferContext){}
  STDMETHODIMP_(void) OnLoopEnd(void* pBufferContext){}
  STDMETHODIMP_(void) OnStreamEnd() {}
  STDMETHODIMP_(void) OnVoiceError(void* pBufferContext, HRESULT Error) {}
  STDMETHODIMP_(void) OnVoiceProcessingPassEnd() {}
  STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) {}

  struct {
    unsigned buffers = 0;
    unsigned latency = 0;

    uint32_t* buffer = nullptr;
    unsigned bufferoffset = 0;

    volatile long submitbuffers = 0;
    unsigned writebuffer = 0;
  } device;

  struct {
    bool synchronize = false;
    unsigned frequency = 22050;
    unsigned latency = 120;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Audio::Synchronize) return true;
    if(name == Audio::Frequency) return true;
    if(name == Audio::Latency) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Audio::Synchronize) return settings.synchronize;
    if(name == Audio::Frequency) return settings.frequency;
    if(name == Audio::Latency) return settings.latency;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Audio::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      if(pXAudio2) clear();
      return true;
    }

    if(name == Audio::Frequency && value.is<unsigned>()) {
      settings.frequency = value.get<unsigned>();
      if(pXAudio2) init();
      return true;
    }

    if(name == Audio::Latency && value.is<unsigned>()) {
      settings.latency = value.get<unsigned>();
      if(pXAudio2) init();
      return true;
    }

    return false;
  }

  auto pushbuffer(unsigned bytes, uint32_t* pAudioData) -> void {
    XAUDIO2_BUFFER xa2buffer = {0};
    xa2buffer.AudioBytes = bytes;
    xa2buffer.pAudioData = reinterpret_cast<BYTE*>(pAudioData);
    xa2buffer.pContext = 0;
    InterlockedIncrement(&device.submitbuffers);
    pSourceVoice->SubmitSourceBuffer(&xa2buffer);
  }

  auto sample(uint16_t left, uint16_t right) -> void {
    device.buffer[device.writebuffer * device.latency + device.bufferoffset++] = left + (right << 16);
    if(device.bufferoffset < device.latency) return;
    device.bufferoffset = 0;

    if(device.submitbuffers == device.buffers - 1) {
      if(settings.synchronize == true) {
        //wait until there is at least one other free buffer for the next sample
        while(device.submitbuffers == device.buffers - 1) {
          //Sleep(0);
        }
      } else {  //we need one free buffer for the next sample, so ignore the current contents
        return;
      }
    }

    pushbuffer(device.latency * 4,device.buffer + device.writebuffer * device.latency);

    device.writebuffer = (device.writebuffer + 1) % device.buffers;
  }

  auto clear() -> void {
    if(!pSourceVoice) return;
    pSourceVoice->Stop(0);
    pSourceVoice->FlushSourceBuffers();  //calls OnBufferEnd for all currently submitted buffers

    device.writebuffer = 0;

    device.bufferoffset = 0;
    if(device.buffer) memset(device.buffer, 0, device.latency * device.buffers * 4);

    pSourceVoice->Start(0);
  }

  auto init() -> bool {
    device.buffers = 8;
    device.latency = settings.frequency * settings.latency / device.buffers / 1000.0 + 0.5;
    device.buffer = new uint32_t[device.latency * device.buffers];
    device.bufferoffset = 0;
    device.submitbuffers = 0;

    HRESULT hr;
    if(FAILED(hr = XAudio2Create(&pXAudio2, 0 , XAUDIO2_DEFAULT_PROCESSOR))) {
      return false;
    }

    unsigned deviceCount = 0;
    pXAudio2->GetDeviceCount(&deviceCount);
    if(deviceCount == 0) { term(); return false; }

    unsigned deviceID = 0;
    for(unsigned deviceIndex = 0; deviceIndex < deviceCount; deviceIndex++) {
      XAUDIO2_DEVICE_DETAILS deviceDetails;
      memset(&deviceDetails, 0, sizeof(XAUDIO2_DEVICE_DETAILS));
      pXAudio2->GetDeviceDetails(deviceIndex, &deviceDetails);
      if(deviceDetails.Role & DefaultGameDevice) deviceID = deviceIndex;
    }

    if(FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice, 2, settings.frequency, 0, deviceID, NULL))) {
      return false;
    }

    WAVEFORMATEX wfx;
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = settings.frequency;
    wfx.nBlockAlign = 4;
    wfx.wBitsPerSample = 16;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    if(FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx, XAUDIO2_VOICE_NOSRC, XAUDIO2_DEFAULT_FREQ_RATIO, this, NULL, NULL))) {
      return false;
    }

    clear();
    return true;
  }

  auto term() -> void {
    if(pSourceVoice) {
      pSourceVoice->Stop(0);
      pSourceVoice->DestroyVoice();
      pSourceVoice = nullptr;
    }
    if(pMasterVoice) {
      pMasterVoice->DestroyVoice();
      pMasterVoice = nullptr;
    }
    if(pXAudio2) {
      pXAudio2->Release();
      pXAudio2 = nullptr;
    }
    if(device.buffer) {
      delete[] device.buffer;
      device.buffer = nullptr;
    }
  }

  STDMETHODIMP_(void) OnBufferEnd(void* pBufferContext) {
    InterlockedDecrement(&device.submitbuffers);
  }
};
