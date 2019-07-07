#if defined(Hiro_TextEdit)

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setTextCursor(TextCursor cursor) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;
  auto textCursor() const -> TextCursor;

  auto _setState() -> void;

  QtTextEdit* qtTextEdit = nullptr;
};

}

#endif
