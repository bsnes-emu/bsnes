#if defined(Hiro_TableView)
struct mTableViewItem : mObject {
  Declare(TableViewItem)

  auto alignment() const -> Alignment;
  auto append(sTableViewCell cell) -> type&;
  auto backgroundColor() const -> Color;
  auto cell(uint position) const -> TableViewCell;
  auto cellCount() const -> uint;
  auto cells() const -> vector<TableViewCell>;
  auto foregroundColor() const -> Color;
  auto remove() -> type& override;
  auto remove(sTableViewCell cell) -> type&;
  auto reset() -> type&;
  auto selected() const -> bool;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setFocused() -> type& override;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSelected(bool selected = true) -> type&;

//private:
  struct State {
    Alignment alignment;
    Color backgroundColor;
    vector<sTableViewCell> cells;
    Color foregroundColor;
    bool selected = false;
  } state;

  auto destruct() -> void override;
};
#endif
