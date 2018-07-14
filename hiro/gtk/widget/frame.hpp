#if defined(Hiro_Frame)

namespace hiro {

struct pFrame : pWidget {
  Declare(Frame, Widget)

  auto append(sSizable sizable) -> void;
  auto container(mWidget& widget) -> GtkWidget* override;
  auto remove(sSizable sizable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _sizable() -> maybe<pSizable&>;

  GtkWidget* gtkLabel = nullptr;
};

}

#endif
