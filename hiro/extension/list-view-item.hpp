#if defined(Hiro_ListView)

struct ListViewItem;
struct mListViewItem;
using sListViewItem = shared_pointer<mListViewItem>;

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
