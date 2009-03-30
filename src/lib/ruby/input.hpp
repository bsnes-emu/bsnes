class Input {
public:
  enum Setting {
    Handle,
    KeyboardSupport,
    MouseSupport,
    JoypadSupport,
  };

  virtual bool cap(Setting) { return false; }
  virtual uintptr_t get(Setting) { return false; }
  virtual bool set(Setting, uintptr_t) { return false; }

  virtual bool acquire() { return false; }
  virtual bool unacquire() { return false; }
  virtual bool acquired() { return false; }

  virtual bool poll(int16_t *table) { return false; }
  virtual bool init() { return true; }
  virtual void term() {}

  Input() {}
  virtual ~Input() {}
};
