#if defined(Hiro_LineEdit)

namespace hiro {

struct pLineEdit : pWidget {
  Declare(LineEdit, Widget)

  auto minimumSize() const -> Size;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;

  auto onChange() -> void;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT> override;

  auto _text() -> string;

  HBRUSH backgroundBrush = nullptr;
};

}

#endif
