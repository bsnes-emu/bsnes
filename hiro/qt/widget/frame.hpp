#if defined(Hiro_Frame)

namespace hiro {

struct pFrame : pWidget {
  Declare(Frame, Widget)

  auto append(sLayout layout) -> void;
  auto remove(sLayout layout) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _setState() -> void;

  QGroupBox* qtFrame = nullptr;
};

}

#endif
