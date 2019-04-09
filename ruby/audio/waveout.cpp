#include <mmsystem.h>

auto CALLBACK waveOutCallback(HWAVEOUT handle, UINT message, DWORD_PTR userData, DWORD_PTR, DWORD_PTR) -> void;

struct AudioWaveOut : AudioDriver {
  AudioWaveOut& self = *this;
  AudioWaveOut(Audio& super) : AudioDriver(super) {}
  ~AudioWaveOut() { terminate(); }

  auto create() -> bool override {
    super.setChannels(2);
    super.setFrequency(44100);
    super.setLatency(0);
    return initialize();
  }

  auto driver() -> string override { return "waveOut"; }
  auto ready() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    vector<string> devices{"Default"};
    for(uint index : range(waveOutGetNumDevs())) {
      WAVEOUTCAPS caps{};
      if(waveOutGetDevCaps(index, &caps, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR) {
        devices.append((const char*)utf8_t(caps.szPname));
      }
    }
    return devices;
  }

  auto hasBlocking() -> bool override { return true; }
  auto hasDynamic() -> bool override { return true; }
  auto hasFrequencies() -> vector<uint> override { return {44100}; }
  auto hasLatencies() -> vector<uint> override { return {0}; }

  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setDynamic(bool dynamic) -> bool override { initialize(); return true; }

  auto clear() -> void override {
    for(auto& header : headers) {
      memory::fill(header.lpData, frameCount * 4);
    }
  }

  auto level() -> double override {
    return (double)((blockQueue * frameCount) + frameIndex) / (blockCount * frameCount);
  }

  auto output(const double samples[]) -> void override {
    uint16_t lsample = sclamp<16>(samples[0] * 32767.0);
    uint16_t rsample = sclamp<16>(samples[1] * 32767.0);

    auto block = (uint32_t*)headers[blockIndex].lpData;
    block[frameIndex] = lsample << 0 | rsample << 16;

    if(++frameIndex >= frameCount) {
      frameIndex = 0;
      if(self.dynamic) {
        while(waveOutWrite(handle, &headers[blockIndex], sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
        InterlockedIncrement(&blockQueue);
      } else while(true) {
        auto result = waveOutWrite(handle, &headers[blockIndex], sizeof(WAVEHDR));
        if(!self.blocking || result != WAVERR_STILLPLAYING) break;
        InterlockedIncrement(&blockQueue);
      }
      if(++blockIndex >= blockCount) {
        blockIndex = 0;
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();

    auto deviceIndex = hasDevices().find(self.device);
    if(!deviceIndex) deviceIndex = 0;

    WAVEFORMATEX format{};
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 2;
    format.nSamplesPerSec = 44100;
    format.nBlockAlign = 4;
    format.wBitsPerSample = 16;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    format.cbSize = 0;  //not sizeof(WAVEFORMAT); size of extra information after WAVEFORMATEX
    //-1 = default; 0+ = specific device; subtract -1 as hasDevices() includes "Default" entry
    waveOutOpen(&handle, (int)*deviceIndex - 1, &format, (DWORD_PTR)waveOutCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);

    headers.resize(blockCount);
    for(auto& header : headers) {
      memory::fill(&header, sizeof(WAVEHDR));
      header.lpData = (LPSTR)LocalAlloc(LMEM_FIXED, frameCount * 4);
      header.dwBufferLength = frameCount * 4;
      waveOutPrepareHeader(handle, &header, sizeof(WAVEHDR));
    }

    frameIndex = 0;
    blockIndex = 0;
    blockQueue = 0;

    waveOutSetVolume(handle, 0xffff'ffff);  //100% volume (65535 left, 65535 right)
    waveOutRestart(handle);
    return true;
  }

  auto terminate() -> void {
    if(!handle) return;
    waveOutPause(handle);
    waveOutReset(handle);
    for(auto& header : headers) {
      waveOutUnprepareHeader(handle, &header, sizeof(WAVEHDR));
      LocalFree(header.lpData);
    }
    waveOutClose(handle);
    handle = nullptr;
    headers.reset();
  }

  HWAVEOUT handle = nullptr;
  vector<WAVEHDR> headers;
  const uint frameCount = 512;
  const uint blockCount =  32;
  uint frameIndex = 0;
  uint blockIndex = 0;

public:
  LONG blockQueue = 0;
};

auto CALLBACK waveOutCallback(HWAVEOUT handle, UINT message, DWORD_PTR userData, DWORD_PTR, DWORD_PTR) -> void {
  auto instance = (AudioWaveOut*)userData;
  if(instance->blockQueue > 0) InterlockedDecrement(&instance->blockQueue);
}
