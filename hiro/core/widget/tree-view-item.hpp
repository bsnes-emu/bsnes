#if defined(Hiro_TreeView)
struct mTreeViewItem : mObject {
  Declare(TreeViewItem)

  auto append(sTreeViewItem item) -> type&;
  auto backgroundColor(bool recursive = false) const -> Color;
  auto checkable() const -> bool;
  auto checked() const -> bool;
  auto expanded() const -> bool;
  auto foregroundColor(bool recursive = false) const -> Color;
  auto icon() const -> image;
  auto item(const string& path) const -> TreeViewItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<TreeViewItem>;
  auto path() const -> string;
  auto remove() -> type& override;
  auto remove(sTreeViewItem item) -> type&;
  auto selected() const -> bool;
  auto setBackgroundColor(Color color = {}) -> type&;
  auto setCheckable(bool checkable = true) -> type&;
  auto setChecked(bool checked = true) -> type&;
  auto setExpanded(bool expanded = true) -> type&;
  auto setFocused() -> type& override;
  auto setForegroundColor(Color color = {}) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;
  auto setSelected() -> type&;
  auto setText(const string& text = "") -> type&;
  auto text() const -> string;

//private:
  struct State {
    Color backgroundColor;
    bool checkable = false;
    bool checked = false;
    bool expanded = false;
    Color foregroundColor;
    image icon;
    vector<sTreeViewItem> items;
    string text;
  } state;

  auto destruct() -> void override;
};
#endif
