#include "state.hpp"
#include "layout/fixed-layout.cpp"
#include "layout/horizontal-layout.cpp"
#include "layout/vertical-layout.cpp"

#if defined(PHOENIX_WINDOWS)
  #include "../windows/windows.cpp"
#elif defined(PHOENIX_QT)
  #include "../qt/qt.cpp"
#elif defined(PHOENIX_GTK)
  #include "../gtk/gtk.cpp"
#elif defined(PHOENIX_REFERENCE)
  #include "../reference/reference.cpp"
#endif

Object::Object() { OS::initialize(); }

Geometry OS::availableGeometry() { return pOS::availableGeometry(); }
Geometry OS::desktopGeometry() { return pOS::desktopGeometry(); }
string OS::fileLoad_(Window &parent, const string &path, const lstring &filter_) { auto filter = filter_; if(filter.size() == 0) filter.append("All files (*)"); return pOS::fileLoad(parent, path, filter); }
string OS::fileSave_(Window &parent, const string &path, const lstring &filter_) { auto filter = filter_; if(filter.size() == 0) filter.append("All files (*)"); return pOS::fileSave(parent, path, filter); }
string OS::folderSelect(Window &parent, const string &path) { return pOS::folderSelect(parent, path); }
void OS::main() { return pOS::main(); }
bool OS::pendingEvents() { return pOS::pendingEvents(); }
void OS::processEvents() { return pOS::processEvents(); }
void OS::quit() { return pOS::quit(); }
void OS::initialize() { static bool initialized = false; if(initialized == false) { initialized = true; return pOS::initialize(); } }

Geometry Font::geometry(const string &text) { return p.geometry(text); }
void Font::setBold(bool bold) { state.bold = bold; return p.setBold(bold); }
void Font::setFamily(const string &family) { state.family = family; return p.setFamily(family); }
void Font::setItalic(bool italic) { state.italic = italic; return p.setItalic(italic); }
void Font::setSize(unsigned size) { state.size = size; return p.setSize(size); }
void Font::setUnderline(bool underline) { state.underline = underline; return p.setUnderline(underline); }
Font::Font() : state(*new State), p(*new pFont(*this)) { p.constructor(); }

void Timer::setEnabled(bool enabled) { state.enabled = enabled; return p.setEnabled(enabled); }
void Timer::setInterval(unsigned milliseconds) { state.milliseconds = milliseconds; return p.setInterval(milliseconds); }
Timer::Timer() : state(*new State), p(*new pTimer(*this)) { p.constructor(); }

MessageWindow::Response MessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::information(parent, text, buttons); }
MessageWindow::Response MessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::question(parent, text, buttons); }
MessageWindow::Response MessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::warning(parent, text, buttons); }
MessageWindow::Response MessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::critical(parent, text, buttons); }

Window Window::None;
void Window::append(Layout &layout) { state.layout.append(layout); return p.append(layout); }
void Window::append(Menu &menu) { state.menu.append(menu); ((Action&)menu).state.parent = this; return p.append(menu); }
void Window::append(Widget &widget) { state.widget.append(widget); return p.append(widget); }
Geometry Window::frameGeometry() { Geometry geometry = p.geometry(), margin = p.frameMargin(); return { geometry.x - margin.x, geometry.y - margin.y, geometry.width + margin.width, geometry.height + margin.height }; }
Geometry Window::frameMargin() { return p.frameMargin(); }
bool Window::focused() { return p.focused(); }
Geometry Window::geometry() { return p.geometry(); }
void Window::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) { state.backgroundColor = true; state.backgroundColorRed = red; state.backgroundColorGreen = green; state.backgroundColorBlue = blue; return p.setBackgroundColor(red, green, blue); }
void Window::setFrameGeometry(const Geometry &geometry) { Geometry margin = p.frameMargin(); return setGeometry({ geometry.x + margin.x, geometry.y + margin.y, geometry.width - margin.width, geometry.height - margin.height }); }
void Window::setFocused() { return p.setFocused(); }
void Window::setFullScreen(bool fullScreen) { state.fullScreen = fullScreen; return p.setFullScreen(fullScreen); }
void Window::setGeometry(const Geometry &geometry) { state.geometry = geometry; return p.setGeometry(geometry); }
void Window::setMenuFont(Font &font) { state.menuFont = &font; return p.setMenuFont(font); }
void Window::setMenuVisible(bool visible) { state.menuVisible = visible; return p.setMenuVisible(visible); }
void Window::setResizable(bool resizable) { state.resizable = resizable; return p.setResizable(resizable); }
void Window::setStatusFont(Font &font) { state.statusFont = &font; return p.setStatusFont(font); }
void Window::setStatusText(const string &text) { state.statusText = text; return p.setStatusText(text); }
void Window::setStatusVisible(bool visible) { state.statusVisible = visible; return p.setStatusVisible(visible); }
void Window::setTitle(const string &text) { state.title = text; return p.setTitle(text); }
void Window::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
void Window::setWidgetFont(Font &font) { state.widgetFont = &font; return p.setWidgetFont(font); }
Window::Window() : state(*new State), p(*new pWindow(*this)) { p.constructor(); }

void Action::setEnabled(bool enabled) { state.enabled = enabled; return p.setEnabled(enabled); }
void Action::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
Action::Action(pAction &p) : state(*new State), p(p) { p.constructor(); }

void Menu::append(Action &action) { state.action.append(action); return p.append(action); }
void Menu::setText(const string &text) { state.text = text; return p.setText(text); }
Menu::Menu() : state(*new State), base_from_member<pMenu&>(*new pMenu(*this)), Action(base_from_member<pMenu&>::value), p(base_from_member<pMenu&>::value) { p.constructor(); }

Separator::Separator() : base_from_member<pSeparator&>(*new pSeparator(*this)), Action(base_from_member<pSeparator&>::value), p(base_from_member<pSeparator&>::value) { p.constructor(); }

void Item::setText(const string &text) { state.text = text; return p.setText(text); }
Item::Item() : state(*new State), base_from_member<pItem&>(*new pItem(*this)), Action(base_from_member<pItem&>::value), p(base_from_member<pItem&>::value) { p.constructor(); }

bool CheckItem::checked() { return p.checked(); }
void CheckItem::setChecked(bool checked) { state.checked = checked; return p.setChecked(checked); }
void CheckItem::setText(const string &text) { state.text = text; return p.setText(text); }
CheckItem::CheckItem() : state(*new State), base_from_member<pCheckItem&>(*new pCheckItem(*this)), Action(base_from_member<pCheckItem&>::value), p(base_from_member<pCheckItem&>::value) { p.constructor(); }

void RadioItem::group(const reference_array<RadioItem&> &list) { foreach(item, list) item.p.setGroup(item.state.group = list); if(list.size()) list[0].setChecked(); }
bool RadioItem::checked() { return p.checked(); }
void RadioItem::setChecked() { foreach(item, state.group) item.state.checked = false; state.checked = true; return p.setChecked(); }
void RadioItem::setText(const string &text) { state.text = text; return p.setText(text); }
RadioItem::RadioItem() : state(*new State), base_from_member<pRadioItem&>(*new pRadioItem(*this)), Action(base_from_member<pRadioItem&>::value), p(base_from_member<pRadioItem&>::value) { p.constructor(); }

bool Widget::enabled() { return state.enabled; }
Font& Widget::font() { return p.font(); }
Geometry Widget::minimumGeometry() { return p.minimumGeometry(); }
void Widget::setEnabled(bool enabled) { state.enabled = enabled; return p.setEnabled(enabled); }
void Widget::setFocused() { return p.setFocused(); }
void Widget::setFont(Font &font) { state.font = &font; return p.setFont(font); }
void Widget::setGeometry(const Geometry &geometry) { state.geometry = geometry; return p.setGeometry(geometry); }
void Widget::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
bool Widget::visible() { return state.visible; }
Widget::Widget() : state(*new State), p(*new pWidget(*this)) { state.abstract = true; p.constructor(); }
Widget::Widget(pWidget &p) : state(*new State), p(p) { p.constructor(); }

void Button::setText(const string &text) { state.text = text; return p.setText(text); }
Button::Button() : state(*new State), base_from_member<pButton&>(*new pButton(*this)), Widget(base_from_member<pButton&>::value), p(base_from_member<pButton&>::value) { p.constructor(); }

uint32_t* Canvas::buffer() { return p.buffer(); }
void Canvas::update() { return p.update(); }
Canvas::Canvas() : base_from_member<pCanvas&>(*new pCanvas(*this)), Widget(base_from_member<pCanvas&>::value), p(base_from_member<pCanvas&>::value) { p.constructor(); }

bool CheckBox::checked() { return p.checked(); }
void CheckBox::setChecked(bool checked) { state.checked = checked; return p.setChecked(checked); }
void CheckBox::setText(const string &text) { state.text = text; return p.setText(text); }
CheckBox::CheckBox() : state(*new State), base_from_member<pCheckBox&>(*new pCheckBox(*this)), Widget(base_from_member<pCheckBox&>::value), p(base_from_member<pCheckBox&>::value) { p.constructor(); }

void ComboBox::append(const string &text) { state.text.append(text); return p.append(text); }
void ComboBox::reset() { state.selection = 0; state.text.reset(); return p.reset(); }
unsigned ComboBox::selection() { return p.selection(); }
void ComboBox::setSelection(unsigned row) { state.selection = row; return p.setSelection(row); }
ComboBox::ComboBox() : state(*new State), base_from_member<pComboBox&>(*new pComboBox(*this)), Widget(base_from_member<pComboBox&>::value), p(base_from_member<pComboBox&>::value) { p.constructor(); }

void HexEdit::setColumns(unsigned columns) { state.columns = columns; return p.setColumns(columns); }
void HexEdit::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void HexEdit::setOffset(unsigned offset) { state.offset = offset; return p.setOffset(offset); }
void HexEdit::setRows(unsigned rows) { state.rows = rows; return p.setRows(rows); }
void HexEdit::update() { return p.update(); }
HexEdit::HexEdit() : state(*new State), base_from_member<pHexEdit&>(*new pHexEdit(*this)), Widget(base_from_member<pHexEdit&>::value), p(base_from_member<pHexEdit&>::value) { p.constructor(); }

unsigned HorizontalSlider::position() { return p.position(); }
void HorizontalSlider::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void HorizontalSlider::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
HorizontalSlider::HorizontalSlider() : state(*new State), base_from_member<pHorizontalSlider&>(*new pHorizontalSlider(*this)), Widget(base_from_member<pHorizontalSlider&>::value), p(base_from_member<pHorizontalSlider&>::value) { p.constructor(); }

void Label::setText(const string &text) { state.text = text; return p.setText(text); }
Label::Label() : state(*new State), base_from_member<pLabel&>(*new pLabel(*this)), Widget(base_from_member<pLabel&>::value), p(base_from_member<pLabel&>::value) { p.constructor(); }

void LineEdit::setEditable(bool editable) { state.editable = editable; return p.setEditable(editable); }
void LineEdit::setText(const string &text) { state.text = text; return p.setText(text); }
string LineEdit::text() { return p.text(); }
LineEdit::LineEdit() : state(*new State), base_from_member<pLineEdit&>(*new pLineEdit(*this)), Widget(base_from_member<pLineEdit&>::value), p(base_from_member<pLineEdit&>::value) { p.constructor(); }

void ListView::append_(const lstring &text) { state.checked.append(false); state.text.append(text); return p.append(text); }
void ListView::autoSizeColumns() { return p.autoSizeColumns(); }
bool ListView::checked(unsigned row) { return p.checked(row); }
void ListView::modify_(unsigned row, const lstring &text) { state.text[row] = text; return p.modify(row, text); }
void ListView::reset() { state.checked.reset(); state.text.reset(); return p.reset(); }
bool ListView::selected() { return p.selected(); }
unsigned ListView::selection() { return p.selection(); }
void ListView::setCheckable(bool checkable) { state.checkable = checkable; return p.setCheckable(checkable); }
void ListView::setChecked(unsigned row, bool checked) { state.checked[row] = checked; return p.setChecked(row, checked); }
void ListView::setHeaderText_(const lstring &text) { state.headerText = text; return p.setHeaderText(text); }
void ListView::setHeaderVisible(bool visible) { state.headerVisible = visible; return p.setHeaderVisible(visible); }
void ListView::setSelected(bool selected) { state.selected = selected; return p.setSelected(selected); }
void ListView::setSelection(unsigned row) { state.selected = true; state.selection = row; return p.setSelection(row); }
ListView::ListView() : state(*new State), base_from_member<pListView&>(*new pListView(*this)), Widget(base_from_member<pListView&>::value), p(base_from_member<pListView&>::value) { p.constructor(); }

void ProgressBar::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
ProgressBar::ProgressBar() : state(*new State), base_from_member<pProgressBar&>(*new pProgressBar(*this)), Widget(base_from_member<pProgressBar&>::value), p(base_from_member<pProgressBar&>::value) { p.constructor(); }

void RadioBox::group(const reference_array<RadioBox&> &list) { foreach(item, list) item.p.setGroup(item.state.group = list); if(list.size()) list[0].setChecked(); }
bool RadioBox::checked() { return p.checked(); }
void RadioBox::setChecked() { foreach(item, state.group) item.state.checked = false; state.checked = true; return p.setChecked(); }
void RadioBox::setText(const string &text) { state.text = text; return p.setText(text); }
RadioBox::RadioBox() : state(*new State), base_from_member<pRadioBox&>(*new pRadioBox(*this)), Widget(base_from_member<pRadioBox&>::value), p(base_from_member<pRadioBox&>::value) { p.constructor(); }

void TextEdit::setCursorPosition(unsigned position) { state.cursorPosition = position; return p.setCursorPosition(position); }
void TextEdit::setEditable(bool editable) { state.editable = editable; return p.setEditable(editable); }
void TextEdit::setText(const string &text) { state.text = text; return p.setText(text); }
void TextEdit::setWordWrap(bool wordWrap) { state.wordWrap = wordWrap; return p.setWordWrap(wordWrap); }
string TextEdit::text() { return p.text(); }
TextEdit::TextEdit() : state(*new State), base_from_member<pTextEdit&>(*new pTextEdit(*this)), Widget(base_from_member<pTextEdit&>::value), p(base_from_member<pTextEdit&>::value) { p.constructor(); }

unsigned VerticalSlider::position() { return p.position(); }
void VerticalSlider::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void VerticalSlider::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
VerticalSlider::VerticalSlider() : state(*new State), base_from_member<pVerticalSlider&>(*new pVerticalSlider(*this)), Widget(base_from_member<pVerticalSlider&>::value), p(base_from_member<pVerticalSlider&>::value) { p.constructor(); }

uintptr_t Viewport::handle() { return p.handle(); }
Viewport::Viewport() : base_from_member<pViewport&>(*new pViewport(*this)), Widget(base_from_member<pViewport&>::value), p(base_from_member<pViewport&>::value) { p.constructor(); }
