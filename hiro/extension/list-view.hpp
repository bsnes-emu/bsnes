#if defined(Hiro_ListView)

struct ListView;
struct ListViewItem;

struct mListView;
struct mListViewItem;

using sListView = shared_pointer<mListView>;
using sListViewItem = shared_pointer<mListViewItem>;

struct mListView : mTableView {
  using type = mListView;
  using mTableView::append;
  using mTableView::remove;

  mListView();
  auto batched() const -> vector<ListViewItem>;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doContext() const -> void;
  auto doToggle(ListViewItem) const -> void;
  auto item(uint position) const -> ListViewItem;
  auto items() const -> vector<ListViewItem>;
  auto onActivate(const function<void ()>& callback) -> type&;
  auto onChange(const function<void ()>& callback) -> type&;
  auto onContext(const function<void ()>& callback) -> type&;
  auto onToggle(const function<void (ListViewItem)>& callback) -> type&;
  auto reset() -> type& override;
  auto resize() -> type&;
  auto selected() const -> ListViewItem;
  auto setVisible(bool visible = true) -> type&;

//private:
  struct State {
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void ()> onContext;
    function<void (ListViewItem)> onToggle;
  } state;
};

struct mListViewItem : mTableViewItem {
  using type = mListViewItem;
  using mTableViewItem::append;
  using mTableViewItem::remove;

  mListViewItem();
  auto checkable() const -> bool;
  auto checked() const -> bool;
  auto icon() const -> image;
  auto reset() -> type&;
  auto setCheckable(bool checkable) -> type&;
  auto setChecked(bool checked) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setText(const string& text) -> type&;
  auto text() const -> string;
};

#endif
