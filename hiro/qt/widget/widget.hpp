#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  auto focused() const -> bool override;
  virtual auto setDroppable(bool droppable) -> void;
  auto setEnabled(bool enabled) -> void override;
  virtual auto setFocusable(bool focusable) -> void;
  auto setFocused() -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setMouseCursor(const MouseCursor& mouseCursor) -> void;
  auto setToolTip(const string& toolTip) -> void;
  auto setVisible(bool visible) -> void override;

  QWidget* qtWidget = nullptr;
};

}

#endif
