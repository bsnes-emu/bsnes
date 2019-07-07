#if defined(Hiro_TextEdit)
struct mTextEdit : mWidget {
  Declare(TextEdit)

  auto backgroundColor() const -> Color;
  auto doChange() const -> void;
  auto doMove() const -> void;
  auto editable() const -> bool;
  auto foregroundColor() const -> Color;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onMove(const function<void ()>& callback = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setEditable(bool editable = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setTextCursor(TextCursor textCursor = {}) -> type&;
  auto setWordWrap(bool wordWrap = true) -> type&;
  auto text() const -> string;
  auto textCursor() const -> TextCursor;
  auto wordWrap() const -> bool;

//private:
  struct State {
    Color backgroundColor;
    bool editable = true;
    Color foregroundColor;
    function<void ()> onChange;
    function<void ()> onMove;
    string text;
    TextCursor textCursor;
    bool wordWrap = true;
  } state;
};
#endif
