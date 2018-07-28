struct Input {
  static auto create(string driver = "") -> Input*;
  static auto optimalDriver() -> string;
  static auto safestDriver() -> string;
  static auto availableDrivers() -> vector<string>;

  virtual ~Input() = default;

  virtual auto driver() -> string { return "None"; }
  virtual auto ready() -> bool { return true; }

  virtual auto hasContext() -> bool { return false; }

  virtual auto context() -> uintptr { return _context; }

  virtual auto setContext(uintptr context) -> bool;

  virtual auto acquired() -> bool { return false; }
  virtual auto acquire() -> bool { return false; }
  virtual auto release() -> bool { return false; }
  virtual auto poll() -> vector<shared_pointer<nall::HID::Device>> { return {}; }
  virtual auto rumble(uint64_t id, bool enable) -> bool { return false; }

  auto onChange(const function<void (shared_pointer<nall::HID::Device>, uint, uint, int16_t, int16_t)>&) -> void;
  auto doChange(shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

protected:
  uintptr _context = 0;

  function<void (shared_pointer<nall::HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue)> _onChange;
};
