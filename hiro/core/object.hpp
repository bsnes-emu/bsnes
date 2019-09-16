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
  virtual auto remove() -> type&;
  virtual auto reset() -> type&;
  virtual auto setEnabled(bool enabled = true) -> type&;
  virtual auto setFocused() -> type&;
  virtual auto setFont(const Font& font = {}) -> type&;
  virtual auto setGroup(sGroup group = {}) -> type&;
  virtual auto setParent(mObject* parent = nullptr, int offset = -1) -> type&;
  virtual auto setVisible(bool visible = true) -> type&;
  auto visible(bool recursive = false) const -> bool;

  template<typename T = string> auto attribute(const string& name) const -> T {
    if(auto attribute = state.attributes.find(name)) {
      if(attribute->value().is<T>()) return attribute->value().get<T>();
    }
    return {};
  }

  //this template basically disables implicit template type deduction:
  //if setAttribute(name, value) is called without a type, the type will be a string, so attribute(name) will just work.
  //if setAttribute<T>(name, value) is called, the type will be T. as such, U must be cast to T on assignment.
  //when T = string, value must be convertible to a string.
  //U defaults to a string, so that setAttribute(name, {values, ...}) will deduce U as a string.
  template<typename T = string, typename U = string> auto setAttribute(const string& name, const U& value) -> type& {
    if constexpr(std::is_same_v<T, string> && !std::is_same_v<U, string>) {
      return setAttribute(name, string{value});
    }
    if(auto attribute = state.attributes.find(name)) {
      if((const T&)value) attribute->setValue((const T&)value);
      else state.attributes.remove(*attribute);
    } else {
      if((const T&)value) state.attributes.insert({name, (const T&)value});
    }
    return *this;
  }

//private:
  struct State {
    set<Attribute> attributes;
    Font font;
    mObject* parent = nullptr;
    int offset = -1;
    char enabled = true;
    char visible = true;
  } state;

  wObject instance;
  pObject* delegate = nullptr;

  virtual auto construct() -> void;
  virtual auto destruct() -> void;
};
#endif
