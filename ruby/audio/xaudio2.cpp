#include "xaudio2.hpp"
#include <windows.h>
#undef interface

struct AudioXAudio2 : AudioDriver, public IXAudio2VoiceCallback {
  AudioXAudio2& self = *this;
  AudioXAudio2(Audio& super) : AudioDriver(super) {}
  ~AudioXAudio2() { terminate(); }

  auto create() -> bool override {
    super.setChannels(2);
    super.setFrequency(48000);
    super.setLatency(40);
    return initialize();
  }

  auto driver() -> string override { return "XAudio2"; }
  auto ready() -> bool override { return self.isReady; }

  auto hasBlocking() -> bool override { return true; }

  auto hasFrequencies() -> vector<uint> override {
    return {44100, 48000, 96000};
  }

  auto hasLatencies() -> vector<uint> override {
    return {20, 40, 60, 80, 100};
  }

  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setFrequency(uint frequency) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return initialize(); }

  auto clear() -> void override {
    if(!self.sourceVoice) return;
    self.sourceVoice->Stop(0);
    self.sourceVoice->FlushSourceBuffers();  //calls OnBufferEnd for all currently submitted buffers

    self.bufferIndex = 0;

    self.bufferOffset = 0;
    if(self.buffer) memory::fill<uint32_t>(self.buffer, self.period * self.bufferCount);

    self.sourceVoice->Start(0);
  }

  auto output(const double samples[]) -> void override {
    self.buffer[self.bufferIndex * self.period + self.bufferOffset]  = (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    self.buffer[self.bufferIndex * self.period + self.bufferOffset] |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;
    if(++self.bufferOffset < self.period) return;
    self.bufferOffset = 0;

    if(self.bufferQueue == self.bufferCount - 1) {
      if(self.blocking) {
        //wait until there is at least one other free buffer for the next sample
        while(self.bufferQueue == self.bufferCount - 1);
      } else {  //we need one free buffer for the next sample, so ignore the current contents
        return;
      }
    }

    pushBuffer(self.period * 4, self.buffer + self.bufferIndex * self.period);
    self.bufferIndex = (self.bufferIndex + 1) % self.bufferCount;
  }

private:
  auto initialize() -> bool {
    terminate();

    self.bufferCount = 8;
    self.period = self.frequency * self.latency / self.bufferCount / 1000.0 + 0.5;
    self.buffer = new uint32_t[self.period * self.bufferCount];
    self.bufferOffset = 0;
    self.bufferIndex = 0;
    self.bufferQueue = 0;

    if(FAILED(XAudio2Create(&self.interface, 0 , XAUDIO2_DEFAULT_PROCESSOR))) return false;

    uint deviceCount = 0;
    self.interface->GetDeviceCount(&deviceCount);
    if(deviceCount == 0) return terminate(), false;

    uint deviceID = 0;
    for(uint deviceIndex : range(deviceCount)) {
      XAUDIO2_DEVICE_DETAILS deviceDetails = {};
      self.interface->GetDeviceDetails(deviceIndex, &deviceDetails);
      if(deviceDetails.Role & DefaultGameDevice) deviceID = deviceIndex;
    }

    if(FAILED(self.interface->CreateMasteringVoice(&self.masterVoice, self.channels, self.frequency, 0, deviceID, nullptr))) return terminate(), false;

    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = self.channels;
    waveFormat.nSamplesPerSec = self.frequency;
    waveFormat.nBlockAlign = 4;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    if(FAILED(self.interface->CreateSourceVoice(&self.sourceVoice, (WAVEFORMATEX*)&waveFormat, XAUDIO2_VOICE_NOSRC, XAUDIO2_DEFAULT_FREQ_RATIO, this, nullptr, nullptr))) return terminate(), false;

    clear();
    return self.isReady = true;
  }

  auto terminate() -> void {
    self.isReady = false;

    if(self.sourceVoice) {
      self.sourceVoice->Stop(0);
      self.sourceVoice->DestroyVoice();
      self.sourceVoice = nullptr;
    }

    if(self.masterVoice) {
      self.masterVoice->DestroyVoice();
      self.masterVoice = nullptr;
    }

    if(self.interface) {
      self.interface->Release();
      self.interface = nullptr;
    }

    delete[] self.buffer;
    self.buffer = nullptr;
  }

  auto pushBuffer(uint bytes, uint32_t* audioData) -> void {
    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = bytes;
    buffer.pAudioData = (BYTE*)audioData;
    buffer.pContext = 0;
    InterlockedIncrement(&self.bufferQueue);
    self.sourceVoice->SubmitSourceBuffer(&buffer);
  }

  bool isReady = false;

  uint32_t* buffer = nullptr;
  uint period = 0;
  uint bufferCount = 0;
  uint bufferOffset = 0;
  uint bufferIndex = 0;
  volatile long bufferQueue = 0;  //how many buffers are queued and ready for playback

  IXAudio2* interface = nullptr;
  IXAudio2MasteringVoice* masterVoice = nullptr;
  IXAudio2SourceVoice* sourceVoice = nullptr;

  //inherited from IXAudio2VoiceCallback
  STDMETHODIMP_(void) OnBufferStart(void* pBufferContext){}
  STDMETHODIMP_(void) OnLoopEnd(void* pBufferContext){}
  STDMETHODIMP_(void) OnStreamEnd() {}
  STDMETHODIMP_(void) OnVoiceError(void* pBufferContext, HRESULT Error) {}
  STDMETHODIMP_(void) OnVoiceProcessingPassEnd() {}
  STDMETHODIMP_(void) OnVoiceProcessingPassStart(UINT32 BytesRequired) {}

  STDMETHODIMP_(void) OnBufferEnd(void* pBufferContext) {
    InterlockedDecrement(&self.bufferQueue);
  }
};
