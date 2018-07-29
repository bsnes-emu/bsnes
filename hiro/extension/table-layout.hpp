#if defined(Hiro_TableLayout)

struct TableLayout;
struct TableLayoutColumn;
struct TableLayoutRow;
struct TableLayoutCell;

struct mTableLayout;
struct mTableLayoutColumn;
struct mTableLayoutRow;
struct mTableLayoutCell;

using sTableLayout = shared_pointer<mTableLayout>;
using sTableLayoutColumn = shared_pointer<mTableLayoutColumn>;
using sTableLayoutRow = shared_pointer<mTableLayoutRow>;
using sTableLayoutCell = shared_pointer<mTableLayoutCell>;

struct mTableLayout : mSizable {
  using type = mTableLayout;
  using mSizable::remove;

  auto alignment() const -> Alignment;
  auto append(sSizable sizable, Size size) -> type&;
  auto cell(uint position) const -> TableLayoutCell;
  auto cell(uint x, uint y) const -> TableLayoutCell;
  auto cell(sSizable sizable) const -> TableLayoutCell;
  auto cellCount() const -> uint;
  auto column(uint position) const -> TableLayoutColumn;
  auto columnCount() const -> uint;
  auto minimumSize() const -> Size override;
  auto padding() const -> Geometry;
  auto remove(sSizable sizable) -> type&;
  auto remove(sTableLayoutCell cell) -> type&;
  auto reset() -> type&;
  auto row(uint position) const -> TableLayoutRow;
  auto rowCount() const -> uint;
  auto setAlignment(Alignment alignment) -> type&;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setGeometry(Geometry geometry) -> type& override;
  auto setPadding(Geometry padding) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSize(Size size) -> type&;
  auto setVisible(bool visible) -> type& override;
  auto size() const -> Size;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    Alignment alignment;
    vector<TableLayoutCell> cells;
    vector<TableLayoutColumn> columns;
    Geometry padding;
    vector<TableLayoutRow> rows;
    Size size;
  } state;
};

struct mTableLayoutColumn : mObject {
  using type = mTableLayoutColumn;

  auto alignment() const -> Alignment;
  auto setAlignment(Alignment alignment) -> type&;
  auto setSpacing(float spacing) -> type&;
  auto spacing() const -> float;
  auto synchronize() -> type&;

private:
  struct State {
    Alignment alignment;
    float spacing = 5;
  } state;

  friend class mTableLayout;
};

struct mTableLayoutRow : mObject {
  using type = mTableLayoutRow;

  auto alignment() const -> Alignment;
  auto setAlignment(Alignment alignment) -> type&;
  auto setSpacing(float spacing) -> type&;
  auto spacing() const -> float;
  auto synchronize() -> type&;

private:
  struct State {
    Alignment alignment;
    float spacing = 5;
  } state;

  friend class mTableLayout;
};

struct mTableLayoutCell : mObject {
  using type = mTableLayoutCell;

  auto alignment() const -> Alignment;
  auto setAlignment(Alignment alignment) -> type&;
  auto setEnabled(bool enabled) -> type& override;
  auto setFont(const Font& font) -> type& override;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSizable(sSizable sizable) -> type&;
  auto setSize(Size size) -> type&;
  auto setVisible(bool visible) -> type& override;
  auto sizable() const -> Sizable;
  auto size() const -> Size;
  auto synchronize() -> type&;

private:
  auto destruct() -> void override;

  struct State {
    Alignment alignment;
    sSizable sizable;
    Size size;
  } state;

  friend class mTableLayout;
};

#endif
