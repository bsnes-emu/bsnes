#if defined(Hiro_MenuBar)

namespace hiro {

struct pMenuBar : pObject {
  Declare(MenuBar, Object)

  auto append(sMenu menu) -> void;
  auto remove(sMenu menu) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setVisible(bool visible) -> void override;

  auto _parent() -> maybe<pWindow&>;
  auto _update() -> void;
  auto _updateDeferred() -> void;
  
  HMENU hmenu = 0;
  vector<wObject> objects;
};

}

#endif
