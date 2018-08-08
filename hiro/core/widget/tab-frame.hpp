#if defined(Hiro_TabFrame)
struct mTabFrame : mWidget {
  Declare(TabFrame)
  using mObject::remove;
  friend class mTabFrameItem;

  auto append(sTabFrameItem item) -> type&;
  auto doChange() const -> void;
  auto doClose(sTabFrameItem item) const -> void;
  auto doMove(sTabFrameItem from, sTabFrameItem to) const -> void;
  auto item(uint position) const -> TabFrameItem;
  auto itemCount() const -> uint;
  auto items() const -> vector<TabFrameItem>;
  auto navigation() const -> Navigation;
  auto onChange(const function<void ()>& callback = {}) -> type&;
  auto onClose(const function<void (TabFrameItem)>& callback = {}) -> type&;
  auto onMove(const function<void (TabFrameItem, TabFrameItem)>& callback = {}) -> type&;
  auto remove(sTabFrameItem item) -> type&;
  auto reset() -> type&;
  auto selected() const -> TabFrameItem;
  auto setEnabled(bool enabled = true) -> type& override;
  auto setFont(const Font& font = {}) -> type& override;
  auto setNavigation(Navigation navigation = Navigation::Top) -> type&;
  auto setParent(mObject* object = nullptr, int offset = -1) -> type& override;
  auto setVisible(bool visible = true) -> type& override;

//private:
  struct State {
    vector<sTabFrameItem> items;
    Navigation navigation = Navigation::Top;
    function<void ()> onChange;
    function<void (TabFrameItem)> onClose;
    function<void (TabFrameItem, TabFrameItem)> onMove;
  } state;

  auto destruct() -> void override;
};
#endif
