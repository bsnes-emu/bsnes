#include <nall/windows/registry.hpp>
#include "asio.hpp"

struct AudioASIO : AudioDriver {
  static AudioASIO* instance;
  AudioASIO& self = *this;
  AudioASIO(Audio& super) : AudioDriver(super) { instance = this; }
  ~AudioASIO() { terminate(); }

  auto create() -> bool override {
    super.setDevice(hasDevices().first());
    super.setChannels(2);
    super.setFrequency(48000);
    super.setLatency(2048);
    return initialize();
  }

  auto driver() -> string override { return "ASIO"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }

  auto hasDevices() -> vector<string> override {
    self.devices.reset();
    for(auto candidate : registry::contents("HKLM\\SOFTWARE\\ASIO\\")) {
      if(auto classID = registry::read({"HKLM\\SOFTWARE\\ASIO\\", candidate, "CLSID"})) {
        self.devices.append({candidate.trimRight("\\", 1L), classID});
      }
    }

    vector<string> devices;
    for(auto& device : self.devices) devices.append(device.name);
    return devices;
  }

  auto hasChannels() -> vector<uint> override {
    return {1, 2};
  }

  auto hasFrequencies() -> vector<uint> override {
    return {self.frequency};
  }

  auto hasLatencies() -> vector<uint> override {
    vector<uint> latencies;
    uint latencyList[] = {64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048, 3072, 6144};  //factors of 6144
    for(auto& latency : latencyList) {
      if(self.activeDevice) {
        if(latency < self.activeDevice.minimumBufferSize) continue;
        if(latency > self.activeDevice.maximumBufferSize) continue;
      }
      latencies.append(latency);
    }
    return latencies;
  }

  auto setContext(uintptr context) -> bool override { return initialize(); }
  auto setDevice(string device) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return initialize(); }
  auto setChannels(uint channels) -> bool override { return initialize(); }
  auto setLatency(uint latency) -> bool override { return initialize(); }

  auto clear() -> void override {
    if(!ready()) return;
    for(uint n : range(self.channels)) {
      memory::fill<uint8_t>(_channel[n].buffers[0], self.latency * _sampleSize);
      memory::fill<uint8_t>(_channel[n].buffers[1], self.latency * _sampleSize);
    }
    memory::fill<uint8_t>(_queue.samples, sizeof(_queue.samples));
    _queue.read = 0;
    _queue.write = 0;
    _queue.count = 0;
  }

  auto output(const double samples[]) -> void override {
    if(!ready()) return;
    if(self.blocking) {
      while(_queue.count >= self.latency);
    }
    for(uint n : range(self.channels)) {
      _queue.samples[_queue.write][n] = samples[n];
    }
    _queue.write++;
    _queue.count++;
  }

private:
  auto initialize() -> bool {
    terminate();

    hasDevices();  //this call populates self.devices
    if(!self.devices) return false;

    self.activeDevice = {};
    for(auto& device : self.devices) {
      if(self.device == device.name) {
        self.activeDevice = device;
        break;
      }
    }
    if(!self.activeDevice) {
      self.activeDevice = self.devices.first();
      self.device = self.activeDevice.name;
    }

    CLSID classID;
    if(CLSIDFromString((LPOLESTR)utf16_t(self.activeDevice.classID), (LPCLSID)&classID) != S_OK) return false;
    if(CoCreateInstance(classID, 0, CLSCTX_INPROC_SERVER, classID, (void**)&_asio) != S_OK) return false;

    if(!_asio->init((void*)self.context)) return false;
    if(_asio->getSampleRate(&self.activeDevice.sampleRate) != ASE_OK) return false;
    if(_asio->getChannels(&self.activeDevice.inputChannels, &self.activeDevice.outputChannels) != ASE_OK) return false;
    if(_asio->getBufferSize(
      &self.activeDevice.minimumBufferSize,
      &self.activeDevice.maximumBufferSize,
      &self.activeDevice.preferredBufferSize,
      &self.activeDevice.granularity
    ) != ASE_OK) return false;

    self.frequency = self.activeDevice.sampleRate;
    self.latency = self.latency < self.activeDevice.minimumBufferSize ? self.activeDevice.minimumBufferSize : self.latency;
    self.latency = self.latency > self.activeDevice.maximumBufferSize ? self.activeDevice.maximumBufferSize : self.latency;

    for(uint n : range(self.channels)) {
      _channel[n].isInput = false;
      _channel[n].channelNum = n;
      _channel[n].buffers[0] = nullptr;
      _channel[n].buffers[1] = nullptr;
    }
    ASIOCallbacks callbacks;
    callbacks.bufferSwitch = &AudioASIO::_bufferSwitch;
    callbacks.sampleRateDidChange = &AudioASIO::_sampleRateDidChange;
    callbacks.asioMessage = &AudioASIO::_asioMessage;
    callbacks.bufferSwitchTimeInfo = &AudioASIO::_bufferSwitchTimeInfo;
    if(_asio->createBuffers(_channel, self.channels, self.latency, &callbacks) != ASE_OK) return false;
    if(_asio->getLatencies(&self.activeDevice.inputLatency, &self.activeDevice.outputLatency) != ASE_OK) return false;

    //assume for the sake of sanity that all buffers use the same sample format ...
    ASIOChannelInfo channelInformation = {};
    channelInformation.channel = 0;
    channelInformation.isInput = false;
    if(_asio->getChannelInfo(&channelInformation) != ASE_OK) return false;
    switch(_sampleFormat = channelInformation.type) {
    case ASIOSTInt16LSB: _sampleSize = 2; break;
    case ASIOSTInt24LSB: _sampleSize = 3; break;
    case ASIOSTInt32LSB: _sampleSize = 4; break;
    case ASIOSTFloat32LSB: _sampleSize = 4; break;
    case ASIOSTFloat64LSB: _sampleSize = 8; break;
    default: return false;  //unsupported sample format
    }

    _ready = true;
    clear();
    if(_asio->start() != ASE_OK) return _ready = false;
    return true;
  }

  auto terminate() -> void {
    _ready = false;
    self.activeDevice = {};
    if(_asio) {
      _asio->stop();
      _asio->disposeBuffers();
      _asio->Release();
      _asio = nullptr;
    }
  }

private:
  static auto _bufferSwitch(long doubleBufferInput, ASIOBool directProcess) -> void {
    return instance->bufferSwitch(doubleBufferInput, directProcess);
  }

  static auto _sampleRateDidChange(ASIOSampleRate sampleRate) -> void {
    return instance->sampleRateDidChange(sampleRate);
  }

  static auto _asioMessage(long selector, long value, void* message, double* optional) -> long {
    return instance->asioMessage(selector, value, message, optional);
  }

  static auto _bufferSwitchTimeInfo(ASIOTime* parameters, long doubleBufferIndex, ASIOBool directProcess) -> ASIOTime* {
    return instance->bufferSwitchTimeInfo(parameters, doubleBufferIndex, directProcess);
  }

  auto bufferSwitch(long doubleBufferInput, ASIOBool directProcess) -> void {
    for(uint sampleIndex : range(self.latency)) {
      double samples[8] = {0};
      if(_queue.count) {
        for(uint n : range(self.channels)) {
          samples[n] = _queue.samples[_queue.read][n];
        }
        _queue.read++;
        _queue.count--;
      }

      for(uint n : range(self.channels)) {
        auto buffer = (uint8_t*)_channel[n].buffers[doubleBufferInput];
        buffer += sampleIndex * _sampleSize;

        switch(_sampleFormat) {
        case ASIOSTInt16LSB: {
          *(uint16_t*)buffer = (uint16_t)sclamp<16>(samples[n] * (32768.0 - 1.0));
          break;
        }

        case ASIOSTInt24LSB: {
          auto value = (uint32_t)sclamp<24>(samples[n] * (256.0 * 32768.0 - 1.0));
          buffer[0] = value >>  0;
          buffer[1] = value >>  8;
          buffer[2] = value >> 16;
          break;
        }

        case ASIOSTInt32LSB: {
          *(uint32_t*)buffer = (uint32_t)sclamp<32>(samples[n] * (65536.0 * 32768.0 - 1.0));
          break;
        }

        case ASIOSTFloat32LSB: {
          *(float*)buffer = max(-1.0, min(+1.0, samples[n]));
          break;
        }

        case ASIOSTFloat64LSB: {
          *(double*)buffer = max(-1.0, min(+1.0, samples[n]));
          break;
        }
        }
      }
    }
  }

  auto sampleRateDidChange(ASIOSampleRate sampleRate) -> void {
  }

  auto asioMessage(long selector, long value, void* message, double* optional) -> long {
    return ASE_OK;
  }

  auto bufferSwitchTimeInfo(ASIOTime* parameters, long doubleBufferIndex, ASIOBool directProcess) -> ASIOTime* {
    return nullptr;
  }

  bool _ready = false;

  struct Queue {
    double samples[65536][8];
    uint16_t read;
    uint16_t write;
    std::atomic<uint16_t> count;
  };

  struct Device {
    explicit operator bool() const { return name; }

    string name;
    string classID;

    ASIOSampleRate sampleRate;
    long inputChannels;
    long outputChannels;
    long inputLatency;
    long outputLatency;
    long minimumBufferSize;
    long maximumBufferSize;
    long preferredBufferSize;
    long granularity;
  };

  Queue _queue;
  vector<Device> devices;
  Device activeDevice;
  IASIO* _asio = nullptr;
  ASIOBufferInfo _channel[8];
  long _sampleFormat;
  long _sampleSize;
};

AudioASIO* AudioASIO::instance = nullptr;
