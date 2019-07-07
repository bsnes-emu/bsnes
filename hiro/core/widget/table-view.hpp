#if defined(Hiro_TableView)
struct mTableView : mWidget {
  Declare(TableView)
  using mObject::remove;

  auto alignment() const -> Alignment;
  auto append(sTableViewColumn column) -> type&;
  auto append(sTableViewItem item) -> type&;
  auto backgroundColor() const -> Color;
  auto batchable() const -> bool;
  auto batched() const -> vector<TableViewItem>;
  auto bordered() const -> bool;
  auto column(uint position) const -> TableViewColumn;
  auto columnCount() const -> uint;
  auto columns() const -> vector<TableViewColumn>;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto doEdit(sTableViewCell cell) const -> void;
  auto doSort(sTableViewColumn column) const -> void;
  auto doToggle(sTableViewCell cell) const -> void;
  auto foregroundColor() const -> Color;
  auto headered() const -> bool;
  auto item(uint position) const -> TableViewItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<TableViewItem>;
  auto onActivate(const function<void ()>& callback = {}) -> type&;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onContext(const function<void ()>& callback = {}) -> type&;
  auto onEdit(const function<void (TableViewCell)>& callback = {}) -> type&;
  auto onSort(const function<void (TableViewColumn)>& callback = {}) -> type&;
  auto onToggle(const function<void (TableViewCell)>& callback = {}) -> type&;
  auto remove(sTableViewColumn column) -> type&;
  auto remove(sTableViewItem item) -> type&;
  auto reset() -> type&;
  auto resizeColumns() -> type&;
  auto selectAll() -> type&;
  auto selectNone() -> type&;
  auto selected() const -> TableViewItem;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setBatchable(bool batchable = true) -> type&;
  auto setBordered(bool bordered = true) -> type&;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setHeadered(bool headered = true) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type& override;
  auto setSortable(bool sortable = true) -> type&;
  auto sort() -> type&;
  auto sortable() const -> bool;

//private:
  struct State {
    uint activeColumn = 0;
    Alignment alignment;
    Color backgroundColor;
    bool batchable = false;
    bool bordered = false;
    vector<sTableViewColumn> columns;
    Color foregroundColor;
    bool headered = false;
    vector<sTableViewItem> items;
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    function<void (TableViewCell)> onEdit;
    function<void (TableViewColumn)> onSort;
    function<void (TableViewCell)> onToggle;
    bool sortable = false;
  } state;

  auto destruct() -> void override;
};
#endif
