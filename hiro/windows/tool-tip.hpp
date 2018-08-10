namespace hiro {

struct pToolTip {
  enum : uint { Delay = 1000, Timeout = 10000 };

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
