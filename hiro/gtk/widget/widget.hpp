#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  virtual auto container(mWidget& widget) -> GtkWidget*;
  auto focused() const -> bool override;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setToolTip(const string& toolTip) -> void;
  auto setVisible(bool visible) -> void override;

  GtkWidget* gtkWidget = nullptr;
  GtkWidget* gtkParent = nullptr;
};

}

#endif
