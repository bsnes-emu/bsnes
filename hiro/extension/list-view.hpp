#if defined(Hiro_ListView)

struct ListView;
struct mListView;
using sListView = shared_pointer<mListView>;

struct mListView : mTableView {
  using type = mListView;
  using mTableView::append;
  using mTableView::remove;

  mListView();
  auto batched() const -> vector<ListViewItem>;
  auto doActivate() const -> void;
  auto doChange() const -> void;
  auto doToggle(ListViewItem) const -> void;
  auto item(uint position) const -> ListViewItem;
  auto items() const -> vector<ListViewItem>;
  auto onActivate(const function<void ()>& callback) -> type&;
  auto onChange(const function<void ()>& callback) -> type&;
  auto onToggle(const function<void (ListViewItem)>& callback) -> type&;
  auto reset() -> type& override;
  auto selected() const -> ListViewItem;

//private:
  struct State {
    function<void ()> onActivate;
    function<void ()> onChange;
    function<void (ListViewItem)> onToggle;
  } state;
};

#endif
