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

  auto _keyPressEvent(QKeyEvent*) -> void;
  auto _rows() -> signed;
  auto _rowsScrollable() -> signed;
  auto _scrollTo(signed position) -> void;
  auto _setState() -> void;

  QtHexEdit* qtHexEdit = nullptr;
  QHBoxLayout* qtLayout = nullptr;
  QtHexEditScrollBar* qtScrollBar = nullptr;
};

}

#endif
