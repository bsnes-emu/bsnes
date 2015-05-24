struct Input {
  static const nall::string Handle;
  static const nall::string KeyboardSupport;
  static const nall::string MouseSupport;
  static const nall::string JoypadSupport;
  static const nall::string JoypadRumbleSupport;

  Input() = default;
  virtual ~Input() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto acquire() -> bool { return false; }
  virtual auto unacquire() -> bool { return false; }
  virtual auto acquired() -> bool { return false; }

  virtual auto poll() -> nall::vector<nall::shared_pointer<nall::HID::Device>> { return {}; }
  virtual auto rumble(uint64_t id, bool enable) -> bool { return false; }
  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}
};
