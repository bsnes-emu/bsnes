struct Video {
  static auto create(string driver = "") -> Video*;
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;
  static auto availableDrivers() -> vector<string>;

  virtual ~Video() = default;

  virtual auto driver() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasExclusive() -> bool { return false; }
  virtual auto hasContext() -> bool { return false; }
  virtual auto hasBlocking() -> bool { return false; }
  virtual auto hasFlush() -> bool { return false; }
  virtual auto hasFormat() -> bool { return false; }
  virtual auto hasSmooth() -> bool { return false; }
  virtual auto hasShader() -> bool { return false; }

  virtual auto availableFormats() -> vector<string> { return {"Default"}; }

  virtual auto defaultFormat() -> string { return availableFormats().first(); }

  virtual auto exclusive() -> bool { return _exclusive; }
  virtual auto context() -> uintptr { return _context; }
  virtual auto blocking() -> bool { return _blocking; }
  virtual auto flush() -> bool { return _flush; }
  virtual auto format() -> string { return _format; }
  virtual auto smooth() -> bool { return _smooth; }
  virtual auto shader() -> string { return _shader; }

  virtual auto setExclusive(bool exclusive) -> bool;
  virtual auto setContext(uintptr context) -> bool;
  virtual auto setBlocking(bool blocking) -> bool;
  virtual auto setFlush(bool flush) -> bool;
  virtual auto setFormat(string format) -> bool;
  virtual auto setSmooth(bool smooth) -> bool;
  virtual auto setShader(string shader) -> bool;

  virtual auto clear() -> void {}
  virtual auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool { return false; }
  virtual auto release() -> void {}
  virtual auto output() -> void {}
  virtual auto poll() -> void {}

  auto onUpdate(const function<void (uint, uint)>&) -> void;
  auto doUpdate(uint width, uint height) -> void;

protected:
  bool _exclusive = false;
  uintptr _context = 0;
  bool _blocking = false;
  bool _flush = false;
  string _format = "Default";
  bool _smooth = false;
  string _shader = "";

  function<void (uint, uint)> _onUpdate;
};
