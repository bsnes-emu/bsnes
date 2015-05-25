struct Audio {
  static const nall::string Device;
  static const nall::string Handle;
  static const nall::string Synchronize;
  static const nall::string Frequency;
  static const nall::string Latency;

  virtual ~Audio() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto sample(uint16_t left, uint16_t right) -> void {}
  virtual auto clear() -> void {}

  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}
};
