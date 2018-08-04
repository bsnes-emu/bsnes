#if defined(Hiro_SourceEdit)
struct mSourceEdit : mWidget {
  Declare(SourceEdit)

  auto cursor() const -> Cursor;
  auto doChange() const -> void;
  auto doMove() const -> void;
  auto editable() const -> bool;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onMove(const function<void ()>& callback = {}) -> type&;
  auto setCursor(Cursor cursor = {}) -> type&;
  auto setEditable(bool editable) -> type&;
  auto setText(const string& text = "") -> type&;
  auto setWordWrap(bool wordWrap = true) -> type&;
  auto text() const -> string;
  auto wordWrap() const -> bool;

//private:
  struct State {
    Cursor cursor;
    bool editable = true;
    function<void ()> onChange;
    function<void ()> onMove;
    string text;
    bool wordWrap = true;
  } state;
};
#endif
