#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  virtual auto container(mWidget& widget) -> GtkWidget*;
  virtual auto focused() const -> bool override;
  auto setEnabled(bool enabled) -> void override;
  virtual auto setFocused() -> void override;
  auto setFont(const string& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setVisible(bool visible) -> void override;

  GtkWidget* gtkWidget = nullptr;
  GtkWidget* gtkParent = nullptr;
};

}

#endif
