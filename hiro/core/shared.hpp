#define DeclareShared(Name) \
  using type = Name; \
  using internalType = m##Name; \
  Name() : s##Name(new m##Name, [](auto p) { \
    p->unbind(); \
    delete p; \
  }) { \
    (*this)->bind(*this); \
  } \
  Name(const s##Name& source) : s##Name(source) { assert(source); } \
  template<typename T> Name(const T& source, \
  std::enable_if_t<std::is_base_of<internalType, typename T::internalType>::value>* = 0) : \
  s##Name((s##Name&)source) { assert(source); } \
  explicit operator bool() const { return self().operator bool(); } \
  auto self() const -> m##Name& { return (m##Name&)operator*(); } \

#define DeclareSharedObject(Name) \
  DeclareShared(Name) \
  template<typename T, typename... P> Name(T* parent, P&&... p) : Name() { \
    if(parent) (*parent)->append(*this, std::forward<P>(p)...); \
  } \
  template<typename T> auto is() -> bool { \
    return dynamic_cast<typename T::internalType*>(data()); \
  } \
  template<typename T> auto cast() -> T { \
    if(auto pointer = dynamic_cast<typename T::internalType*>(data())) { \
      if(auto shared = pointer->instance.acquire()) return T(shared); \
    } \
    return T(); \
  } \
  auto enabled(bool recursive = false) const { return self().enabled(recursive); } \
  auto focused() const { return self().focused(); } \
  auto font(bool recursive = false) const { return self().font(recursive); } \
  auto offset() const { return self().offset(); } \
  auto parent() const { \
    if(auto object = self().parent()) { \
      if(auto instance = object->instance.acquire()) return Object(instance); \
    } \
    return Object(); \
  } \
  auto property(const string& name) const { return self().property(name); } \
  auto remove() { return self().remove(), *this; } \
  auto setEnabled(bool enabled = true) { return self().setEnabled(enabled), *this; } \
  auto setFocused() { return self().setFocused(), *this; } \
  auto setFont(const Font& font = {}) { return self().setFont(font), *this; } \
  auto setProperty(const string& name, const string& value = "") { return self().setProperty(name, value), *this; } \
  auto setVisible(bool visible = true) { return self().setVisible(visible), *this; } \
  auto visible(bool recursive = false) const { return self().visible(recursive); } \

#define DeclareSharedAction(Name) \
  DeclareSharedObject(Name) \

#define DeclareSharedSizable(Name) \
  DeclareSharedObject(Name) \
  auto doSize() const { return self().doSize(); } \
  auto geometry() const { return self().geometry(); } \
  auto minimumSize() const { return self().minimumSize(); } \
  auto onSize(const function<void ()>& callback = {}) { return self().onSize(callback), *this; } \
  auto setGeometry(Geometry geometry) { return self().setGeometry(geometry), *this; } \

#define DeclareSharedWidget(Name) \
  DeclareSharedSizable(Name) \
  auto setToolTip(const string& toolTip = "") { return self().setToolTip(toolTip), *this; } \
  auto toolTip() const { return self().toolTip(); } \

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
  auto objectCount() const { return self().objectCount(); }
  template<typename T = Object> auto objects() const -> vector<T> {
    vector<T> objects;
    for(auto object : self().objects()) {
      if(auto casted = object.cast<T>()) objects.append(casted);
    }
    return objects;
  }
  auto remove(sObject object) -> type& { return self().remove(object), *this; }

private:
  auto _append() {}
  template<typename T, typename... P> auto _append(T* object, P&&... p) {
    append(*object);
    _append(std::forward<P>(p)...);
  }
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
  auto actionCount() const { return self().actionCount(); }
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
  auto doDrop(vector<string> names) { return self().doDrop(names); }
  auto doMouseLeave() const { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const { return self().doMouseRelease(button); }
  auto gradient() const { return self().gradient(); }
  auto icon() const { return self().icon(); }
  auto onDrop(const function<void (vector<string>)>& callback = {}) { return self().onDrop(callback), *this; }
  auto onMouseLeave(const function<void ()>& callback = {}) { return self().onMouseLeave(callback), *this; }
  auto onMouseMove(const function<void (Position)>& callback = {}) { return self().onMouseMove(callback), *this; }
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) { return self().onMousePress(callback), *this; }
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) { return self().onMouseRelease(callback), *this; }
  auto setColor(Color color) { return self().setColor(color), *this; }
  auto setDroppable(bool droppable = true) { return self().setDroppable(droppable), *this; }
  auto setGradient(Gradient gradient = {}) { return self().setGradient(gradient), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setSize(Size size = {}) { return self().setSize(size), *this; }
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
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto remove(sComboButtonItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setParent(mObject* parent = nullptr, signed offset = -1) { return self().setParent(parent, offset), *this; }
};
#endif

#if defined(Hiro_ComboEdit)
struct ComboEditItem : sComboEditItem {
  DeclareSharedObject(ComboEditItem)

  auto icon() const { return self().icon(); }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_ComboEdit)
struct ComboEdit : sComboEdit {
  DeclareSharedWidget(ComboEdit)

  auto append(sComboEditItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto editable() const { return self().editable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(uint position) const { return self().item(position); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto remove(sComboEditItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
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

  auto append(sSizable sizable) { return self().append(sizable), *this; }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto reset() { return self().reset(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto sizable() const { return self().sizable(); }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_HexEdit)
struct HexEdit : sHexEdit {
  DeclareSharedWidget(HexEdit)

  auto address() const { return self().address(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto columns() const { return self().columns(); }
  auto doRead(unsigned offset) const { return self().doRead(offset); }
  auto doWrite(unsigned offset, uint8_t data) const { return self().doWrite(offset, data); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto length() const { return self().length(); }
  auto onRead(const function<uint8_t (unsigned)>& callback = {}) { return self().onRead(callback), *this; }
  auto onWrite(const function<void (unsigned, uint8_t)>& callback = {}) { return self().onWrite(callback), *this; }
  auto rows() const { return self().rows(); }
  auto setAddress(unsigned address) { return self().setAddress(address), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setColumns(unsigned columns = 16) { return self().setColumns(columns), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setLength(unsigned length) { return self().setLength(length), *this; }
  auto setRows(unsigned rows = 16) { return self().setRows(rows), *this; }
  auto update() { return self().update(), *this; }
};
#endif

#if defined(Hiro_HorizontalScrollBar)
struct HorizontalScrollBar : sHorizontalScrollBar {
  DeclareSharedWidget(HorizontalScrollBar)

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
  auto batchable() const { return self().batchable(); }
  auto batched() const { return self().batched(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto flow() const { return self().flow(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(unsigned position) const { return self().item(position); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto orientation() const { return self().orientation(); }
  auto remove(sIconViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setBatchable(bool batchable = true) { return self().setBatchable(batchable), *this; }
  auto setFlow(Orientation orientation = Orientation::Vertical) { return self().setFlow(orientation), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setOrientation(Orientation orientation = Orientation::Horizontal) { return self().setOrientation(orientation), *this; }
  auto setSelected(const vector<signed>& selections) { return self().setSelected(selections), *this; }
};
#endif

#if defined(Hiro_Label)
struct Label : sLabel {
  DeclareSharedWidget(Label)

  auto alignment() const { return self().alignment(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
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

  auto cursor() const { return self().cursor(); }
  auto doChange() const { return self().doChange(); }
  auto doMove() const { return self().doMove(); }
  auto editable() const { return self().editable(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto setCursor(Cursor cursor = {}) { return self().setCursor(cursor), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto setWordWrap(bool wordWrap = true) { return self().setWordWrap(wordWrap), *this; }
  auto text() const { return self().text(); }
  auto wordWrap() const { return self().wordWrap(); }
};
#endif

#if defined(Hiro_TabFrame)
struct TabFrameItem : sTabFrameItem {
  DeclareSharedObject(TabFrameItem)

  auto append(sSizable sizable) { return self().append(sizable), *this; }
  auto closable() const { return self().closable(); }
  auto icon() const { return self().icon(); }
  auto movable() const { return self().movable(); }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setClosable(bool closable = true) { return self().setClosable(closable), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setMovable(bool movable = true) { return self().setMovable(movable), *this; }
  auto setSelected() { return self().setSelected(), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto sizable() const { return self().sizable(); }
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
  auto item(unsigned position) const { return self().item(position); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto navigation() const { return self().navigation(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onClose(const function<void (sTabFrameItem)>& callback = {}) { return self().onClose(callback), *this; }
  auto onMove(const function<void (sTabFrameItem, sTabFrameItem)>& callback = {}) { return self().onMove(callback), *this; }
  auto remove(sTabFrameItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setNavigation(Navigation navigation = Navigation::Top) { return self().setNavigation(navigation), *this; }
};
#endif

#if defined(Hiro_TableView)
struct TableViewColumn : sTableViewColumn {
  DeclareSharedObject(TableViewColumn)

  auto active() const { return self().active(); }
  auto alignment() const { return self().alignment(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto editable() const { return self().editable(); }
  auto expandable() const { return self().expandable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto horizontalAlignment() const { return self().horizontalAlignment(); }
  auto icon() const { return self().icon(); }
  auto resizable() const { return self().resizable(); }
  auto setActive() { return self().setActive(), *this; }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setEditable(bool editable = true) { return self().setEditable(editable), *this; }
  auto setExpandable(bool expandable = true) { return self().setExpandable(expandable), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setHorizontalAlignment(float alignment = 0.0) { return self().setHorizontalAlignment(alignment), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setResizable(bool resizable = true) { return self().setResizable(resizable), *this; }
  auto setSorting(Sort sorting = Sort::None) { return self().setSorting(sorting), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto setVerticalAlignment(float alignment = 0.5) { return self().setVerticalAlignment(alignment), *this; }
  auto setWidth(float width = 0) { return self().setWidth(width), *this; }
  auto sort(Sort sorting) { return self().sort(sorting), *this; }
  auto sorting() const { return self().sorting(); }
  auto text() const { return self().text(); }
  auto verticalAlignment() const { return self().verticalAlignment(); }
  auto width() const { return self().width(); }
};
#endif

#if defined(Hiro_TableView)
struct TableViewCell : sTableViewCell {
  DeclareSharedObject(TableViewCell)

  auto alignment() const { return self().alignment(); }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto checkable() const { return self().checkable(); }
  auto checked() const { return self().checked(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto icon() const { return self().icon(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) const { return self().setCheckable(checkable), *this; }
  auto setChecked(bool checked = true) const { return self().setChecked(checked), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setIcon(const image& icon = {}) { return self().setIcon(icon), *this; }
  auto setText(const string& text = "") { return self().setText(text), *this; }
  auto text() const { return self().text(); }
};
#endif

#if defined(Hiro_TableView)
struct TableViewItem : sTableViewItem {
  DeclareSharedObject(TableViewItem)

  auto alignment() const { return self().alignment(); }
  auto append(sTableViewCell cell) { return self().append(cell), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto cell(unsigned position) const { return self().cell(position); }
  auto cellCount() const { return self().cellCount(); }
  auto cells() const { return self().cells(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto remove(sTableViewCell cell) { return self().remove(cell), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setSelected(bool selected = true) { return self().setSelected(selected), *this; }
};
#endif

#if defined(Hiro_TableView)
struct TableView : sTableView {
  DeclareSharedWidget(TableView)

  auto alignment() const { return self().alignment(); }
  auto append(sTableViewColumn column) { return self().append(column), *this; }
  auto append(sTableViewItem item) { return self().append(item), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto batchable() const { return self().batchable(); }
  auto batched() const { return self().batched(); }
  auto bordered() const { return self().bordered(); }
  auto column(uint position) const { return self().column(position); }
  auto columnCount() const { return self().columnCount(); }
  auto columns() const { return self().columns(); }
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto doEdit(sTableViewCell cell) const { return self().doEdit(cell); }
  auto doSort(sTableViewColumn column) const { return self().doSort(column); }
  auto doToggle(sTableViewCell cell) const { return self().doToggle(cell); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto headered() const { return self().headered(); }
  auto item(unsigned position) const { return self().item(position); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto onEdit(const function<void (TableViewCell)>& callback = {}) { return self().onEdit(callback), *this; }
  auto onSort(const function<void (TableViewColumn)>& callback = {}) { return self().onSort(callback), *this; }
  auto onToggle(const function<void (TableViewCell)>& callback = {}) { return self().onToggle(callback), *this; }
  auto remove(sTableViewColumn column) { return self().remove(column), *this; }
  auto remove(sTableViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto resizeColumns() { return self().resizeColumns(), *this; }
  auto selected() const { return self().selected(); }
  auto setAlignment(Alignment alignment = {}) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setBatchable(bool batchable = true) { return self().setBatchable(batchable), *this; }
  auto setBordered(bool bordered = true) { return self().setBordered(bordered), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
  auto setHeadered(bool headered = true) { return self().setHeadered(headered), *this; }
  auto setSortable(bool sortable = true) { return self().setSortable(sortable), *this; }
  auto sort() { return self().sort(), *this; }
  auto sortable() const { return self().sortable(); }
};
#endif

#if defined(Hiro_TextEdit)
struct TextEdit : sTextEdit {
  DeclareSharedWidget(TextEdit)

  auto backgroundColor() const { return self().backgroundColor(); }
  auto cursor() const { return self().cursor(); }
  auto doChange() const { return self().doChange(); }
  auto doMove() const { return self().doMove(); }
  auto editable() const { return self().editable(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCursor(Cursor cursor = {}) { return self().setCursor(cursor), *this; }
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
  auto backgroundColor() const { return self().backgroundColor(); }
  auto checkable() const { return self().checkable(); }
  auto checked() const { return self().checked(); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto icon() const { return self().icon(); }
  auto item(const string& path) const { return self().item(path); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto path() const { return self().path(); }
  auto remove(sTreeViewItem item) { return self().remove(item), *this; }
  auto selected() const { return self().selected(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCheckable(bool checkable = true) { return self().setCheckable(checkable), *this; }
  auto setChecked(bool checked = true) { return self().setChecked(checked), *this; }
  auto setExpanded(bool expanded = true) { return self().setExpanded(expanded), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
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
  auto doActivate() const { return self().doActivate(); }
  auto doChange() const { return self().doChange(); }
  auto doContext() const { return self().doContext(); }
  auto doToggle(sTreeViewItem item) const { return self().doToggle(item); }
  auto foregroundColor() const { return self().foregroundColor(); }
  auto item(const string& path) const { return self().item(path); }
  auto itemCount() const { return self().itemCount(); }
  auto items() const { return self().items(); }
  auto onActivate(const function<void ()>& callback = {}) { return self().onActivate(callback), *this; }
  auto onChange(const function<void ()>& callback = {}) { return self().onChange(callback), *this; }
  auto onContext(const function<void ()>& callback = {}) { return self().onContext(callback), *this; }
  auto onToggle(const function<void (sTreeViewItem)>& callback = {}) { return self().onToggle(callback), *this; }
  auto remove(sTreeViewItem item) { return self().remove(item), *this; }
  auto reset() { return self().reset(), *this; }
  auto selected() const { return self().selected(); }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setForegroundColor(Color color = {}) { return self().setForegroundColor(color), *this; }
};
#endif

#if defined(Hiro_VerticalScrollBar)
struct VerticalScrollBar : sVerticalScrollBar {
  DeclareSharedWidget(VerticalScrollBar)

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

  auto doDrop(vector<string> names) const { return self().doDrop(names); }
  auto doMouseLeave() const { return self().doMouseLeave(); }
  auto doMouseMove(Position position) const { return self().doMouseMove(position); }
  auto doMousePress(Mouse::Button button) const { return self().doMousePress(button); }
  auto doMouseRelease(Mouse::Button button) const { return self().doMouseRelease(button); }
  auto droppable() const { return self().droppable(); }
  auto handle() const { return self().handle(); }
  auto onDrop(const function<void (vector<string>)>& callback = {}) { return self().onDrop(callback), *this; }
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
  auto actionCount() const { return self().actionCount(); }
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
  auto menuCount() const { return self().menuCount(); }
  auto menus() const { return self().menus(); }
  auto remove(sMenu menu) { return self().remove(menu), *this; }
  auto reset() { return self().reset(), *this; }
};
#endif

#if defined(Hiro_Window)
struct Window : sWindow {
  DeclareSharedObject(Window)

  auto append(sMenuBar menuBar) { return self().append(menuBar), *this; }
  auto append(sSizable sizable) { return self().append(sizable), *this; }
  auto append(sStatusBar statusBar) { return self().append(statusBar), *this; }
  auto backgroundColor() const { return self().backgroundColor(); }
  auto dismissable() const { return self().dismissable(); }
  auto doClose() const { return self().doClose(); }
  auto doDrop(vector<string> names) const { return self().doDrop(names); }
  auto doKeyPress(signed key) const { return self().doKeyPress(key); }
  auto doKeyRelease(signed key) const { return self().doKeyRelease(key); }
  auto doMove() const { return self().doMove(); }
  auto doSize() const { return self().doSize(); }
  auto droppable() const { return self().droppable(); }
  auto frameGeometry() const { return self().frameGeometry(); }
  auto fullScreen() const { return self().fullScreen(); }
  auto geometry() const { return self().geometry(); }
  auto maximized() const { return self().maximized(); }
  auto maximumSize() const { return self().maximumSize(); }
  auto menuBar() const { return self().menuBar(); }
  auto minimized() const { return self().minimized(); }
  auto minimumSize() const { return self().minimumSize(); }
  auto modal() const { return self().modal(); }
  auto monitor() const { return self().monitor(); }
  auto onClose(const function<void ()>& callback = {}) { return self().onClose(callback), *this; }
  auto onDrop(const function<void (vector<string>)>& callback = {}) { return self().onDrop(callback), *this; }
  auto onKeyPress(const function<void (signed)>& callback = {}) { return self().onKeyPress(callback), *this; }
  auto onKeyRelease(const function<void (signed)>& callback = {}) { return self().onKeyRelease(callback), *this; }
  auto onMove(const function<void ()>& callback = {}) { return self().onMove(callback), *this; }
  auto onSize(const function<void ()>& callback = {}) { return self().onSize(callback), *this; }
  auto remove(sMenuBar menuBar) { return self().remove(menuBar), *this; }
  auto remove(sSizable sizable) { return self().remove(sizable), *this; }
  auto remove(sStatusBar statusBar) { return self().remove(statusBar), *this; }
  auto reset() { return self().reset(), *this; }
  auto resizable() const { return self().resizable(); }
  auto setAlignment(Alignment alignment) { return self().setAlignment(alignment), *this; }
  auto setBackgroundColor(Color color = {}) { return self().setBackgroundColor(color), *this; }
  auto setCentered(sWindow parent = {}) { return self().setCentered(parent), *this; }
  auto setDismissable(bool dismissable = true) { return self().setDismissable(dismissable), *this; }
  auto setDroppable(bool droppable = true) { return self().setDroppable(droppable), *this; }
  auto setFrameGeometry(Geometry geometry) { return self().setFrameGeometry(geometry), *this; }
  auto setFramePosition(Position position) { return self().setFramePosition(position), *this; }
  auto setFrameSize(Size size) { return self().setFrameSize(size), *this; }
  auto setFullScreen(bool fullScreen = true) { return self().setFullScreen(fullScreen), *this; }
  auto setGeometry(Geometry geometry) { return self().setGeometry(geometry), *this; }
  auto setMaximized(bool maximized) { return self().setMaximized(maximized), *this; }
  auto setMaximumSize(Size size = {}) { return self().setMaximumSize(size), *this; }
  auto setMinimized(bool minimized) { return self().setMinimized(minimized), *this; }
  auto setMinimumSize(Size size = {}) { return self().setMinimumSize(size), *this; }
  auto setModal(bool modal = true) { return self().setModal(modal), *this; }
  auto setPosition(Position position) { return self().setPosition(position), *this; }
  auto setResizable(bool resizable = true) { return self().setResizable(resizable), *this; }
  auto setSize(Size size) { return self().setSize(size), *this; }
  auto setTitle(const string& title = "") { return self().setTitle(title), *this; }
  auto sizable() const { return self().sizable(); }
  auto statusBar() const { return self().statusBar(); }
  auto title() const { return self().title(); }
};
#endif
