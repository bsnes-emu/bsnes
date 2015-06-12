#if defined(Hiro_TextEdit)

namespace hiro {

struct pTextEdit : pWidget {
  Declare(TextEdit, Widget)

  auto setBackgroundColor(Color color) -> void;
  auto setCursorPosition(unsigned position) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(string text) -> void;
  auto setWordWrap(bool wordWrap) -> void;
  auto text() const -> string;

  auto onChange() -> void;

  HBRUSH backgroundBrush = nullptr;
};

}

#endif
