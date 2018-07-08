#if defined(Hiro_Object)

namespace hiro {

struct pObject {
  pObject(mObject& reference) : reference(reference) {}
  virtual ~pObject() {}
  auto self() const -> mObject& { return (mObject&)reference; }
  auto state() const -> mObject::State& { return self().state; }
  virtual auto construct() -> void;
  virtual auto destruct() -> void;

  virtual auto focused() const -> bool;
  virtual auto remove() -> void;
  virtual auto reset() -> void;
  virtual auto setEnabled(bool enabled) -> void;
  virtual auto setFocused() -> void;
  virtual auto setFont(const Font& font) -> void;
  virtual auto setVisible(bool visible) -> void;

  auto locked() const -> bool { return locks || Application::state.quit; }
  auto lock() -> void { ++locks; }
  auto unlock() -> void { --locks; }

  struct Lock {
    Lock(pObject& self) : self(self) { self.locks++; }
    ~Lock() { self.locks--; }

    pObject& self;
  };
  auto acquire() -> Lock { return {*this}; }

  mObject& reference;
  signed locks = 0;
};

}

#endif
