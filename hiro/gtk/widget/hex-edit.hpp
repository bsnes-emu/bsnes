#if defined(Hiro_HexEdit)

namespace hiro {

struct pHexEdit : pWidget {
  Declare(HexEdit, Widget)

  auto focused() const -> bool override;
  auto setBackgroundColor(Color color) -> void;
  auto setColumns(unsigned columns) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setLength(unsigned length) -> void;
  auto setOffset(unsigned offset) -> void;
  auto setRows(unsigned rows) -> void;
  auto update() -> void;

  auto cursorPosition() -> unsigned;
  auto keyPress(unsigned scancode, unsigned mask) -> bool;
  auto rows() -> signed;
  auto rowsScrollable() -> signed;
  auto scroll(signed position) -> void;
  auto setCursorPosition(unsigned position) -> void;
  auto setScroll() -> void;
  auto updateScroll() -> void;

  GtkWidget* container = nullptr;
  GtkWidget* subWidget = nullptr;
  GtkWidget* scrollBar = nullptr;
  GtkTextBuffer* textBuffer = nullptr;
  GtkTextMark* textCursor = nullptr;
};

}

#endif
