#include "asio.hpp"

struct AudioASIO : Audio {
  static AudioASIO* self;
  AudioASIO() { self = this; initialize(); }
  ~AudioASIO() { terminate(); }

  auto availableDevices() -> string_vector {
    string_vector devices;
    for(auto& device : _devices) devices.append(device.name);
    return devices;
  }

  auto availableFrequencies() -> vector<double> {
    return {_frequency};
  }

  auto availableLatencies() -> vector<uint> {
    vector<uint> latencies;
    uint latencyList[] = {64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048, 3072, 6144};  //factors of 6144
    for(auto& latency : latencyList) {
      if(latency < _active.minimumBufferSize) continue;
      if(latency > _active.maximumBufferSize) continue;
      latencies.append(latency);
    }
    return latencies;
  }

  auto availableChannels() -> vector<uint> {
    return {1, 2};
  }

  auto ready() -> bool { return _ready; }
  auto context() -> uintptr { return _context; }
  auto device() -> string { return _device; }
  auto blocking() -> bool { return _blocking; }
  auto channels() -> uint { return _channels; }
  auto frequency() -> double { return _frequency; }
  auto latency() -> uint { return _latency; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
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
    return initialize();
  }

  auto setChannels(uint channels) -> bool {
    if(_channels == channels) return true;
    _channels = channels;
    return initialize();
  }

  auto setLatency(uint latency) -> bool {
    if(_latency == latency) return true;
    _latency = latency;
    return initialize();
  }

  auto clear() -> void {
    if(!ready()) return;
    for(uint n : range(_channels)) {
      memory::fill(_channel[n].buffers[0], _latency * _sampleSize);
      memory::fill(_channel[n].buffers[1], _latency * _sampleSize);
    }
    memory::fill(_queue.samples, sizeof(_queue.samples));
    _queue.read = 0;
    _queue.write = 0;
    _queue.count = 0;
  }

  auto output(const double samples[]) -> void {
    if(!ready()) return;
    if(_blocking) {
      while(_queue.count >= _latency);
    }
    for(uint n : range(_channels)) {
      _queue.samples[_queue.write][n] = samples[n];
    }
    _queue.write++;
    _queue.count++;
  }

private:
  auto initialize() -> bool {
    terminate();

    //enumerate available ASIO drivers from the registry
    for(auto candidate : registry::contents("HKLM\\SOFTWARE\\ASIO\\")) {
      if(auto classID = registry::read({"HKLM\\SOFTWARE\\ASIO\\", candidate, "CLSID"})) {
        _devices.append({candidate.trimRight("\\", 1L), classID});
        if(candidate == _device) _active = _devices.right();
      }
    }
    if(!_devices) return false;

    if(!_active.name) {
      _active = _devices.left();
      _device = _active.name;
    }

    CLSID classID;
    if(CLSIDFromString((LPOLESTR)utf16_t(_active.classID), (LPCLSID)&classID) != S_OK) return false;
    if(CoCreateInstance(classID, 0, CLSCTX_INPROC_SERVER, classID, (void**)&_asio) != S_OK) return false;

    if(!_asio->init((void*)_context)) return false;
    if(_asio->getSampleRate(&_active.sampleRate) != ASE_OK) return false;
    if(_asio->getChannels(&_active.inputChannels, &_active.outputChannels) != ASE_OK) return false;
    if(_asio->getBufferSize(
      &_active.minimumBufferSize,
      &_active.maximumBufferSize,
      &_active.preferredBufferSize,
      &_active.granularity
    ) != ASE_OK) return false;

    _frequency = _active.sampleRate;
    _latency = _latency < _active.minimumBufferSize ? _active.minimumBufferSize : _latency;
    _latency = _latency > _active.maximumBufferSize ? _active.maximumBufferSize : _latency;

    for(auto n : range(_channels)) {
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
    if(_asio->createBuffers(_channel, _channels, _latency, &callbacks) != ASE_OK) return false;
    if(_asio->getLatencies(&_active.inputLatency, &_active.outputLatency) != ASE_OK) return false;

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
    _devices.reset();
    _active = {};
    if(_asio) {
      _asio->stop();
      _asio->disposeBuffers();
      _asio->Release();
      _asio = nullptr;
    }
  }

private:
  static auto _bufferSwitch(long doubleBufferInput, ASIOBool directProcess) -> void {
    return self->bufferSwitch(doubleBufferInput, directProcess);
  }

  static auto _sampleRateDidChange(ASIOSampleRate sampleRate) -> void {
    return self->sampleRateDidChange(sampleRate);
  }

  static auto _asioMessage(long selector, long value, void* message, double* optional) -> long {
    return self->asioMessage(selector, value, message, optional);
  }

  static auto _bufferSwitchTimeInfo(ASIOTime* parameters, long doubleBufferIndex, ASIOBool directProcess) -> ASIOTime* {
    return self->bufferSwitchTimeInfo(parameters, doubleBufferIndex, directProcess);
  }

  auto bufferSwitch(long doubleBufferInput, ASIOBool directProcess) -> void {
    for(uint sampleIndex : range(_latency)) {
      double samples[8] = {0};
      if(_queue.count) {
        for(uint n : range(_channels)) {
          samples[n] = _queue.samples[_queue.read][n];
        }
        _queue.read++;
        _queue.count--;
      }

      for(uint n : range(_channels)) {
        auto buffer = (uint8_t*)_channel[n].buffers[doubleBufferInput];
        buffer += sampleIndex * _sampleSize;

        switch(_sampleFormat) {
        case ASIOSTInt16LSB: {
          *(int16_t*)buffer = samples[n] * double(1 << 15);
          break;
        }

        case ASIOSTInt24LSB: {
          int value = samples[n] * double(1 << 23);
          buffer[0] = value >> 0;
          buffer[1] = value >> 8;
          buffer[2] = value >> 16;
          break;
        }

        case ASIOSTInt32LSB: {
          *(int32_t*)buffer = samples[n] * double(1 << 31);
          break;
        }

        case ASIOSTFloat32LSB: {
          *(float*)buffer = samples[n];
          break;
        }

        case ASIOSTFloat64LSB: {
          *(double*)buffer = samples[n];
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
  uintptr _context = 0;
  string _device;
  bool _blocking = true;
  uint _channels = 2;
  double _frequency = 48000.0;
  uint _latency = 0;

  struct Queue {
    double samples[65536][8];
    uint16_t read;
    uint16_t write;
    std::atomic<uint16_t> count;
  };

  struct Device {
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
  vector<Device> _devices;
  Device _active;
  IASIO* _asio = nullptr;
  ASIOBufferInfo _channel[8];
  long _sampleFormat;
  long _sampleSize;
};

AudioASIO* AudioASIO::self = nullptr;
