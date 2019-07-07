#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  virtual auto container(mWidget& widget) -> GtkWidget*;
  auto focused() const -> bool override;
  auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  virtual auto setFocusable(bool focusable) -> void;
  auto setFocused() -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setMouseCursor(const MouseCursor& mouseCursor) -> void;
  auto setToolTip(const string& toolTip) -> void;
  auto setVisible(bool visible) -> void override;

  GtkWidget* gtkWidget = nullptr;
  GtkWidget* gtkParent = nullptr;
  GdkCursor* gdkMouseCursor = nullptr;
};

}

#endif
