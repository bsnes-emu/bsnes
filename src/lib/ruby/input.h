class Input {
public:
  enum Setting {
    Handle,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual bool key_down(uint16_t key) { return false; }
  virtual bool key_up  (uint16_t key) { return !key_down(key); }

  virtual void clear() {}
  virtual void poll() {}
  virtual bool init() { return true; }
  virtual void term() {}

  Input() {}
  virtual ~Input() {}
};
