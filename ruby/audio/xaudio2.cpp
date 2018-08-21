#include "xaudio2.hpp"
#undef interface

struct AudioXAudio2 : AudioDriver, public IXAudio2VoiceCallback {
  enum : uint { Buffers = 32 };

  AudioXAudio2& self = *this;
  AudioXAudio2(Audio& super) : AudioDriver(super) { construct(); }
  ~AudioXAudio2() { destruct(); }

  auto create() -> bool override {
    super.setDevice(hasDevices().first());
    super.setChannels(2);
    super.setFrequency(48000);
    super.setLatency(40);
    return initialize();
  }

  auto driver() -> string override { return "XAudio 2.1"; }
  auto ready() -> bool override { return self.isReady; }

  auto hasBlocking() -> bool override { return true; }
  auto hasDynamic() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    vector<string> devices;
    for(auto& device : self.devices) devices.append(device.name);
    return devices;
  }

  auto hasFrequencies() -> vector<uint> override {
    return {44100, 48000, 96000};
  }

  auto hasLatencies() -> vector<uint> override {
    return {20, 40, 60, 80, 100};
  }

  auto setDevice(string device) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setFrequency(uint frequency) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return initialize(); }

  auto clear() -> void override {
    self.sourceVoice->Stop(0);
    self.sourceVoice->FlushSourceBuffers();  //calls OnBufferEnd for all currently submitted buffers

    self.index = 0;
    self.queue = 0;
    for(uint n : range(Buffers)) self.buffers[n].fill();

    self.sourceVoice->Start(0);
  }

  auto level() -> double override {
    XAUDIO2_VOICE_STATE state{};
    self.sourceVoice->GetState(&state);
    uint level = state.BuffersQueued * self.period - state.SamplesPlayed % self.period;
    uint limit = Buffers * self.period;
    return (double)(limit - level) / limit;
  }

  auto output(const double samples[]) -> void override {
    uint32_t frame = 0;
    frame |= (uint16_t)sclamp<16>(samples[0] * 32767.0) <<  0;
    frame |= (uint16_t)sclamp<16>(samples[1] * 32767.0) << 16;

    auto& buffer = self.buffers[self.index];
    buffer.write(frame);
    if(!buffer.full()) return;

    buffer.flush();
    if(self.queue == Buffers - 1) {
      if(self.blocking) {
        //wait until there is at least one other free buffer for the next sample
        while(self.queue == Buffers - 1);
      } else {
        //there is no free buffer for the next block, so ignore the current contents
        return;
      }
    }

    write(buffer.data(), buffer.capacity<uint8_t>());
    self.index = (self.index + 1) % Buffers;
  }

private:
  struct Device {
    uint id = 0;
    uint channels = 0;
    uint frequency = 0;
    Format format = Format::none;
    string name;
  };
  vector<Device> devices;

  auto construct() -> void {
    XAudio2Create(&self.interface, 0 , XAUDIO2_DEFAULT_PROCESSOR);

    uint deviceCount = 0;
    self.interface->GetDeviceCount(&deviceCount);
    for(uint deviceIndex : range(deviceCount)) {
      XAUDIO2_DEVICE_DETAILS deviceDetails{};
      self.interface->GetDeviceDetails(deviceIndex, &deviceDetails);
      auto format = deviceDetails.OutputFormat.Format.wFormatTag;
      auto bits = deviceDetails.OutputFormat.Format.wBitsPerSample;

      Device device;
      device.id = deviceIndex;
      device.name = (const char*)utf8_t(deviceDetails.DisplayName);
      device.channels = deviceDetails.OutputFormat.Format.nChannels;
      device.frequency = deviceDetails.OutputFormat.Format.nSamplesPerSec;
      if(format == WAVE_FORMAT_PCM) {
        if(bits == 16) device.format = Format::int16;
        if(bits == 32) device.format = Format::int32;
      } else if(format == WAVE_FORMAT_IEEE_FLOAT) {
        if(bits == 32) device.format = Format::float32;
      }

      //ensure devices.first() is the default device
      if(deviceDetails.Role & DefaultGameDevice) {
        devices.prepend(device);
      } else {
        devices.append(device);
      }
    }
  }

  auto destruct() -> void {
    terminate();

    if(self.interface) {
      self.interface->Release();
      self.interface = nullptr;
    }
  }

  auto initialize() -> bool {
    terminate();
    if(!self.interface) return false;

    self.period = self.frequency * self.latency / Buffers / 1000.0 + 0.5;
    for(uint n : range(Buffers)) buffers[n].resize(self.period);
    self.index = 0;
    self.queue = 0;

    if(!hasDevices().find(self.device)) self.device = hasDevices().first();
    uint deviceID = devices[hasDevices().find(self.device)()].id;

    if(FAILED(self.interface->CreateMasteringVoice(&self.masterVoice, self.channels, self.frequency, 0, deviceID, nullptr))) return terminate(), false;

    WAVEFORMATEX waveFormat{};
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
  }

  auto write(const uint32_t* audioData, uint bytes) -> void {
    XAUDIO2_BUFFER buffer{};
    buffer.AudioBytes = bytes;
    buffer.pAudioData = (const BYTE*)audioData;
    buffer.pContext = nullptr;
    InterlockedIncrement(&self.queue);
    self.sourceVoice->SubmitSourceBuffer(&buffer);
  }

  bool isReady = false;

  queue<uint32_t> buffers[Buffers];
  uint period = 0;          //amount (in 32-bit frames) of samples per buffer
  uint index = 0;           //current buffer for writing samples to
  volatile long queue = 0;  //how many buffers are queued and ready for playback

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
    InterlockedDecrement(&self.queue);
  }
};
