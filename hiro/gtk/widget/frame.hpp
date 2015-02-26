namespace hiro {

struct pFrame : pWidget {
  Declare(Frame, Widget)

  auto append(shared_pointer<mLayout> layout) -> void;
  auto container(mWidget& widget) -> GtkWidget* override;
  auto remove(shared_pointer<mLayout> layout) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const string& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _layout() -> pLayout*;

  GtkWidget* gtkLabel = nullptr;
};

}
