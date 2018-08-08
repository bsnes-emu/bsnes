#if defined(Hiro_Widget)

namespace hiro {

struct pWidget : pSizable {
  Declare(Widget, Sizable)

  auto focused() const -> bool;
  auto setEnabled(bool enabled) -> void override;
  auto setFocused() -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setToolTip(const string& toolTip) -> void;
  auto setVisible(bool visible) -> void override;

  NSView* cocoaView = nullptr;
  bool abstract = false;
};

}

#endif
