#if defined(Hiro_HexEdit)

namespace hiro {

struct pHexEdit : pWidget {
  Declare(HexEdit, Widget)

  auto setAddress(unsigned address) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setColumns(unsigned columns) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setLength(unsigned length) -> void;
  auto setRows(unsigned rows) -> void;
  auto update() -> void;

  auto keyPress(unsigned key) -> bool;
  auto rows() -> signed;
  auto rowsScrollable() -> signed;
  auto scrollPosition() -> signed;
  auto scrollTo(signed position) -> void;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT> override;

  HWND scrollBar = nullptr;
  HBRUSH backgroundBrush = nullptr;
};

}

#endif
