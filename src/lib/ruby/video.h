class Video {
public:
  enum Setting {
    Handle,
    Synchronize,
    Filter,
  };

  enum Filter {
    FilterPoint,
    FilterLinear,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual bool lock(uint32_t *&data, unsigned &pitch) { return false; }
  virtual void unlock() {}

  virtual void clear() {}
  virtual void refresh(unsigned width, unsigned height) {}
  virtual bool init() { return true; }
  virtual void term() {}

  Video() {}
  virtual ~Video() {}
};
