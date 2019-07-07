#if defined(Hiro_TextEdit)

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto focused() const -> bool override;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setFocused() -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setTextCursor(TextCursor textCursor) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;
  auto textCursor() const -> TextCursor;

  GtkWidget* subWidget = nullptr;
  GtkTextBuffer* textBuffer = nullptr;
};

}

#endif
