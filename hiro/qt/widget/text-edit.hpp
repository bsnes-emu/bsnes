#if defined(Hiro_TextEdit)

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto setBackgroundColor(Color color) -> void;
  auto setCursorPosition(unsigned position) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  auto _setState() -> void;

  QtTextEdit* qtTextEdit = nullptr;
};

}

#endif
