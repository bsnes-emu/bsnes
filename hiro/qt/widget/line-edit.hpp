#if defined(Hiro_LineEdit)

namespace hiro {

struct pLineEdit : pWidget {
  Declare(LineEdit, Widget)

  auto minimumSize() const -> Size override;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void;

  QtLineEdit* qtLineEdit = nullptr;
};

}

#endif
