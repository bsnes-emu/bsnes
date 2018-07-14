#if defined(Hiro_Frame)

namespace hiro {

struct pFrame : pWidget {
  Declare(Frame, Widget)

  auto append(sSizable sizable) -> void;
  auto remove(sSizable sizable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;
};

}

#endif
