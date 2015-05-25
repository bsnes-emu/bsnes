struct Video {
  static const nall::string Handle;
  static const nall::string Synchronize;
  static const nall::string Depth;
  static const nall::string Filter;
  static const nall::string Shader;

  static const unsigned FilterNearest;
  static const unsigned FilterLinear;

  virtual ~Video() = default;

  virtual auto cap(const nall::string& name) -> bool { return false; }
  virtual auto get(const nall::string& name) -> nall::any { return false; }
  virtual auto set(const nall::string& name, const nall::any& value) -> bool { return false; }

  virtual auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool { return false; }
  virtual auto unlock() -> void {}
  virtual auto clear() -> void {}
  virtual auto refresh() -> void {}

  virtual auto init() -> bool { return true; }
  virtual auto term() -> void {}
};
