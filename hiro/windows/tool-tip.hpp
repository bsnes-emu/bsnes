namespace hiro {

struct pToolTip {
  pToolTip(const string& text);
  ~pToolTip();

  auto drawLayered() -> void;
  auto drawOpaque() -> void;
  auto show() -> void;
  auto hide() -> void;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT>;

  HWND hwnd = nullptr;
  HTHEME htheme = nullptr;
  POINT position{};
  SIZE size{};
  POINT tracking{};
  string text;
  Timer timeout;
};

}
