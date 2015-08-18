#if defined(Hiro_MenuBar)

namespace hiro {

struct pMenuBar : pObject {
  Declare(MenuBar, Object)

  auto append(sMenu menu) -> void;
  auto remove(sMenu menu) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const string& font) -> void override;
  auto setVisible(bool visible) -> void override;

  auto _parent() -> maybe<pWindow&>;
  auto _setState() -> void;
};

}

#endif
