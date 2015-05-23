#define Declare(Name) \
  using type = Name; \
  Name() : s##Name(new m##Name, [](m##Name* p) { \
    p->unbind(); \
    delete p; \
  }) { \
    (*this)->bind(*this); \
  } \
  template<typename T, typename... P> Name(T* parent, P&&... p) : Name() { \
    if(parent && *parent) (*parent)->append(*this, std::forward<P>(p)...); \
  } \
  auto self() const -> m##Name& { return (m##Name&)operator*(); } \

#define DeclareObject(Name) \
  Declare(Name) \
  auto enabled(bool recursive = false) const -> bool { return self().enabled(recursive); } \
  auto focused() const -> bool { return self().focused(); } \
  auto font(bool recursive = false) const -> string { return self().font(recursive); } \
  auto remove() -> type& { return self().remove(), *this; } \
  auto setEnabled(bool enabled = true) -> type& { return self().setEnabled(enabled), *this; } \
  auto setFocused() -> type& { return self().setFocused(), *this; } \
  auto setFont(const string& font = "") -> type& { return self().setFont(font), *this; } \
  auto setVisible(bool visible = true) -> type& { return self().setVisible(visible), *this; } \
  auto visible(bool recursive = false) const -> bool { return self().visible(recursive); } \

#define DeclareAction(Name) \
  DeclareObject(Name) \

#define DeclareSizable(Name) \
  DeclareObject(Name) \
  auto geometry() const -> Geometry { return self().geometry(); } \
  auto minimumSize() const -> Size { return self().minimumSize(); } \
  auto setGeometry(Geometry geometry) -> type& { return self().setGeometry(geometry), *this; } \

#define DeclareLayout(Name) \
  DeclareSizable(Name) \
  auto append(sSizable sizable) -> type& { return self().append(sizable), *this; } \
  auto remove(sSizable sizable) -> type& { return self().remove(sizable), *this; } \
  auto reset() -> type& { return self().reset(), *this; } \
  auto sizable(unsigned position) -> sSizable { return self().sizable(position); } \
  auto sizables() const -> unsigned { return self().sizables(); } \

#define DeclareWidget(Name) \
  DeclareSizable(Name) \
  auto doSize() const -> void { return self().doSize(); } \
  auto onSize(const function<void ()>& function = {}) -> type& { return self().onSize(function), *this; } \

#if defined(Hiro_Object)
struct Object : sObject {
  DeclareObject(Object)
};
#endif

#if defined(Hiro_Hotkey)
struct Hotkey : sHotkey {
  DeclareObject(Hotkey)

  auto doPress() const -> void { return self().doPress(); }
  auto doRelease() const -> void { return self().doRelease(); }
  auto onPress(const function<void ()>& function = {}) -> type& { return self().onPress(function), *this; }
  auto onRelease(const function<void ()>& function = {}) -> type& { return self().onRelease(function), *this; }
  auto parent() const -> wObject { return self().parent(); }
  auto sequence() const -> string { return self().sequence(); }
  auto setParent(sObject object) -> type& { return self().setParent(object), *this; }
  auto setSequence(const string& sequence = "") -> type& { return self().setSequence(sequence), *this; }
};
#endif

#if defined(Hiro_Timer)
struct Timer : sTimer {
  DeclareObject(Timer)

  auto doActivate() const -> void { return self().doActivate(); }
  auto interval() const -> unsigned { return self().interval(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto setInterval(unsigned interval = 0) -> type& { return self().setInterval(interval), *this; }
};
#endif

#if defined(Hiro_Window)
struct Window : sWindow {
  DeclareObject(Window)

  auto append(sLayout layout) -> type& { return self().append(layout), *this; }
  auto append(sMenuBar menuBar) -> type& { return self().append(menuBar), *this; }
  auto append(sStatusBar statusBar) -> type& { return self().append(statusBar), *this; }
  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto doClose() const -> void { return self().doClose(); }
  auto doDrop(lstring names) const -> void { return self().doDrop(names); }
  auto doKeyPress(signed key) const -> void { return self().doKeyPress(key); }
  auto doKeyRelease(signed key) const -> void { return self().doKeyRelease(key); }
  auto doMove() const -> void { return self().doMove(); }
  auto doSize() const -> void { return self().doSize(); }
  auto droppable() const -> bool { return self().droppable(); }
  auto frameGeometry() const -> Geometry { return self().frameGeometry(); }
  auto fullScreen() const -> bool { return self().fullScreen(); }
  auto geometry() const -> Geometry { return self().geometry(); }
  auto layout() const -> sLayout { return self().layout(); }
  auto menuBar() const -> sMenuBar { return self().menuBar(); }
  auto modal() const -> bool { return self().modal(); }
  auto onClose(const function<void ()>& function = {}) -> type& { return self().onClose(function), *this; }
  auto onDrop(const function<void (nall::lstring)>& function = {}) -> type& { return self().onDrop(function), *this; }
  auto onKeyPress(const function<void (signed)>& function = {}) -> type& { return self().onKeyPress(function), *this; }
  auto onKeyRelease(const function<void (signed)>& function = {}) -> type& { return self().onKeyRelease(function), *this; }
  auto onMove(const function<void ()>& function = {}) -> type& { return self().onMove(function), *this; }
  auto onSize(const function<void ()>& function = {}) -> type& { return self().onSize(function), *this; }
  auto remove(sLayout layout) -> type& { return self().remove(layout), *this; }
  auto remove(sMenuBar menuBar) -> type& { return self().remove(menuBar), *this; }
  auto remove(sStatusBar statusBar) -> type& { return self().remove(statusBar), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto resizable() const -> bool { return self().resizable(); }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setCentered() -> type& { return self().setCentered(), *this; }
  auto setCentered(sWindow parent) -> type& { return self().setCentered(parent), *this; }
  auto setDroppable(bool droppable = true) -> type& { return self().setDroppable(droppable), *this; }
  auto setFrameGeometry(Geometry geometry) -> type& { return self().setFrameGeometry(geometry), *this; }
  auto setFramePosition(Position position) -> type& { return self().setFramePosition(position), *this; }
  auto setFrameSize(Size size) -> type& { return self().setFrameSize(size), *this; }
  auto setFullScreen(bool fullScreen = true) -> type& { return self().setFullScreen(fullScreen), *this; }
  auto setGeometry(Geometry geometry) -> type& { return self().setGeometry(geometry), *this; }
  auto setModal(bool modal = true) -> type& { return self().setModal(modal), *this; }
  auto setPlacement(double x, double y) -> type& { return self().setPlacement(x, y), *this; }
  auto setPosition(Position position) -> type& { return self().setPosition(position), *this; }
  auto setResizable(bool resizable = true) -> type& { return self().setResizable(resizable), *this; }
  auto setSize(Size size) -> type& { return self().setSize(size), *this; }
  auto setTitle(const string& title = "") -> type& { return self().setTitle(title), *this; }
  auto statusBar() const -> sStatusBar { return self().statusBar(); }
  auto title() const -> string { return self().title(); }
};
#endif

#if defined(Hiro_StatusBar)
struct StatusBar : sStatusBar {
  DeclareObject(StatusBar)

  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_MenuBar)
struct MenuBar : sMenuBar {
  DeclareObject(MenuBar)

  auto append(sMenu menu) -> type& { return self().append(menu), *this; }
  auto menu(unsigned position) const -> sMenu { return self().menu(position); }
  auto menus() const -> unsigned { return self().menus(); }
  auto remove(sMenu menu) -> type& { return self().remove(menu), *this; }
  auto reset() -> type& { return self().reset(), *this; }
};
#endif

#if defined(Hiro_PopupMenu)
struct PopupMenu : sPopupMenu {
  DeclareObject(PopupMenu)

  auto action(unsigned position) const -> sAction { return self().action(position); }
  auto actions() const -> unsigned { return self().actions(); }
  auto append(sAction action) -> type& { return self().append(action), *this; }
  auto remove(sAction action) -> type& { return self().remove(action), *this; }
  auto reset() -> type& { return self().reset(), *this; }
};
#endif

#if defined(Hiro_Action)
struct Action : sAction {
  DeclareAction(Action)
};
#endif

#if defined(Hiro_Menu)
struct Menu : sMenu {
  DeclareAction(Menu)

  auto action(unsigned position) const -> sAction { return self().action(position); }
  auto actions() const -> unsigned { return self().actions(); }
  auto append(sAction action) -> type& { return self().append(action), *this; }
  auto icon() const -> image { return self().icon(); }
  auto remove(sAction action) -> type& { return self().remove(action), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_MenuSeparator)
struct MenuSeparator : sMenuSeparator {
  DeclareAction(MenuSeparator)
};
#endif

#if defined(Hiro_MenuItem)
struct MenuItem : sMenuItem {
  DeclareAction(MenuItem)

  auto doActivate() const -> void { return self().doActivate(); }
  auto icon() const -> image { return self().icon(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_MenuCheckItem)
struct MenuCheckItem : sMenuCheckItem {
  DeclareAction(MenuCheckItem)

  auto checked() const -> bool { return self().checked(); }
  auto doToggle() const -> void { return self().doToggle(); }
  auto onToggle(const function<void ()>& function = {}) -> type& { return self().onToggle(function), *this; }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_MenuRadioItem)
struct MenuRadioItem : sMenuRadioItem {
  DeclareAction(MenuRadioItem)

  auto checked() const -> bool { return self().checked(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto setChecked() -> type& { return self().setChecked(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }

  static auto group(const vector<MenuRadioItem>& group) -> void {
    vector<wMenuRadioItem> items;
    for(auto& item : group) items.append(item);
    return mMenuRadioItem::group(items);
  }
};
#endif

#if defined(Hiro_Sizable)
struct Sizable : sSizable {
  DeclareSizable(Sizable)
};
#endif

#if defined(Hiro_Layout)
struct Layout : sLayout {
  DeclareLayout(Layout)
};
#endif

#if defined(Hiro_Widget)
struct Widget : sWidget {
  DeclareWidget(Widget)
};
#endif

#if defined(Hiro_Button)
struct Button : sButton {
  DeclareWidget(Button)

  auto bordered() const -> bool { return self().bordered(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto icon() const -> image { return self().icon(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto orientation() const -> Orientation { return self().orientation(); }
  auto setBordered(bool bordered = true) -> type& { return self().setBordered(bordered), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type& { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_Canvas)
struct Canvas : sCanvas {
  DeclareWidget(Canvas)

  auto color() const -> Color { return self().color(); }
  auto data() -> uint32_t* { return self().data(); }
  auto droppable() const -> bool { return self().droppable(); }
  auto doDrop(lstring names) -> void { return self().doDrop(names); }
  auto doMouseLeave() const -> void { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const -> void { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const -> void { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const -> void { return self().doMouseRelease(button); }
  auto gradient() const -> vector<Color> { return self().gradient(); }
  auto icon() const -> image { return self().icon(); }
  auto onDrop(const function<void (lstring)>& function = {}) -> type& { return self().onDrop(function), *this; }
  auto onMouseLeave(const function<void ()>& function = {}) -> type& { return self().onMouseLeave(function), *this; }
  auto onMouseMove(const function<void (Position)>& function = {}) -> type& { return self().onMouseMove(function), *this; }
  auto onMousePress(const function<void (Mouse::Button)>& function = {}) -> type& { return self().onMousePress(function), *this; }
  auto onMouseRelease(const function<void (Mouse::Button)>& function = {}) -> type& { return self().onMouseRelease(function), *this; }
  auto setColor(Color color) -> type& { return self().setColor(color), *this; }
  auto setData(Size size) -> type& { return self().setData(size), *this; }
  auto setDroppable(bool droppable = true) -> type& { return self().setDroppable(droppable), *this; }
  auto setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> type& { return self().setGradient(topLeft, topRight, bottomLeft, bottomRight), *this; }
  auto setHorizontalGradient(Color left, Color right) -> type& { return self().setGradient(left, right, left, right), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setVerticalGradient(Color top, Color bottom) -> type& { return self().setGradient(top, top, bottom, bottom), *this; }
  auto size() const -> Size { return self().size(); }
  auto update() -> type& { return self().update(), *this; }
};
#endif

#if defined(Hiro_CheckButton)
struct CheckButton : sCheckButton {
  DeclareWidget(CheckButton)

  auto bordered() const -> bool { return self().bordered(); }
  auto checked() const -> bool { return self().checked(); }
  auto doToggle() const -> void { return self().doToggle(); }
  auto icon() const -> image { return self().icon(); }
  auto onToggle(const function<void ()>& function = {}) -> type& { return self().onToggle(function), *this; }
  auto orientation() const -> Orientation { return self().orientation(); }
  auto setBordered(bool bordered = true) -> type& { return self().setBordered(bordered), *this; }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type& { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_CheckLabel)
struct CheckLabel : sCheckLabel {
  DeclareWidget(CheckLabel)

  auto checked() const -> bool { return self().checked(); }
  auto doToggle() const -> void { return self().doToggle(); }
  auto onToggle(const function<void ()>& function = {}) -> type& { return self().onToggle(function), *this; }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_ComboButton)
struct ComboButton : sComboButton {
  DeclareWidget(ComboButton)

  auto append(sComboButtonItem item) -> type& { return self().append(item), *this; }
  auto doChange() const -> void { return self().doChange(); }
  auto item(unsigned position) const -> sComboButtonItem { return self().item(position); }
  auto items() const -> unsigned { return self().items(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto remove(sComboButtonItem item) -> type& { return self().remove(item), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto selected() const -> sComboButtonItem { return self().selected(); }
};
#endif

#if defined(Hiro_ComboButton)
struct ComboButtonItem : sComboButtonItem {
  DeclareObject(ComboButtonItem)

  auto icon() const -> image { return self().icon(); }
  auto selected() const -> bool { return self().selected(); }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setSelected() -> type& { return self().setSelected(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_Console)
struct Console : sConsole {
  DeclareWidget(Console)

  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto doActivate(string command) const -> void { return self().doActivate(command); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto onActivate(const function<void (string)>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto print(const string& text) -> type& { return self().print(text), *this; }
  auto prompt() const -> string { return self().prompt(); }
  auto reset() -> type& { return self().reset(), *this; }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setPrompt(const string& prompt = "") -> type& { return self().setPrompt(prompt), *this; }
};
#endif

#if defined(Hiro_Frame)
struct Frame : sFrame {
  DeclareWidget(Frame)

  auto append(sLayout layout) -> type& { return self().append(layout), *this; }
  auto layout() const -> sLayout { return self().layout(); }
  auto remove(sLayout layout) -> type& { return self().remove(layout), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_HexEdit)
struct HexEdit : sHexEdit {
  DeclareWidget(HexEdit)

  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto columns() const -> unsigned { return self().columns(); }
  auto doRead(unsigned offset) const -> uint8_t { return self().doRead(offset); }
  auto doWrite(unsigned offset, uint8_t data) const -> void { return self().doWrite(offset, data); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto length() const -> unsigned { return self().length(); }
  auto offset() const -> unsigned { return self().offset(); }
  auto onRead(const function<uint8_t (unsigned)>& function = {}) -> type& { return self().onRead(function), *this; }
  auto onWrite(const function<void (unsigned, uint8_t)>& function = {}) -> type& { return self().onWrite(function), *this; }
  auto rows() const -> unsigned { return self().rows(); }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setColumns(unsigned columns = 16) -> type& { return self().setColumns(columns), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setLength(unsigned length) -> type& { return self().setLength(length), *this; }
  auto setOffset(unsigned offset) -> type& { return self().setOffset(offset), *this; }
  auto setRows(unsigned rows = 16) -> type& { return self().setRows(rows), *this; }
  auto update() -> type& { return self().update(), *this; }
};
#endif

#if defined(Hiro_HorizontalScroller)
struct HorizontalScroller : sHorizontalScroller {
  DeclareWidget(HorizontalScroller)

  auto doChange() const -> void { return self().doChange(); }
  auto length() const -> unsigned { return self().length(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto position() const -> unsigned { return self().position(); }
  auto setLength(unsigned length = 101) -> type& { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) -> type& { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_HorizontalSlider)
struct HorizontalSlider : sHorizontalSlider {
  DeclareWidget(HorizontalSlider)

  auto doChange() const -> void { return self().doChange(); }
  auto length() const -> unsigned { return self().length(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto position() const -> unsigned { return self().position(); }
  auto setLength(unsigned length = 101) -> type& { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) -> type& { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_IconView)
struct IconView : sIconView {
  DeclareWidget(IconView)

  auto append(sIconViewItem item) -> type& { return self().append(item), *this; }
  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto doChange() const -> void { return self().doChange(); }
  auto doContext() const -> void { return self().doContext(); }
  auto flow() const -> Orientation { return self().flow(); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto item(unsigned position) const -> sIconViewItem { return self().item(position); }
  auto items() const -> unsigned { return self().items(); }
  auto multiSelect() const -> bool { return self().multiSelect(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onContext(const function<void ()>& function = {}) -> type& { return self().onContext(function), *this; }
  auto orientation() const -> Orientation { return self().orientation(); }
  auto remove(sIconViewItem item) -> type& { return self().remove(item), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto selected() const -> maybe<unsigned> { return self().selected(); }
  auto selectedItems() const -> vector<unsigned> { return self().selectedItems(); }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setFlow(Orientation orientation = Orientation::Vertical) -> type& { return self().setFlow(orientation), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setMultiSelect(bool multiSelect = true) -> type& { return self().setMultiSelect(multiSelect), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type& { return self().setOrientation(orientation), *this; }
  auto setSelected(const vector<signed>& selections) -> type& { return self().setSelected(selections), *this; }
};
#endif

#if defined(Hiro_IconView)
struct IconViewItem : sIconViewItem {
  DeclareObject(IconViewItem)

  auto icon() const -> image { return self().icon(); }
  auto selected() const -> bool { return self().selected(); }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setSelected(bool selected = true) -> type& { return self().setSelected(selected), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_Label)
struct Label : sLabel {
  DeclareWidget(Label)

  auto horizontalAlignment() const -> double { return self().horizontalAlignment(); }
  auto setHorizontalAlignment(double alignment = 0.0) -> type& { return self().setHorizontalAlignment(alignment), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto setVerticalAlignment(double alignment = 0.5) -> type& { return self().setVerticalAlignment(alignment), *this; }
  auto text() const -> string { return self().text(); }
  auto verticalAlignment() const -> double { return self().verticalAlignment(); }
};
#endif

#if defined(Hiro_LineEdit)
struct LineEdit : sLineEdit {
  DeclareWidget(LineEdit)

  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto doChange() const -> void { return self().doChange(); }
  auto editable() const -> bool { return self().editable(); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) -> type& { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_ListView)
struct ListView : sListView {
  DeclareWidget(ListView)

  auto append(sListViewColumn column) -> type& { return self().append(column), *this; }
  auto append(sListViewItem item) -> type& { return self().append(item), *this; }
  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto checkable() const -> bool { return self().checkable(); }
  auto checked() const -> vector<sListViewItem> { return self().checked(); }
  auto column(unsigned position) -> sListViewColumn { return self().column(position); }
  auto columns() const -> unsigned { return self().columns(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto doChange() const -> void { return self().doChange(); }
  auto doContext() const -> void { return self().doContext(); }
  auto doEdit(sListViewItem item, sListViewColumn column) const -> void { return self().doEdit(item, column); }
  auto doSort(sListViewColumn column) const -> void { return self().doSort(column); }
  auto doToggle(sListViewItem item) const -> void { return self().doToggle(item); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto gridVisible() const -> bool { return self().gridVisible(); }
  auto headerVisible() const -> bool { return self().headerVisible(); }
  auto item(unsigned position) -> sListViewItem { return self().item(position); }
  auto items() const -> unsigned { return self().items(); }
  auto multiSelect() const -> bool { return self().multiSelect(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onContext(const function<void ()>& function = {}) -> type& { return self().onContext(function), *this; }
  auto onEdit(const function<void (sListViewItem, sListViewColumn)>& function = {}) -> type& { return self().onEdit(function), *this; }
  auto onSort(const function<void (sListViewColumn)>& function = {}) -> type& { return self().onSort(function), *this; }
  auto onToggle(const function<void (sListViewItem)>& function = {}) -> type& { return self().onToggle(function), *this; }
  auto remove(sListViewColumn column) -> type& { return self().remove(column), *this; }
  auto remove(sListViewItem item) -> type& { return self().remove(item), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto resizeColumns() -> type& { return self().resizeColumns(), *this; }
  auto selected() const -> sListViewItem { return self().selected(); }
  auto selectedItems() const -> vector<sListViewItem> { return self().selectedItems(); }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) -> type& { return self().setCheckable(checkable), *this; }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setGridVisible(bool visible = true) -> type& { return self().setGridVisible(visible), *this; }
  auto setHeaderVisible(bool visible = true) -> type& { return self().setHeaderVisible(visible), *this; }
  auto setMultiSelect(bool multiSelect = true) -> type& { return self().setMultiSelect(multiSelect), *this; }
  auto setSelected(bool selected = true) -> type& { return self().setSelected(selected), *this; }
};
#endif

#if defined(Hiro_ListView)
struct ListViewColumn : sListViewColumn {
  DeclareObject(ListViewColumn)

  auto active() const -> bool { return self().active(); }
  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto editable() const -> bool { return self().editable(); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto horizontalAlignment() const -> double { return self().horizontalAlignment(); }
  auto icon() const -> image { return self().icon(); }
  auto resizable() const -> bool { return self().resizable(); }
  auto setActive() -> type& { return self().setActive(), *this; }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) -> type& { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setHorizontalAlignment(double alignment = 0.0) -> type& { return self().setHorizontalAlignment(alignment), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setResizable(bool resizable = true) -> type& { return self().setResizable(resizable), *this; }
  auto setSortable(bool sortable = true) -> type& { return self().setSortable(sortable), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto setVerticalAlignment(double alignment = 0.5) -> type& { return self().setVerticalAlignment(alignment), *this; }
  auto setWidth(signed width = 0) -> type& { return self().setWidth(width), *this; }
  auto sortable() const -> bool { return self().sortable(); }
  auto text() const -> string { return self().text(); }
  auto verticalAlignment() const -> double { return self().verticalAlignment(); }
  auto width() const -> signed { return self().width(); }
};
#endif

#if defined(Hiro_ListView)
struct ListViewItem : sListViewItem {
  DeclareObject(ListViewItem)

  auto checked() const -> bool { return self().checked(); }
  auto icon(unsigned column = 0) const -> image { return self().icon(column); }
  auto selected() const -> bool { return self().selected(); }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setIcon(unsigned column, const image& icon = {}) -> type& { return self().setIcon(column, icon), *this; }
  auto setSelected(bool selected = true) -> type& { return self().setSelected(selected), *this; }
  auto setText(const lstring& text) -> type& { return self().setText(text), *this; }
  auto setText(unsigned column, const string& text = "") -> type& { return self().setText(column, text), *this; }
  auto text(unsigned column = 0) const -> string { return self().text(column); }
};
#endif

#if defined(Hiro_ProgressBar)
struct ProgressBar : sProgressBar {
  DeclareWidget(ProgressBar)

  auto position() const -> unsigned { return self().position(); }
  auto setPosition(unsigned position = 0) -> type& { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_RadioButton)
struct RadioButton : sRadioButton {
  DeclareWidget(RadioButton)

  auto bordered() const -> bool { return self().bordered(); }
  auto checked() const -> bool { return self().checked(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto icon() const -> image { return self().icon(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto orientation() const -> Orientation { return self().orientation(); }
  auto setBordered(bool bordered = true) -> type& { return self().setBordered(bordered), *this; }
  auto setChecked() -> type& { return self().setChecked(), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) -> type& { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }

  static auto group(const vector<RadioButton>& group) -> void {
    vector<wRadioButton> items;
    for(auto& item : group) items.append(item);
    return mRadioButton::group(items);
  }
};
#endif

#if defined(Hiro_RadioLabel)
struct RadioLabel : sRadioLabel {
  DeclareWidget(RadioLabel)

  auto checked() const -> bool { return self().checked(); }
  auto doActivate() const -> void { return self().doActivate(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto setChecked() -> type& { return self().setChecked(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }

  static auto group(const vector<RadioLabel>& group) -> void {
    vector<wRadioLabel> items;
    for(auto& item : items) items.append(item);
    return mRadioLabel::group(items);
  }
};
#endif

#if defined(Hiro_SourceEdit)
struct SourceEdit : sSourceEdit {
  DeclareWidget(SourceEdit)

  auto doChange() const -> void { return self().doChange(); }
  auto doMove() const -> void { return self().doMove(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onMove(const function<void ()>& function = {}) -> type& { return self().onMove(function), *this; }
  auto position() const -> unsigned { return self().position(); }
  auto setPosition(signed position) -> type& { return self().setPosition(position), *this; }
  auto setSelected(Position selected) -> type& { return self().setSelected(selected), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_TabFrame)
struct TabFrame : sTabFrame {
  DeclareWidget(TabFrame)

  auto append(sTabFrameItem item) -> type& { return self().append(item), *this; }
  auto doChange() const -> void { return self().doChange(); }
  auto doClose(sTabFrameItem item) const -> void { return self().doClose(item); }
  auto doMove(sTabFrameItem from, sTabFrameItem to) const -> void { return self().doMove(from, to); }
  auto edge() const -> Edge;
  auto item(unsigned position) const -> sTabFrameItem { return self().item(position); }
  auto items() const -> unsigned { return self().items(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onClose(const function<void (sTabFrameItem)>& function = {}) -> type& { return self().onClose(function), *this; }
  auto onMove(const function<void (sTabFrameItem, sTabFrameItem)>& function = {}) -> type& { return self().onMove(function), *this; }
  auto remove(sTabFrameItem item) -> type& { return self().remove(item), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto selected() const -> sTabFrameItem { return self().selected(); }
  auto setEdge(Edge edge = Edge::Top) -> type& { return self().setEdge(edge), *this; }
};
#endif

#if defined(Hiro_TabFrame)
struct TabFrameItem : sTabFrameItem {
  DeclareObject(TabFrameItem)

  auto append(sLayout layout) -> type& { return self().append(layout), *this; }
  auto closable() const -> bool { return self().closable(); }
  auto icon() const -> image { return self().icon(); }
  auto layout() const -> sLayout { return self().layout(); }
  auto movable() const -> bool { return self().movable(); }
  auto remove(sLayout layout) -> type& { return self().remove(layout), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto selected() const -> bool { return self().selected(); }
  auto setClosable(bool closable = true) -> type& { return self().setClosable(closable), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setMovable(bool movable = true) -> type& { return self().setMovable(movable), *this; }
  auto setSelected() -> type& { return self().setSelected(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_TextEdit)
struct TextEdit : sTextEdit {
  DeclareWidget(TextEdit)

  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto cursorPosition() const -> unsigned { return self().cursorPosition(); }
  auto doChange() const -> void { return self().doChange(); }
  auto doMove() const -> void { return self().doMove(); }
  auto editable() const -> bool { return self().editable(); }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onMove(const function<void ()>& function = {}) -> type& { return self().onMove(function), *this; }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setCursorPosition(unsigned position) -> type& { return self().setCursorPosition(position), *this; }
  auto setEditable(bool editable = true) -> type& { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto setWordWrap(bool wordWrap = true) -> type& { return self().setWordWrap(wordWrap), *this; }
  auto text() const -> string { return self().text(); }
  auto wordWrap() const -> bool { return self().wordWrap(); }
};
#endif

#if defined(Hiro_TreeView)
struct TreeView : sTreeView {
  DeclareWidget(TreeView)

  auto append(sTreeViewItem item) -> type& { return self().append(item), *this; }
  auto backgroundColor() const -> Color { return self().backgroundColor(); }
  auto checkable() const -> bool { return self().checkable(); }
  auto collapse() -> type& { return self().collapse(), *this; }
  auto doActivate() const -> void { return self().doActivate(); }
  auto doChange() const -> void { return self().doChange(); }
  auto doContext() const -> void { return self().doContext(); }
  auto doToggle(sTreeViewItem item) const -> void { return self().doToggle(item); }
  auto expand() -> type& { return self().expand(), *this; }
  auto foregroundColor() const -> Color { return self().foregroundColor(); }
  auto item(const string& path) const -> sTreeViewItem { return self().item(path); }
  auto items() const -> unsigned { return self().items(); }
  auto onActivate(const function<void ()>& function = {}) -> type& { return self().onActivate(function), *this; }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto onContext(const function<void ()>& function = {}) -> type& { return self().onContext(function), *this; }
  auto onToggle(const function<void (sTreeViewItem)>& function = {}) -> type& { return self().onToggle(function), *this; }
  auto remove(sTreeViewItem item) -> type& { return self().remove(item), *this; }
  auto reset() -> type& { return self().reset(), *this; }
  auto selected() const -> sTreeViewItem { return self().selected(); }
  auto setBackgroundColor(Color color = {}) -> type& { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) -> type& { return self().setCheckable(checkable), *this; }
  auto setForegroundColor(Color color = {}) -> type& { return self().setForegroundColor(color), *this; }
};
#endif

#if defined(Hiro_TreeView)
struct TreeViewItem : sTreeViewItem {
  DeclareObject(TreeViewItem)

  auto append(sTreeViewItem item) -> type& { return self().append(item), *this; }
  auto checked() const -> bool { return self().checked(); }
  auto icon() const -> image { return self().icon(); }
  auto item(const string& path) const -> sTreeViewItem { return self().item(path); }
  auto items() const -> unsigned { return self().items(); }
  auto path() const -> string { return self().path(); }
  auto remove(sTreeViewItem item) -> type& { return self().remove(item), *this; }
  auto selected() const -> bool { return self().selected(); }
  auto setChecked(bool checked = true) -> type& { return self().setChecked(checked), *this; }
  auto setIcon(const image& icon = {}) -> type& { return self().setIcon(icon), *this; }
  auto setSelected() -> type& { return self().setSelected(), *this; }
  auto setText(const string& text = "") -> type& { return self().setText(text), *this; }
  auto text() const -> string { return self().text(); }
};
#endif

#if defined(Hiro_VerticalScroller)
struct VerticalScroller : sVerticalScroller {
  DeclareWidget(VerticalScroller)

  auto doChange() const -> void { return self().doChange(); }
  auto length() const -> unsigned { return self().length(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto position() const -> unsigned { return self().position(); }
  auto setLength(unsigned length = 101) -> type& { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) -> type& { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_VerticalSlider)
struct VerticalSlider : sVerticalSlider {
  DeclareWidget(VerticalSlider)

  auto doChange() const -> void { return self().doChange(); }
  auto length() const -> unsigned { return self().length(); }
  auto onChange(const function<void ()>& function = {}) -> type& { return self().onChange(function), *this; }
  auto position() const -> unsigned { return self().position(); }
  auto setLength(unsigned length = 101) -> type& { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) -> type& { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_Viewport)
struct Viewport : sViewport {
  DeclareWidget(Viewport)

  auto doDrop(lstring names) const -> void { return self().doDrop(names); }
  auto doMouseLeave() const -> void { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const -> void { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const -> void { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const -> void { return self().doMouseRelease(button); }
  auto droppable() const -> bool { return self().droppable(); }
  auto handle() const -> uintptr_t { return self().handle(); }
  auto onDrop(const function<void (lstring)>& function = {}) -> type& { return self().onDrop(function), *this; }
  auto onMouseLeave(const function<void ()>& function = {}) -> type& { return self().onMouseLeave(function), *this; }
  auto onMouseMove(const function<void (Position)>& function = {}) -> type& { return self().onMouseMove(function), *this; }
  auto onMousePress(const function<void (Mouse::Button)>& function = {}) -> type& { return self().onMousePress(function), *this; }
  auto onMouseRelease(const function<void (Mouse::Button)>& function = {}) -> type& { return self().onMouseRelease(function), *this; }
  auto setDroppable(bool droppable = true) -> type& { return self().setDroppable(droppable), *this; }
};
#endif

#if defined(Hiro_FixedLayout)
using sFixedLayout = shared_pointer<mFixedLayout>;
struct FixedLayout : sFixedLayout {
  DeclareLayout(FixedLayout)

  auto append(sSizable sizable, Geometry geometry) -> type& { return self().append(sizable, geometry), *this; }
};
#endif

#if defined(Hiro_HorizontalLayout)
using sHorizontalLayout = shared_pointer<mHorizontalLayout>;
struct HorizontalLayout : sHorizontalLayout {
  DeclareLayout(HorizontalLayout)

  auto append(sSizable sizable, Size size, signed spacing = 5) -> type& { return self().append(sizable, size, spacing), *this; }
  auto setAlignment(double alignment = 0.5) -> type& { return self().setAlignment(alignment), *this; }
  auto setMargin(signed margin = 0) -> type& { return self().setMargin(margin), *this; }
  auto setSpacing(signed spacing = 5) -> type& { return self().setSpacing(spacing), *this; }
};
#endif

#if defined(Hiro_VerticalLayout)
using sVerticalLayout = shared_pointer<mVerticalLayout>;
struct VerticalLayout : sVerticalLayout {
  DeclareLayout(VerticalLayout)

  auto append(sSizable sizable, Size size, signed spacing = 5) -> type& { return self().append(sizable, size, spacing), *this; }
  auto setAlignment(double alignment = 0.0) -> type& { return self().setAlignment(alignment), *this; }
  auto setMargin(signed margin = 0) -> type& { return self().setMargin(margin), *this; }
  auto setSpacing(signed spacing = 5) -> type& { return self().setSpacing(spacing), *this; }
};
#endif

#undef Declare
#undef DeclareObject
#undef DeclareAction
#undef DeclareSizable
#undef DeclareLayout
#undef DeclareWidget
