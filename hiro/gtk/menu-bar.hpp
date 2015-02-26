namespace hiro {

struct pMenuBar : pObject {
  Declare(MenuBar, Object)

  auto append(shared_pointer<mMenu> menu) -> void;
  auto remove(shared_pointer<mMenu> menu) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const string& font) -> void override;
  auto setVisible(bool visible) -> void override;

  auto _parent() -> pWindow*;
};

}
