#define DeclareShared(Name) \
  using type = Name; \
  Name() : s##Name(new m##Name, [](auto p) { \
    p->unbind(); \
    delete p; \
  }) { \
    (*this)->bind(*this); \
  } \
  Name(std::nullptr_t) {} \
  Name(const s##Name& source) : s##Name(source) {} \
  explicit operator bool() const { return !empty(); } \
  auto self() const -> m##Name& { return (m##Name&)operator*(); } \

#define DeclareSharedObject(Name) \
  DeclareShared(Name) \
  template<typename T, typename... P> Name(T* parent, P&&... p) : Name() { \
    if(parent && *parent) (*parent)->append(*this, std::forward<P>(p)...); \
  } \
  auto enabled(bool recursive = false) const { return self().enabled(recursive); } \
  auto focused() const { return self().focused(); } \
  auto font(bool recursive = false) const { return self().font(recursive); } \
  auto offset() { return self().offset(); } \
  auto remove() { return self().remove(), *this; } \
  auto setEnabled(bool enabled = true) { return self().setEnabled(enabled), *this; } \
  auto setFocused() { return self().setFocused(), *this; } \
  auto setFont(const string& font = "") { return self().setFont(font), *this; } \
  auto setVisible(bool visible = true) { return self().setVisible(visible), *this; } \
  auto visible(bool recursive = false) const { return self().visible(recursive); } \

#define DeclareSharedAction(Name) \
  DeclareSharedObject(Name) \

#define DeclareSharedSizable(Name) \
  DeclareSharedObject(Name) \
  auto geometry() const { return self().geometry(); } \
  auto minimumSize() const { return self().minimumSize(); } \
  auto setGeometry(Geometry geometry) { return self().setGeometry(geometry), *this; } \

#define DeclareSharedLayout(Name) \
  DeclareSharedSizable(Name) \
  auto append(sSizable sizable) { return self().append(sizable), *this; } \
  auto remove(sSizable sizable) { return self().remove(sizable), *this; } \
  auto reset() { return self().reset(), *this; } \
  auto sizable(unsigned position) { return self().sizable(position); } \
  auto sizables() const { return self().sizables(); } \

#define DeclareSharedWidget(Name) \
  DeclareSharedSizable(Name) \
  auto doSize() const { return self().doSize(); } \
  auto onSize(const function<void ()>& callback = {}) { return self().onSize(callback), *this; } \

#if defined(Hiro_Object)
struct Object : sObject {
  DeclareSharedObject(Object)
};
#endif

#if defined(Hiro_Group)
struct Group : sGroup {
  DeclareShared(Group)
  template<typename... P> Group(P&&... p) : Group() { _append(std::forward<P>(p)...); }

  auto append(sObject object) -> type& { return self().append(object), *this; }
  auto object(unsigned position) const { return self().object(position); }
  auto objects() const { return self().objects(); }
  auto remove(sObject object) -> type& { return self().remove(object), *this; }

private:
  auto _append() {}
  template<typename T, typename... P> auto _append(T* object, P&&... p) {
    append(*object);
    _append(std::forward<P>(p)...);
  }
};
#endif

#if defined(Hiro_Hotkey)
struct Hotkey : sHotkey {
  DeclareSharedObject(Hotkey)

  auto doPress() const { return self().doPress(); }
  auto doRelease() const { return self().doRelease(); }
  auto onPress(const function<void ()>& callback = {}) { return self().onPress(callback), *this; }
  auto onRelease(const function<void ()>& callback = {}) { return self().onRelease(callback), *this; }
  auto parent() const { return self().parent(); }
  auto sequence() const { return self().sequence(); }
  auto setParent(sObject object) { return self().setParent(object), *this; }
  auto setSequence(const string& sequence = "") { return self().setSequence(sequence), *this; }
};
#endif

#if defined(Hiro_Timer)
struct Timer : sTimer {
  DeclareSharedObject(Timer)

  auto doActivate() const { return self().doActivate(); }
  auto interval() const { return self().interval(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto setInterval(unsigned interval = 0) { return self().setInterval(interval), *this; }
};
#endif

#if defined(Hiro_Action)
struct Action : sAction {
  DeclareSharedAction(Action)
};
#endif

#if defined(Hiro_Menu)
struct Menu : sMenu {
  DeclareSharedAction(Menu)

  auto action(unsigned position) const { return self().action(position); }
  auto actions() const { return self().actions(); }
  auto append(sAction action) { return self().append(action), *this; }
  auto icon() const { return self().icon(); }
  auto remove(sAction action) { return self().remove(action), *this; }
  auto reset() { return self().reset(), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_MenuSeparator)
struct MenuSeparator : sMenuSeparator {
  DeclareSharedAction(MenuSeparator)
};
#endif

#if defined(Hiro_MenuItem)
struct MenuItem : sMenuItem {
  DeclareSharedAction(MenuItem)

  auto doActivate() const { return self().doActivate(); }
  auto icon() const { return self().icon(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_MenuCheckItem)
struct MenuCheckItem : sMenuCheckItem {
  DeclareSharedAction(MenuCheckItem)

  auto checked() const { return self().checked(); }
  auto doToggle() const { return self().doToggle(); }
  auto onToggle(const function<void ()>& callback = {}) { return self().onToggle(callback), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_MenuRadioItem)
struct MenuRadioItem : sMenuRadioItem {
  DeclareSharedAction(MenuRadioItem)

  auto checked() const { return self().checked(); }
  auto doActivate() const { return self().doActivate(); }
  auto group() const { return self().group(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto setChecked() { return self().setChecked(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_Sizable)
struct Sizable : sSizable {
  DeclareSharedSizable(Sizable)
};
#endif

#if defined(Hiro_Layout)
struct Layout : sLayout {
  DeclareSharedLayout(Layout)
};
#endif

#if defined(Hiro_Widget)
struct Widget : sWidget {
  DeclareSharedWidget(Widget)
};
#endif

#if defined(Hiro_Button)
struct Button : sButton {
  DeclareSharedWidget(Button)

  auto bordered() const { return self().bordered(); }
  auto doActivate() const { return self().doActivate(); }
  auto icon() const { return self().icon(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto orientation() const { return self().orientation(); }
  auto setBordered(bool bordered = true) { return self().setBordered(bordered), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_Canvas)
struct Canvas : sCanvas {
  DeclareSharedWidget(Canvas)

  auto color() const { return self().color(); }
  auto data() { return self().data(); }
  auto droppable() const { return self().droppable(); }
  auto doDrop(lstring names) { return self().doDrop(names); }
  auto doMouseLeave() const { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const { return self().doMouseRelease(button); }
  auto gradient() const { return self().gradient(); }
  auto icon() const { return self().icon(); }
  auto onDrop(const function<void (lstring)>& callback = {}) { return self().onDrop(callback), *this; }
  auto onMouseLeave(const function<void ()>& callback = {}) { return self().onMouseLeave(callback), *this; }
  auto onMouseMove(const function<void (Position)>& callback = {}) { return self().onMouseMove(callback), *this; }
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) { return self().onMousePress(callback), *this; }
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) { return self().onMouseRelease(callback), *this; }
  auto setColor(Color color) { return self().setColor(color), *this; }
  auto setData(Size size) { return self().setData(size), *this; }
  auto setDroppable(bool droppable = true) { return self().setDroppable(droppable), *this; }
  auto setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) { return self().setGradient(topLeft, topRight, bottomLeft, bottomRight), *this; }
  auto setHorizontalGradient(Color left, Color right) { return self().setGradient(left, right, left, right), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setVerticalGradient(Color top, Color bottom) { return self().setGradient(top, top, bottom, bottom), *this; }
  auto size() const { return self().size(); }
  auto update() { return self().update(), *this; }
};
#endif

#if defined(Hiro_CheckButton)
struct CheckButton : sCheckButton {
  DeclareSharedWidget(CheckButton)

  auto bordered() const { return self().bordered(); }
  auto checked() const { return self().checked(); }
  auto doToggle() const { return self().doToggle(); }
  auto icon() const { return self().icon(); }
  auto onToggle(const function<void ()>& callback = {}) { return self().onToggle(callback), *this; }
  auto orientation() const { return self().orientation(); }
  auto setBordered(bool bordered = true) { return self().setBordered(bordered), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_CheckLabel)
struct CheckLabel : sCheckLabel {
  DeclareSharedWidget(CheckLabel)

  auto checked() const { return self().checked(); }
  auto doToggle() const { return self().doToggle(); }
  auto onToggle(const function<void ()>& callback = {}) { return self().onToggle(callback), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_ComboButton)
struct ComboButtonItem : sComboButtonItem {
  DeclareSharedObject(ComboButtonItem)

  auto icon() const { return self().icon(); }
  auto selected() const { return self().selected(); }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setSelected() { return self().setSelected(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_ComboButton)
struct ComboButton : sComboButton {
  DeclareSharedWidget(ComboButton)

  auto append(sComboButtonItem item) { return self().append(item), *this; }
  auto doChange() const { return self().doChange(); }
  auto item(unsigned position) const { return self().item(position); }
  auto items() const { return self().items(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto remove(sComboButtonItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setParent(mObject* parent = nullptr, signed offset = -1) { return self().setParent(parent, offset), *this; }
};
#endif

#if defined(Hiro_Console)
struct Console : sConsole {
  DeclareSharedWidget(Console)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto doActivate(string command) const { return self().doActivate(command); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto onActivate(const function<void (string)>& callback = {}) { return self().onActivate(callback), *this; }
  auto print(const string& text) { return self().print(text), *this; }
  auto prompt() const { return self().prompt(); }
  auto reset() { return self().reset(), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setPrompt(const string& prompt = "") { return self().setPrompt(prompt), *this; }
};
#endif

#if defined(Hiro_Frame)
struct Frame : sFrame {
  DeclareSharedWidget(Frame)

  auto append(sLayout layout) { return self().append(layout), *this; }
  auto layout() const { return self().layout(); }
  auto remove(sLayout layout) { return self().remove(layout), *this; }
  auto reset() { return self().reset(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_HexEdit)
struct HexEdit : sHexEdit {
  DeclareSharedWidget(HexEdit)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto columns() const { return self().columns(); }
  auto doRead(unsigned offset) const { return self().doRead(offset); }
  auto doWrite(unsigned offset, uint8_t data) const { return self().doWrite(offset, data); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto length() const { return self().length(); }
  auto offset() const { return self().offset(); }
  auto onRead(const function<uint8_t (unsigned)>& callback = {}) { return self().onRead(callback), *this; }
  auto onWrite(const function<void (unsigned, uint8_t)>& callback = {}) { return self().onWrite(callback), *this; }
  auto rows() const { return self().rows(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setColumns(unsigned columns = 16) { return self().setColumns(columns), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setLength(unsigned length) { return self().setLength(length), *this; }
  auto setOffset(unsigned offset) { return self().setOffset(offset), *this; }
  auto setRows(unsigned rows = 16) { return self().setRows(rows), *this; }
  auto update() { return self().update(), *this; }
};
#endif

#if defined(Hiro_HorizontalScroller)
struct HorizontalScroller : sHorizontalScroller {
  DeclareSharedWidget(HorizontalScroller)

  auto doChange() const { return self().doChange(); }
  auto length() const { return self().length(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto position() const { return self().position(); }
  auto setLength(unsigned length = 101) { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_HorizontalSlider)
struct HorizontalSlider : sHorizontalSlider {
  DeclareSharedWidget(HorizontalSlider)

  auto doChange() const { return self().doChange(); }
  auto length() const { return self().length(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto position() const { return self().position(); }
  auto setLength(unsigned length = 101) { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_IconView)
struct IconViewItem : sIconViewItem {
  DeclareSharedObject(IconViewItem)

  auto icon() const { return self().icon(); }
  auto selected() const { return self().selected(); }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setSelected(bool selected = true) { return self().setSelected(selected), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_IconView)
struct IconView : sIconView {
  DeclareSharedWidget(IconView)

  auto append(sIconViewItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto flow() const { return self().flow(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(unsigned position) const { return self().item(position); }
  auto items() const { return self().items(); }
  auto multiSelect() const { return self().multiSelect(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto orientation() const { return self().orientation(); }
  auto remove(sIconViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto selectedItems() const { return self().selectedItems(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setFlow(Orientation orientation = Orientation::Vertical) { return self().setFlow(orientation), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setMultiSelect(bool multiSelect = true) { return self().setMultiSelect(multiSelect), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) { return self().setOrientation(orientation), *this; }
  auto setSelected(const vector<signed>& selections) { return self().setSelected(selections), *this; }
};
#endif

#if defined(Hiro_Label)
struct Label : sLabel {
  DeclareSharedWidget(Label)

  auto horizontalAlignment() const { return self().horizontalAlignment(); }
  auto setHorizontalAlignment(double alignment = 0.0) { return self().setHorizontalAlignment(alignment), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto setVerticalAlignment(double alignment = 0.5) { return self().setVerticalAlignment(alignment), *this; }
  auto text() const { return self().text(); }
  auto verticalAlignment() const { return self().verticalAlignment(); }
};
#endif

#if defined(Hiro_LineEdit)
struct LineEdit : sLineEdit {
  DeclareSharedWidget(LineEdit)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto editable() const { return self().editable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_ListView)
struct ListViewColumn : sListViewColumn {
  DeclareSharedObject(ListViewColumn)

  auto active() const { return self().active(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto editable() const { return self().editable(); }
  auto expandable() const { return self().expandable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto horizontalAlignment() const { return self().horizontalAlignment(); }
  auto icon() const { return self().icon(); }
  auto resizable() const { return self().resizable(); }
  auto setActive() { return self().setActive(), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setExpandable(bool expandable = true) { return self().setExpandable(expandable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setHorizontalAlignment(double alignment = 0.0) { return self().setHorizontalAlignment(alignment), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setResizable(bool resizable = true) { return self().setResizable(resizable), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto setVerticalAlignment(double alignment = 0.5) { return self().setVerticalAlignment(alignment), *this; }
  auto setWidth(signed width = 0) { return self().setWidth(width), *this; }
  auto text() const { return self().text(); }
  auto verticalAlignment() const { return self().verticalAlignment(); }
  auto width() const { return self().width(); }
};
#endif

#if defined(Hiro_ListView)
struct ListViewCell : sListViewCell {
  DeclareSharedObject(ListViewCell)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto icon() const { return self().icon(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_ListView)
struct ListViewItem : sListViewItem {
  DeclareSharedObject(ListViewItem)

  auto append(sListViewCell cell) { return self().append(cell), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto cell(unsigned position) const { return self().cell(position); }
  auto cells() const { return self().cells(); }
  auto checked() const { return self().checked(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto remove(sListViewCell cell) { return self().remove(cell), *this; }
  auto selected() const { return self().selected(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setSelected(bool selected = true) { return self().setSelected(selected), *this; }
};
#endif

#if defined(Hiro_ListView)
struct ListView : sListView {
  DeclareSharedWidget(ListView)

  auto append(sListViewColumn column) { return self().append(column), *this; }
  auto append(sListViewItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto batchable() const { return self().batchable(); }
  auto checkable() const { return self().checkable(); }
  auto checkAll() { return self().checkAll(), *this; }
  auto checked() const { return self().checked(); }
  auto column(unsigned position) { return self().column(position); }
  auto columns() const { return self().columns(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto doEdit(sListViewCell cell) const { return self().doEdit(cell); }
  auto doSort(sListViewColumn column) const { return self().doSort(column); }
  auto doToggle(sListViewItem item) const { return self().doToggle(item); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto gridVisible() const { return self().gridVisible(); }
  auto headerVisible() const { return self().headerVisible(); }
  auto item(unsigned position) { return self().item(position); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto onEdit(const function<void (ListViewCell)>& callback = {}) { return self().onEdit(callback), *this; }
  auto onSort(const function<void (ListViewColumn)>& callback = {}) { return self().onSort(callback), *this; }
  auto onToggle(const function<void (ListViewItem)>& callback = {}) { return self().onToggle(callback), *this; }
  auto remove(sListViewColumn column) { return self().remove(column), *this; }
  auto remove(sListViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto resizeColumns() { return self().resizeColumns(), *this; }
  auto selectAll() { return self().selectAll(), *this; }
  auto selected() const { return self().selected(); }
  auto selectedItems() const { return self().selectedItems(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setBatchable(bool batchable = true) { return self().setBatchable(batchable), *this; }
  auto setCheckable(bool checkable = true) { return self().setCheckable(checkable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setGridVisible(bool visible = true) { return self().setGridVisible(visible), *this; }
  auto setHeaderVisible(bool visible = true) { return self().setHeaderVisible(visible), *this; }
  auto setSortable(bool sortable = true) { return self().setSortable(sortable), *this; }
  auto sortable() const { return self().sortable(); }
  auto uncheckAll() { return self().uncheckAll(), *this; }
  auto unselectAll() { return self().unselectAll(), *this; }
};
#endif

#if defined(Hiro_ProgressBar)
struct ProgressBar : sProgressBar {
  DeclareSharedWidget(ProgressBar)

  auto position() const { return self().position(); }
  auto setPosition(unsigned position = 0) { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_RadioButton)
struct RadioButton : sRadioButton {
  DeclareSharedWidget(RadioButton)

  auto bordered() const { return self().bordered(); }
  auto checked() const { return self().checked(); }
  auto doActivate() const { return self().doActivate(); }
  auto group() const { return self().group(); }
  auto icon() const { return self().icon(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto orientation() const { return self().orientation(); }
  auto setBordered(bool bordered = true) { return self().setBordered(bordered), *this; }
  auto setChecked() { return self().setChecked(), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) { return self().setOrientation(orientation), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_RadioLabel)
struct RadioLabel : sRadioLabel {
  DeclareSharedWidget(RadioLabel)

  auto checked() const { return self().checked(); }
  auto doActivate() const { return self().doActivate(); }
  auto group() const { return self().group(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto setChecked() { return self().setChecked(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_SourceEdit)
struct SourceEdit : sSourceEdit {
  DeclareSharedWidget(SourceEdit)

  auto doChange() const { return self().doChange(); }
  auto doMove() const { return self().doMove(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto position() const { return self().position(); }
  auto setPosition(signed position) { return self().setPosition(position), *this; }
  auto setSelected(Position selected) { return self().setSelected(selected), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_TabFrame)
struct TabFrameItem : sTabFrameItem {
  DeclareSharedObject(TabFrameItem)

  auto append(sLayout layout) { return self().append(layout), *this; }
  auto closable() const { return self().closable(); }
  auto icon() const { return self().icon(); }
  auto layout() const { return self().layout(); }
  auto movable() const { return self().movable(); }
  auto remove(sLayout layout) { return self().remove(layout), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setClosable(bool closable = true) { return self().setClosable(closable), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setMovable(bool movable = true) { return self().setMovable(movable), *this; }
  auto setSelected() { return self().setSelected(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_TabFrame)
struct TabFrame : sTabFrame {
  DeclareSharedWidget(TabFrame)

  auto append(sTabFrameItem item) { return self().append(item), *this; }
  auto doChange() const { return self().doChange(); }
  auto doClose(sTabFrameItem item) const { return self().doClose(item); }
  auto doMove(sTabFrameItem from, sTabFrameItem to) const { return self().doMove(from, to); }
  auto edge() const { return self().edge(); }
  auto item(unsigned position) const { return self().item(position); }
  auto items() const { return self().items(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onClose(const function<void (sTabFrameItem)>& callback = {}) { return self().onClose(callback), *this; }
  auto onMove(const function<void (sTabFrameItem, sTabFrameItem)>& callback = {}) { return self().onMove(callback), *this; }
  auto remove(sTabFrameItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setEdge(Edge edge = Edge::Top) { return self().setEdge(edge), *this; }
};
#endif

#if defined(Hiro_TextEdit)
struct TextEdit : sTextEdit {
  DeclareSharedWidget(TextEdit)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto cursorPosition() const { return self().cursorPosition(); }
  auto doChange() const { return self().doChange(); }
  auto doMove() const { return self().doMove(); }
  auto editable() const { return self().editable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCursorPosition(unsigned position) { return self().setCursorPosition(position), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto setWordWrap(bool wordWrap = true) { return self().setWordWrap(wordWrap), *this; }
  auto text() const { return self().text(); }
  auto wordWrap() const { return self().wordWrap(); }
};
#endif

#if defined(Hiro_TreeView)
struct TreeViewItem : sTreeViewItem {
  DeclareSharedObject(TreeViewItem)

  auto append(sTreeViewItem item) { return self().append(item), *this; }
  auto checked() const { return self().checked(); }
  auto icon() const { return self().icon(); }
  auto item(const string& path) const { return self().item(path); }
  auto items() const { return self().items(); }
  auto path() const { return self().path(); }
  auto remove(sTreeViewItem item) { return self().remove(item), *this; }
  auto selected() const { return self().selected(); }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setSelected() { return self().setSelected(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_TreeView)
struct TreeView : sTreeView {
  DeclareSharedWidget(TreeView)

  auto append(sTreeViewItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto checkable() const { return self().checkable(); }
  auto collapse() { return self().collapse(), *this; }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto doToggle(sTreeViewItem item) const { return self().doToggle(item); }
  auto expand() { return self().expand(), *this; }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(const string& path) const { return self().item(path); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto onToggle(const function<void (sTreeViewItem)>& callback = {}) { return self().onToggle(callback), *this; }
  auto remove(sTreeViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) { return self().setCheckable(checkable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
};
#endif

#if defined(Hiro_VerticalScroller)
struct VerticalScroller : sVerticalScroller {
  DeclareSharedWidget(VerticalScroller)

  auto doChange() const { return self().doChange(); }
  auto length() const { return self().length(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto position() const { return self().position(); }
  auto setLength(unsigned length = 101) { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_VerticalSlider)
struct VerticalSlider : sVerticalSlider {
  DeclareSharedWidget(VerticalSlider)

  auto doChange() const { return self().doChange(); }
  auto length() const { return self().length(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto position() const { return self().position(); }
  auto setLength(unsigned length = 101) { return self().setLength(length), *this; }
  auto setPosition(unsigned position = 0) { return self().setPosition(position), *this; }
};
#endif

#if defined(Hiro_Viewport)
struct Viewport : sViewport {
  DeclareSharedWidget(Viewport)

  auto doDrop(lstring names) const { return self().doDrop(names); }
  auto doMouseLeave() const { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const { return self().doMouseRelease(button); }
  auto droppable() const { return self().droppable(); }
  auto handle() const { return self().handle(); }
  auto onDrop(const function<void (lstring)>& callback = {}) { return self().onDrop(callback), *this; }
  auto onMouseLeave(const function<void ()>& callback = {}) { return self().onMouseLeave(callback), *this; }
  auto onMouseMove(const function<void (Position)>& callback = {}) { return self().onMouseMove(callback), *this; }
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) { return self().onMousePress(callback), *this; }
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) { return self().onMouseRelease(callback), *this; }
  auto setDroppable(bool droppable = true) { return self().setDroppable(droppable), *this; }
};
#endif

#if defined(Hiro_StatusBar)
struct StatusBar : sStatusBar {
  DeclareSharedObject(StatusBar)

  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_PopupMenu)
struct PopupMenu : sPopupMenu {
  DeclareSharedObject(PopupMenu)

  auto action(unsigned position) const { return self().action(position); }
  auto actions() const { return self().actions(); }
  auto append(sAction action) { return self().append(action), *this; }
  auto remove(sAction action) { return self().remove(action), *this; }
  auto reset() { return self().reset(), *this; }
};
#endif

#if defined(Hiro_MenuBar)
struct MenuBar : sMenuBar {
  DeclareSharedObject(MenuBar)

  auto append(sMenu menu) { return self().append(menu), *this; }
  auto menu(unsigned position) const { return self().menu(position); }
  auto menus() const { return self().menus(); }
  auto remove(sMenu menu) { return self().remove(menu), *this; }
  auto reset() { return self().reset(), *this; }
};
#endif

#if defined(Hiro_Window)
struct Window : sWindow {
  DeclareSharedObject(Window)

  auto append(sLayout layout) { return self().append(layout), *this; }
  auto append(sMenuBar menuBar) { return self().append(menuBar), *this; }
  auto append(sStatusBar statusBar) { return self().append(statusBar), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto doClose() const { return self().doClose(); }
  auto doDrop(lstring names) const { return self().doDrop(names); }
  auto doKeyPress(signed key) const { return self().doKeyPress(key); }
  auto doKeyRelease(signed key) const { return self().doKeyRelease(key); }
  auto doMove() const { return self().doMove(); }
  auto doSize() const { return self().doSize(); }
  auto droppable() const { return self().droppable(); }
  auto frameGeometry() const { return self().frameGeometry(); }
  auto fullScreen() const { return self().fullScreen(); }
  auto geometry() const { return self().geometry(); }
  auto layout() const { return self().layout(); }
  auto menuBar() const { return self().menuBar(); }
  auto modal() const { return self().modal(); }
  auto onClose(const function<void ()>& callback = {}) { return self().onClose(callback), *this; }
  auto onDrop(const function<void (lstring)>& callback = {}) { return self().onDrop(callback), *this; }
  auto onKeyPress(const function<void (signed)>& callback = {}) { return self().onKeyPress(callback), *this; }
  auto onKeyRelease(const function<void (signed)>& callback = {}) { return self().onKeyRelease(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto onSize(const function<void ()>& callback = {}) { return self().onSize(callback), *this; }
  auto remove(sLayout layout) { return self().remove(layout), *this; }
  auto remove(sMenuBar menuBar) { return self().remove(menuBar), *this; }
  auto remove(sStatusBar statusBar) { return self().remove(statusBar), *this; }
  auto reset() { return self().reset(), *this; }
  auto resizable() const { return self().resizable(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCentered(sWindow parent = {}) { return self().setCentered(parent), *this; }
  auto setDroppable(bool droppable = true) { return self().setDroppable(droppable), *this; }
  auto setFrameGeometry(Geometry geometry) { return self().setFrameGeometry(geometry), *this; }
  auto setFramePosition(Position position) { return self().setFramePosition(position), *this; }
  auto setFrameSize(Size size) { return self().setFrameSize(size), *this; }
  auto setFullScreen(bool fullScreen = true) { return self().setFullScreen(fullScreen), *this; }
  auto setGeometry(Geometry geometry) { return self().setGeometry(geometry), *this; }
  auto setModal(bool modal = true) { return self().setModal(modal), *this; }
  auto setPlacement(double x, double y) { return self().setPlacement(x, y), *this; }
  auto setPosition(Position position) { return self().setPosition(position), *this; }
  auto setResizable(bool resizable = true) { return self().setResizable(resizable), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto setTitle(const string& title = "") { return self().setTitle(title), *this; }
  auto statusBar() const { return self().statusBar(); }
  auto title() const { return self().title(); }
};
#endif
