#if defined(Hiro_Object)
struct mObject {
  Declare(Object)

  mObject();
  virtual ~mObject();
  mObject(const mObject&) = delete;
  mObject& operator=(const mObject&) = delete;

  explicit operator bool() const;

  auto abstract() const -> bool;
  auto adjustOffset(int displacement) -> type&;
  auto enabled(bool recursive = false) const -> bool;
  virtual auto focused() const -> bool;
  auto font(bool recursive = false) const -> Font;
  virtual auto group() const -> Group;
  auto offset() const -> int;
  auto parent() const -> mObject*;
  auto parentComboButton(bool recursive = false) const -> mComboButton*;
  auto parentComboEdit(bool recursive = false) const -> mComboEdit*;
  auto parentFrame(bool recursive = false) const -> mFrame*;
  auto parentIconView(bool recursive = false) const -> mIconView*;
  auto parentMenu(bool recursive = false) const -> mMenu*;
  auto parentMenuBar(bool recursive = false) const -> mMenuBar*;
  auto parentPopupMenu(bool recursive = false) const -> mPopupMenu*;
  auto parentSizable(bool recursive = false) const -> mSizable*;
  auto parentTabFrame(bool recursive = false) const -> mTabFrame*;
  auto parentTabFrameItem(bool recursive = false) const -> mTabFrameItem*;
  auto parentTableView(bool recursive = false) const -> mTableView*;
  auto parentTableViewItem(bool recursive = false) const -> mTableViewItem*;
  auto parentTreeView(bool recursive = false) const -> mTreeView*;
  auto parentTreeViewItem(bool recursive = false) const -> mTreeViewItem*;
  auto parentWidget(bool recursive = false) const -> mWidget*;
  auto parentWindow(bool recursive = false) const -> mWindow*;
  auto property(const string& name) const -> string;
  virtual auto remove() -> type&;
  virtual auto reset() -> type&;
  virtual auto setEnabled(bool enabled = true) -> type&;
  virtual auto setFocused() -> type&;
  virtual auto setFont(const Font& font = {}) -> type&;
  virtual auto setGroup(sGroup group = {}) -> type&;
  virtual auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;
  virtual auto setProperty(const string& name, const string& value = "") -> type&;
  virtual auto setVisible(bool visible = true) -> type&;
  auto visible(bool recursive = false) const -> bool;

//private:
//sizeof(mObject) == 88
  struct State {
    Font font;                  //32
    set<Property> properties;   //16
    mObject* parent = nullptr;  // 8
    int offset = -1;            // 4
    char enabled = true;        // 1+
    char visible = true;        // 1=4
  } state;

  wObject instance;             // 8
  pObject* delegate = nullptr;  // 8
  //vtable                      // 8

  virtual auto construct() -> void;
  virtual auto destruct() -> void;
};
#endif
