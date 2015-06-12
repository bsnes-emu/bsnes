#if defined(Hiro_Action)

namespace hiro {

struct pAction : pObject {
  Declare(Action, Object)

  auto setEnabled(bool enabled) -> void;
  auto setVisible(bool visible) -> void;

  auto _parentMenu() -> maybe<pMenu&>;
  auto _parentMenuBar() -> maybe<pMenuBar&>;
  auto _parentPopupMenu() -> maybe<pPopupMenu&>;
  auto _synchronize() -> void;

  unsigned position = 0;
};

}

#endif
