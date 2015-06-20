#if defined(Hiro_Object)

mObject::mObject() {
  Application::initialize();
}

mObject::~mObject() {
}

auto mObject::allocate() -> pObject* {
  return new pObject(*this);
}

auto mObject::construct() -> void {
  if(delegate) return;
  delegate = allocate();
  signal(construct);
}

auto mObject::destruct() -> void {
  if(!delegate) return;
  signal(destruct);
  delete delegate;
  delegate = nullptr;
}

//

//used to test if returned items "exist" from eg Window::layout(), ListView::selected(), etc.
mObject::operator bool() const {
  return parent() || !abstract();
}

//this is used to control dynamic allocation of pObject delegates
//an mObject is abstract only if it has no top-level object (eg a Button not attached to any Window)
//if the mObject is not abstract, the pObject delegate is allocated immediately
//otherwise, the pObject is not allocated until it is attached to a non-abstract parent
auto mObject::abstract() const -> bool {
  #if defined(Hiro_Group)
  if(dynamic_cast<const mGroup*>(this)) return false;
  #endif

  #if defined(Hiro_Window)
  if(dynamic_cast<const mWindow*>(this)) return false;
  #endif

  #if defined(Hiro_PopupMenu)
  if(dynamic_cast<const mPopupMenu*>(this)) return false;
  #endif

  if(auto object = parent()) return object->abstract();
  return true;
}

auto mObject::adjustOffset(signed displacement) -> type& {
  state.offset += displacement;
  return *this;
}

auto mObject::enabled(bool recursive) const -> bool {
  if(!recursive || !state.enabled) return state.enabled;
  if(auto object = parent()) return object->enabled(true);
  return true;
}

auto mObject::focused() const -> bool {
  if(signal(focused)) return true;
  return false;
}

auto mObject::font(bool recursive) const -> string {
  if(!recursive || state.font) return state.font;
  if(auto object = parent()) return object->font(true);
  return Application::font();
}

auto mObject::group() const -> Group {
  return {};
}

auto mObject::offset() const -> signed {
  return state.offset;
}

auto mObject::parent() const -> mObject* {
  return state.parent;
}

#if defined(Hiro_ComboButton)
auto mObject::parentComboButton(bool recursive) const -> mComboButton* {
  if(auto comboButton = dynamic_cast<mComboButton*>(parent())) return comboButton;
  if(recursive) {
    if(auto object = parent()) return object->parentComboButton(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Frame)
auto mObject::parentFrame(bool recursive) const -> mFrame* {
  if(auto frame = dynamic_cast<mFrame*>(parent())) return frame;
  if(recursive) {
    if(auto object = parent()) return object->parentFrame(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_IconView)
auto mObject::parentIconView(bool recursive) const -> mIconView* {
  if(auto iconView = dynamic_cast<mIconView*>(parent())) return iconView;
  if(recursive) {
    if(auto object = parent()) return object->parentIconView(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Layout)
auto mObject::parentLayout(bool recursive) const -> mLayout* {
  if(auto layout = dynamic_cast<mLayout*>(parent())) return layout;
  if(recursive) {
    if(auto object = parent()) return object->parentLayout(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_ListView)
auto mObject::parentListView(bool recursive) const -> mListView* {
  if(auto listView = dynamic_cast<mListView*>(parent())) return listView;
  if(recursive) {
    if(auto object = parent()) return object->parentListView(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_ListView)
auto mObject::parentListViewItem(bool recursive) const -> mListViewItem* {
  if(auto listViewItem = dynamic_cast<mListViewItem*>(parent())) return listViewItem;
  if(recursive) {
    if(auto object = parent()) return object->parentListViewItem(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Menu)
auto mObject::parentMenu(bool recursive) const -> mMenu* {
  if(auto menu = dynamic_cast<mMenu*>(parent())) return menu;
  if(recursive) {
    if(auto object = parent()) return object->parentMenu(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_MenuBar)
auto mObject::parentMenuBar(bool recursive) const -> mMenuBar* {
  if(auto menuBar = dynamic_cast<mMenuBar*>(parent())) return menuBar;
  if(recursive) {
    if(auto object = parent()) return object->parentMenuBar(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_PopupMenu)
auto mObject::parentPopupMenu(bool recursive) const -> mPopupMenu* {
  if(auto popupMenu = dynamic_cast<mPopupMenu*>(parent())) return popupMenu;
  if(recursive) {
    if(auto object = parent()) return object->parentPopupMenu(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Sizable)
auto mObject::parentSizable(bool recursive) const -> mSizable* {
  if(auto sizable = dynamic_cast<mSizable*>(parent())) return sizable;
  if(recursive) {
    if(auto object = parent()) return object->parentSizable(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_TabFrame)
auto mObject::parentTabFrame(bool recursive) const -> mTabFrame* {
  if(auto tabFrame = dynamic_cast<mTabFrame*>(parent())) return tabFrame;
  if(recursive) {
    if(auto object = parent()) return object->parentTabFrame(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_TabFrame)
auto mObject::parentTabFrameItem(bool recursive) const -> mTabFrameItem* {
  if(auto tabFrameItem = dynamic_cast<mTabFrameItem*>(parent())) return tabFrameItem;
  if(recursive) {
    if(auto object = parent()) return object->parentTabFrameItem(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_TreeView)
auto mObject::parentTreeView(bool recursive) const -> mTreeView* {
  if(auto treeView = dynamic_cast<mTreeView*>(parent())) return treeView;
  if(recursive) {
    if(auto object = parent()) return object->parentTreeView(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_TreeView)
auto mObject::parentTreeViewItem(bool recursive) const -> mTreeViewItem* {
  if(auto treeViewItem = dynamic_cast<mTreeViewItem*>(parent())) return treeViewItem;
  if(recursive) {
    if(auto object = parent()) return object->parentTreeViewItem(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Widget)
auto mObject::parentWidget(bool recursive) const -> mWidget* {
  if(auto widget = dynamic_cast<mWidget*>(parent())) return widget;
  if(recursive) {
    if(auto object = parent()) return object->parentWidget(true);
  }
  return nullptr;
}
#endif

#if defined(Hiro_Window)
auto mObject::parentWindow(bool recursive) const -> mWindow* {
  if(auto window = dynamic_cast<mWindow*>(parent())) return window;
  if(recursive) {
    if(auto object = parent()) return object->parentWindow(true);
  }
  return nullptr;
}
#endif

auto mObject::remove() -> type& {
  signal(remove);
  return *this;
}

auto mObject::reset() -> type& {
  signal(reset);
  return *this;
}

auto mObject::setEnabled(bool enabled) -> type& {
  state.enabled = enabled;
  signal(setEnabled, this->enabled(true));
  return *this;
}

auto mObject::setFocused() -> type& {
  signal(setFocused);
  return *this;
}

auto mObject::setFont(const string& font) -> type& {
  state.font = font;
  signal(setFont, this->font(true));
  return *this;
}

auto mObject::setGroup(sGroup group) -> type& {
  return *this;
}

auto mObject::setParent(mObject* parent, signed offset) -> type& {
  destruct();
  state.parent = parent;
  state.offset = offset;
  if(!abstract()) construct();
  return *this;
}

auto mObject::setVisible(bool visible) -> type& {
  state.visible = visible;
  signal(setVisible, this->visible(true));
  return *this;
}

auto mObject::visible(bool recursive) const -> bool {
  if(!recursive || !state.visible) return state.visible;
  if(auto object = parent()) return object->visible(true);
  return true;
}

#endif
