#if defined(Hiro_Action)

namespace hiro {

struct pAction : pObject {
  Declare(Action, Object)

  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setVisible(bool visible) -> void override;

  auto _parentMenu() -> maybe<pMenu&>;
  auto _parentMenuBar() -> maybe<pMenuBar&>;
  auto _parentPopupMenu() -> maybe<pPopupMenu&>;
  virtual auto _setState() -> void;
};

}

#endif
