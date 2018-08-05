#if defined(Hiro_Object)

namespace hiro {

struct pObject : Lock {
  pObject(mObject& reference) : reference(reference) {}
  virtual ~pObject() = default;
  virtual auto construct() -> void;
  virtual auto destruct() -> void;
  virtual auto reconstruct() -> void;

  virtual auto focused() const -> bool;
  virtual auto remove() -> void;
  virtual auto reset() -> void;
  virtual auto setEnabled(bool enabled) -> void;
  virtual auto setFocused() -> void;
  virtual auto setFont(const Font& font) -> void;
  virtual auto setGroup(sGroup group) -> void;
  virtual auto setVisible(bool visible) -> void;

  mObject& reference;
};

}

#endif
