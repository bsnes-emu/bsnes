#if defined(Hiro_TextEdit)

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto focused() const -> bool override;
  auto setBackgroundColor(Color color) -> void;
  auto setCursor(Cursor cursor) -> void;
  auto setEditable(bool editable) -> void;
  auto setFocused() -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  GtkWidget* subWidget = nullptr;
  GtkTextBuffer* textBuffer = nullptr;
};

}

#endif
