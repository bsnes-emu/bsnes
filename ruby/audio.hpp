struct Audio {
  static auto create(string driver = "") -> Audio*;
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;
  static auto availableDrivers() -> vector<string>;

  virtual ~Audio() = default;

  virtual auto driver() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasExclusive() -> bool { return false; }
  virtual auto hasContext() -> bool { return false; }
  virtual auto hasDevice() -> bool { return false; }
  virtual auto hasBlocking() -> bool { return false; }
  virtual auto hasDynamic() -> bool { return false; }
  virtual auto hasChannels() -> bool { return false; }
  virtual auto hasFrequency() -> bool { return false; }
  virtual auto hasLatency() -> bool { return false; }

  virtual auto availableDevices() -> vector<string> { return {"Default"}; }
  virtual auto availableChannels() -> vector<uint> { return {2}; }
  virtual auto availableFrequencies() -> vector<double> { return {48000.0}; }
  virtual auto availableLatencies() -> vector<uint> { return {0}; }

  virtual auto defaultDevice() -> string { return availableDevices().first(); }
  virtual auto defaultChannels() -> uint { return availableChannels().first(); }
  virtual auto defaultFrequency() -> double { return availableFrequencies().first(); }
  virtual auto defaultLatency() -> uint { return availableLatencies().first(); }

  virtual auto exclusive() -> bool { return _exclusive; }
  virtual auto context() -> uintptr { return _context; }
  virtual auto device() -> string { return _device; }
  virtual auto blocking() -> bool { return _blocking; }
  virtual auto dynamic() -> bool { return _dynamic; }
  virtual auto channels() -> uint { return _channels; }
  virtual auto frequency() -> double { return _frequency; }
  virtual auto latency() -> uint { return _latency; }

  virtual auto setExclusive(bool exclusive) -> bool;
  virtual auto setContext(uintptr context) -> bool;
  virtual auto setDevice(string device) -> bool;
  virtual auto setBlocking(bool blocking) -> bool;
  virtual auto setDynamic(bool dynamic) -> bool;
  virtual auto setChannels(uint channels) -> bool;
  virtual auto setFrequency(double frequency) -> bool;
  virtual auto setLatency(uint latency) -> bool;

  virtual auto clear() -> void {}
  virtual auto level() -> double { return 0.5; }
  virtual auto output(const double samples[]) -> void {}

protected:
  virtual auto pending() -> bool;
  virtual auto outputDynamic(const double samples[]) -> void;

  bool _exclusive = false;
  uintptr _context = 0;
  string _device = "Default";
  bool _blocking = false;
  bool _dynamic = false;
  uint _channels = 2;
  double _frequency = 48000.0;
  uint _latency = 0;

  vector<nall::DSP::Resampler::Cubic> _resamplers;
};
