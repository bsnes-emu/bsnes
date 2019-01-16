struct Video;

struct VideoDriver {
  VideoDriver(Video& super) : super(super) {}
  virtual ~VideoDriver() = default;

  virtual auto create() -> bool { return true; }
  virtual auto driver() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasExclusive() -> bool { return false; }
  virtual auto hasContext() -> bool { return false; }
  virtual auto hasBlocking() -> bool { return false; }
  virtual auto hasFlush() -> bool { return false; }
  virtual auto hasFormats() -> vector<string> { return {"RGB24"}; }
  virtual auto hasShader() -> bool { return false; }

  auto hasFormat(string format) -> bool { return (bool)hasFormats().find(format); }

  virtual auto setExclusive(bool exclusive) -> bool { return true; }
  virtual auto setContext(uintptr context) -> bool { return true; }
  virtual auto setBlocking(bool blocking) -> bool { return true; }
  virtual auto setFlush(bool flush) -> bool { return true; }
  virtual auto setFormat(string format) -> bool { return true; }
  virtual auto setShader(string shader) -> bool { return true; }

  virtual auto configure(uint width, uint height, double inputFrequency, double outputFrequency) -> bool { return true; }
  virtual auto clear() -> void {}
  virtual auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool { return false; }
  virtual auto release() -> void {}
  virtual auto output() -> void {}
  virtual auto poll() -> void {}

protected:
  Video& super;
  friend class Video;

  bool exclusive = false;
  uintptr context = 0;
  bool blocking = false;
  bool flush = false;
  string format = "RGB24";
  string shader = "Blur";

  uint width = 0;
  uint height = 0;
  double inputFrequency = 0.0;
  double outputFrequency = 0.0;
};

struct Video {
  static auto hasDrivers() -> vector<string>;
  static auto hasDriver(string driver) -> bool { return (bool)hasDrivers().find(driver); }
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;

  Video() : self(*this) { reset(); }
  explicit operator bool() { return instance->driver() != "None"; }
  auto reset() -> void { instance = new VideoDriver(*this); }
  auto create(string driver = "") -> bool;
  auto driver() -> string { return instance->driver(); }
  auto ready() -> bool { return instance->ready(); }

  auto hasExclusive() -> bool { return instance->hasExclusive(); }
  auto hasContext() -> bool { return instance->hasContext(); }
  auto hasBlocking() -> bool { return instance->hasBlocking(); }
  auto hasFlush() -> bool { return instance->hasFlush(); }
  auto hasFormats() -> vector<string> { return instance->hasFormats(); }
  auto hasShader() -> bool { return instance->hasShader(); }

  auto hasFormat(string format) -> bool { return instance->hasFormat(format); }

  auto exclusive() -> bool { return instance->exclusive; }
  auto context() -> uintptr { return instance->context; }
  auto blocking() -> bool { return instance->blocking; }
  auto flush() -> bool { return instance->flush; }
  auto format() -> string { return instance->format; }
  auto shader() -> string { return instance->shader; }

  auto setExclusive(bool exclusive) -> bool;
  auto setContext(uintptr context) -> bool;
  auto setBlocking(bool blocking) -> bool;
  auto setFlush(bool flush) -> bool;
  auto setFormat(string format) -> bool;
  auto setShader(string shader) -> bool;

  auto configure(uint width, uint height, double inputFrequency, double outputFrequency) -> bool;
  auto clear() -> void;
  struct Acquire {
    explicit operator bool() const { return data; }
    uint32_t* data = nullptr;
    uint pitch = 0;
  };
  auto acquire(uint width, uint height) -> Acquire;
  auto release() -> void;
  auto output() -> void;
  auto poll() -> void;

  auto onUpdate(const function<void (uint, uint)>&) -> void;
  auto doUpdate(uint width, uint height) -> void;

protected:
  Video& self;
  unique_pointer<VideoDriver> instance;
  function<void (uint, uint)> update;
};
