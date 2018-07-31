struct Video;

struct VideoDriver {
  VideoDriver(Video& super) : super(super) {}
  virtual ~VideoDriver() = default;

  virtual auto create() -> bool { return true; }
  virtual auto driverName() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasExclusive() -> bool { return false; }
  virtual auto hasContext() -> bool { return false; }
  virtual auto hasBlocking() -> bool { return false; }
  virtual auto hasFlush() -> bool { return false; }
  virtual auto hasFormats() -> vector<string> { return {"RGB24"}; }
  virtual auto hasSmooth() -> bool { return false; }
  virtual auto hasShader() -> bool { return false; }

  auto hasFormat(string format) -> bool { return (bool)hasFormats().find(format); }

  virtual auto setExclusive(bool exclusive) -> bool { return true; }
  virtual auto setContext(uintptr context) -> bool { return true; }
  virtual auto setBlocking(bool blocking) -> bool { return true; }
  virtual auto setFlush(bool flush) -> bool { return true; }
  virtual auto setFormat(string format) -> bool { return true; }
  virtual auto setSmooth(bool smooth) -> bool { return true; }
  virtual auto setShader(string shader) -> bool { return true; }

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
  bool smooth = false;
  string shader = "";
};

struct Video {
  static auto hasDrivers() -> vector<string>;
  static auto hasDriver(string driver) -> bool { return (bool)hasDrivers().find(driver); }
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;

  Video() : self(*this) {}
  explicit operator bool() const { return (bool)driver; }
  auto reset() -> void { driver.reset(); }
  auto create(string driver = "") -> bool;
  auto driverName() -> string { return driver->driverName(); }
  auto ready() -> bool { return driver->ready(); }

  auto hasExclusive() -> bool { return driver->hasExclusive(); }
  auto hasContext() -> bool { return driver->hasContext(); }
  auto hasBlocking() -> bool { return driver->hasBlocking(); }
  auto hasFlush() -> bool { return driver->hasFlush(); }
  auto hasFormats() -> vector<string> { return driver->hasFormats(); }
  auto hasSmooth() -> bool { return driver->hasSmooth(); }
  auto hasShader() -> bool { return driver->hasShader(); }

  auto hasFormat(string format) -> bool { return driver->hasFormat(format); }

  auto exclusive() -> bool { return driver->exclusive; }
  auto context() -> uintptr { return driver->context; }
  auto blocking() -> bool { return driver->blocking; }
  auto flush() -> bool { return driver->flush; }
  auto format() -> string { return driver->format; }
  auto smooth() -> bool { return driver->smooth; }
  auto shader() -> string { return driver->shader; }

  auto setExclusive(bool exclusive) -> bool;
  auto setContext(uintptr context) -> bool;
  auto setBlocking(bool blocking) -> bool;
  auto setFlush(bool flush) -> bool;
  auto setFormat(string format) -> bool;
  auto setSmooth(bool smooth) -> bool;
  auto setShader(string shader) -> bool;

  auto clear() -> void;
  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool;
  auto release() -> void;
  auto output() -> void;
  auto poll() -> void;

  auto onUpdate(const function<void (uint, uint)>&) -> void;
  auto doUpdate(uint width, uint height) -> void;

protected:
  Video& self;
  unique_pointer<VideoDriver> driver;
  function<void (uint, uint)> update;
};
