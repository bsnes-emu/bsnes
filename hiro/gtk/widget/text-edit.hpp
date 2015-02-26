namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto focused() const -> bool override;
  auto setBackgroundColor(Color color) -> void;
  auto setCursorPosition(unsigned position) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  GtkWidget* subWidget = nullptr;
  GtkTextBuffer* textBuffer = nullptr;
};

}
