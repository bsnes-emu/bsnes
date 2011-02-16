struct OS::State {
  bool initialized;

  State() {
    initialized = false;
  }
};

struct Font::State {
  bool bold;
  string family;
  bool italic;
  unsigned size;
  bool underline;

  State() {
    bold = false;
    italic = false;
    size = 8;
    underline = false;
  }
};

struct Window::State {
  bool backgroundColor;
  unsigned backgroundColorRed, backgroundColorGreen, backgroundColorBlue;
  bool fullScreen;
  Geometry geometry;
  Layout *layout;
  Font *menuFont;
  bool menuVisible;
  bool resizable;
  Font *statusFont;
  string statusText;
  bool statusVisible;
  string title;
  bool visible;
  Font *widgetFont;

  State() {
    backgroundColor = false;
    backgroundColorRed = 0;
    backgroundColorGreen = 0;
    backgroundColorBlue = 0;
    fullScreen = false;
    geometry = { 128, 128, 256, 256 };
    layout = 0;
    menuFont = 0;
    menuVisible = false;
    resizable = true;
    statusVisible = false;
    visible = false;
    widgetFont = 0;
  }
};

struct Action::State {
  bool enabled;
  Window *parent;
  bool visible;

  State() {
    enabled = true;
    parent = 0;
    visible = true;
  }
};

struct Menu::State {
  string text;
};

struct MenuItem::State {
  string text;
};

struct MenuCheckItem::State {
  bool checked;
  string text;

  State() {
    checked = false;
  }
};

struct MenuRadioItem::State {
  bool checked;
  array<MenuRadioItem*> group;
  string text;

  State() {
    checked = true;
  }
};

struct Layout::State {
  Window *parent;

  State() {
    parent = 0;
  }
};

struct Widget::State {
  bool enabled;
  Font *font;
  Geometry geometry;
  bool visible;

  State() {
    enabled = true;
    font = 0;
    geometry = { 0, 0, 0, 0 };
    visible = true;
  }
};

struct Button::State {
  string text;
};

struct CheckBox::State {
  bool checked;
  string text;

  State() {
    checked = false;
  }
};

struct ComboBox::State {
  unsigned selection;
  linear_vector<string> text;

  State() {
    selection = 0;
  }
};

struct HexEdit::State {
  unsigned columns;
  unsigned length;
  unsigned offset;
  unsigned rows;

  State() {
    columns = 16;
    length = 0;
    offset = 0;
    rows = 16;
  }
};

struct HorizontalSlider::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

struct Label::State {
  string text;
};

struct LineEdit::State {
  bool editable;
  string text;

  State() {
    editable = true;
  }
};

struct ListView::State {
  bool checkable;
  array<bool> checked;
  lstring headerText;
  bool headerVisible;
  optional<unsigned> selection;
  linear_vector<lstring> text;

  State() : selection(false, 0) {
    checkable = false;
    headerVisible = false;
  }
};

struct ProgressBar::State {
  unsigned position;

  State() {
    position = 0;
  }
};

struct RadioBox::State {
  bool checked;
  array<RadioBox*> group;
  string text;

  State() {
    checked = true;
  }
};

struct TextEdit::State {
  unsigned cursorPosition;
  bool editable;
  string text;
  bool wordWrap;

  State() {
    cursorPosition = 0;
    editable = true;
    wordWrap = false;
  }
};

struct VerticalSlider::State {
  unsigned length;
  unsigned position;

  State() {
    length = 101;
    position = 0;
  }
};

#include "layout/fixed-layout.cpp"
#include "layout/horizontal-layout.cpp"
#include "layout/vertical-layout.cpp"

#if defined(PHOENIX_QT)
  #include "../qt/qt.cpp"
#elif defined(PHOENIX_REFERENCE)
  #include "../reference/reference.cpp"
#endif

Object::Object() { OS::initialize(); }

OS::State OS::state;
unsigned OS::desktopWidth() { return pOS::desktopWidth(); }
unsigned OS::desktopHeight() { return pOS::desktopHeight(); }
string OS::fileLoad_(Window &parent, const string &path, lstring &filter) { if(filter.size() == 0) filter.append("All files (*)"); return pOS::fileLoad(parent, path, filter); }
string OS::fileSave_(Window &parent, const string &path, lstring &filter) { if(filter.size() == 0) filter.append("All files (*)"); return pOS::fileSave(parent, path, filter); }
string OS::folderSelect(Window &parent, const string &path) { return pOS::folderSelect(parent, path); }
void OS::main() { return pOS::main(); }
bool OS::pending() { return pOS::pending(); }
void OS::process() { return pOS::process(); }
void OS::quit() { return pOS::quit(); }
void OS::initialize() { if(state.initialized == false) { state.initialized = true; return pOS::initialize(); } }

void Font::setBold(bool bold) { state.bold = bold; return p.setBold(bold); }
void Font::setFamily(const string &family) { state.family = family; return p.setFamily(family); }
void Font::setItalic(bool italic) { state.italic = italic; return p.setItalic(italic); }
void Font::setSize(unsigned size) { state.size = size; return p.setSize(size); }
void Font::setUnderline(bool underline) { state.underline = underline; return p.setUnderline(underline); }
Font::Font() : state(*new State), p(*new pFont(*this)) {}

MessageWindow::Response MessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::information(parent, text, buttons); }
MessageWindow::Response MessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::question(parent, text, buttons); }
MessageWindow::Response MessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::warning(parent, text, buttons); }
MessageWindow::Response MessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) { return pMessageWindow::critical(parent, text, buttons); }

Window Window::None;
void Window::append(Menu &menu) { ((Action&)menu).state.parent = this; return p.append(menu); }
Geometry Window::frameGeometry() { return p.frameGeometry(); }
bool Window::focused() { return p.focused(); }
Geometry Window::geometry() { return p.geometry(); }
void Window::setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) { state.backgroundColor = true; state.backgroundColorRed = red; state.backgroundColorGreen = green; state.backgroundColorBlue = blue; return p.setBackgroundColor(red, green, blue); }
void Window::setFrameGeometry(const Geometry &geometry) { return p.setFrameGeometry(geometry); }
void Window::setFocused() { return p.setFocused(); }
void Window::setFullScreen(bool fullScreen) { state.fullScreen = fullScreen; return p.setFullScreen(fullScreen); }
void Window::setGeometry(const Geometry &geometry) { state.geometry = geometry; return p.setGeometry(geometry); }
void Window::setLayout(Layout &layout) { layout.state.parent = this; state.layout = &layout; return p.setLayout(layout); }
void Window::setMenuFont(Font &font) { state.menuFont = &font; return p.setMenuFont(font); }
void Window::setMenuVisible(bool visible) { state.menuVisible = visible; return p.setMenuVisible(visible); }
void Window::setResizable(bool resizable) { state.resizable = resizable; return p.setResizable(resizable); }
void Window::setStatusFont(Font &font) { state.statusFont = &font; return p.setStatusFont(font); }
void Window::setStatusText(const string &text) { state.statusText = text; return p.setStatusText(text); }
void Window::setStatusVisible(bool visible) { state.statusVisible = visible; return p.setStatusVisible(visible); }
void Window::setTitle(const string &text) { state.title = text; return p.setTitle(text); }
void Window::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
void Window::setWidgetFont(Font &font) { state.widgetFont = &font; return p.setWidgetFont(font); }
Window::Window() : state(*new State), p(*new pWindow(*this)) {}

void Action::setEnabled(bool enabled) { state.enabled = enabled; return p.setEnabled(enabled); }
void Action::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
Action::Action(pAction &p) : state(*new State), p(p) {}

void Menu::append(Action &action) { return p.append(action); }
void Menu::setText(const string &text) { state.text = text; return p.setText(text); }
Menu::Menu() : state(*new State), base_from_member<pMenu&>(*new pMenu(*this)), Action(base_from_member<pMenu&>::value), p(base_from_member<pMenu&>::value) {}

MenuSeparator::MenuSeparator() : base_from_member<pMenuSeparator&>(*new pMenuSeparator(*this)), Action(base_from_member<pMenuSeparator&>::value), p(base_from_member<pMenuSeparator&>::value) {}

void MenuItem::setText(const string &text) { state.text = text; return p.setText(text); }
MenuItem::MenuItem() : state(*new State), base_from_member<pMenuItem&>(*new pMenuItem(*this)), Action(base_from_member<pMenuItem&>::value), p(base_from_member<pMenuItem&>::value) {}

bool MenuCheckItem::checked() { return p.checked(); }
void MenuCheckItem::setChecked(bool checked) { state.checked = checked; return p.setChecked(checked); }
void MenuCheckItem::setText(const string &text) { state.text = text; return p.setText(text); }
MenuCheckItem::MenuCheckItem() : state(*new State), base_from_member<pMenuCheckItem&>(*new pMenuCheckItem(*this)), Action(base_from_member<pMenuCheckItem&>::value), p(base_from_member<pMenuCheckItem&>::value) {}

bool MenuRadioItem::checked() { return p.checked(); }
void MenuRadioItem::setChecked() { foreach(item, state.group) item->state.checked = false; state.checked = true; return p.setChecked(); }
void MenuRadioItem::setGroup(const array<MenuRadioItem*> &group) { state.group = group; return p.setGroup(group); }
void MenuRadioItem::setText(const string &text) { state.text = text; return p.setText(text); }
MenuRadioItem::MenuRadioItem() : state(*new State), base_from_member<pMenuRadioItem&>(*new pMenuRadioItem(*this)), Action(base_from_member<pMenuRadioItem&>::value), p(base_from_member<pMenuRadioItem&>::value) {}

void Layout::append(Widget &widget) { return p.append(widget); }
void Layout::setParent(Window &parent) { state.parent = &parent; }
Layout::Layout() : state(*new State), p(*new pLayout(*this)) {}

bool Widget::enabled() { return p.enabled(); }
void Widget::setEnabled(bool enabled) { state.enabled = enabled; return p.setEnabled(enabled); }
void Widget::setFocused() { return p.setFocused(); }
void Widget::setFont(Font &font) { state.font = &font; return p.setFont(font); }
void Widget::setGeometry(const Geometry &geometry) { state.geometry = geometry; return p.setGeometry(geometry); }
void Widget::setVisible(bool visible) { state.visible = visible; return p.setVisible(visible); }
Widget::Widget(pWidget &p) : state(*new State), p(p) {}

void Button::setText(const string &text) { state.text = text; return p.setText(text); }
Button::Button() : state(*new State), base_from_member<pButton&>(*new pButton(*this)), Widget(base_from_member<pButton&>::value), p(base_from_member<pButton&>::value) {}

bool CheckBox::checked() { return p.checked(); }
void CheckBox::setChecked(bool checked) { state.checked = checked; return p.setChecked(checked); }
void CheckBox::setText(const string &text) { state.text = text; return p.setText(text); }
CheckBox::CheckBox() : state(*new State), base_from_member<pCheckBox&>(*new pCheckBox(*this)), Widget(base_from_member<pCheckBox&>::value), p(base_from_member<pCheckBox&>::value) {}

void ComboBox::append(const string &text) { state.text.append(text); return p.append(text); }
void ComboBox::reset() { state.selection = 0; state.text.reset(); return p.reset(); }
unsigned ComboBox::selection() { return p.selection(); }
void ComboBox::setSelection(unsigned row) { state.selection = row; return p.setSelection(row); }
ComboBox::ComboBox() : state(*new State), base_from_member<pComboBox&>(*new pComboBox(*this)), Widget(base_from_member<pComboBox&>::value), p(base_from_member<pComboBox&>::value) {}

void HexEdit::setColumns(unsigned columns) { state.columns = columns; return p.setColumns(columns); }
void HexEdit::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void HexEdit::setOffset(unsigned offset) { state.offset = offset; return p.setOffset(offset); }
void HexEdit::setRows(unsigned rows) { state.rows = rows; return p.setRows(rows); }
void HexEdit::update() { return p.update(); }
HexEdit::HexEdit() : state(*new State), base_from_member<pHexEdit&>(*new pHexEdit(*this)), Widget(base_from_member<pHexEdit&>::value), p(base_from_member<pHexEdit&>::value) {}

unsigned HorizontalSlider::position() { return p.position(); }
void HorizontalSlider::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void HorizontalSlider::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
HorizontalSlider::HorizontalSlider() : state(*new State), base_from_member<pHorizontalSlider&>(*new pHorizontalSlider(*this)), Widget(base_from_member<pHorizontalSlider&>::value), p(base_from_member<pHorizontalSlider&>::value) {}

void Label::setText(const string &text) { state.text = text; return p.setText(text); }
Label::Label() : state(*new State), base_from_member<pLabel&>(*new pLabel(*this)), Widget(base_from_member<pLabel&>::value), p(base_from_member<pLabel&>::value) {}

void LineEdit::setEditable(bool editable) { state.editable = editable; return p.setEditable(editable); }
void LineEdit::setText(const string &text) { state.text = text; return p.setText(text); }
string LineEdit::text() { return p.text(); }
LineEdit::LineEdit() : state(*new State), base_from_member<pLineEdit&>(*new pLineEdit(*this)), Widget(base_from_member<pLineEdit&>::value), p(base_from_member<pLineEdit&>::value) {}

void ListView::append_(lstring &text) { state.text.append(text); return p.append(text); }
void ListView::autosizeColumns() { return p.autosizeColumns(); }
bool ListView::checked(unsigned row) { return p.checked(row); }
void ListView::modify_(unsigned row, lstring &text) { state.text[row] = text; return p.modify(row, text); }
void ListView::modify(unsigned row, unsigned column, const string &text) { state.text[row][column] = text; return p.modify(row, column, text); }
void ListView::reset() { state.checked.reset(); state.text.reset(); return p.reset(); }
optional<unsigned> ListView::selection() { return p.selection(); }
void ListView::setCheckable(bool checkable) { state.checkable = checkable; return p.setCheckable(checkable); }
void ListView::setChecked(unsigned row, bool checked) { state.checked[row] = checked; return p.setChecked(row, checked); }
void ListView::setHeaderText_(lstring &text) { state.headerText = text; return p.setHeaderText(text); }
void ListView::setHeaderVisible(bool visible) { state.headerVisible = visible; return p.setHeaderVisible(visible); }
void ListView::setSelection(unsigned row) { state.selection = { true, row }; return p.setSelection(row); }
ListView::ListView() : state(*new State), base_from_member<pListView&>(*new pListView(*this)), Widget(base_from_member<pListView&>::value), p(base_from_member<pListView&>::value) {}

void ProgressBar::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
ProgressBar::ProgressBar() : state(*new State), base_from_member<pProgressBar&>(*new pProgressBar(*this)), Widget(base_from_member<pProgressBar&>::value), p(base_from_member<pProgressBar&>::value) {}

bool RadioBox::checked() { return p.checked(); }
void RadioBox::setChecked() { foreach(item, state.group) item->state.checked = false; state.checked = true; return p.setChecked(); }
void RadioBox::setGroup(const array<RadioBox*> &group) { state.group = group; return p.setGroup(group); }
void RadioBox::setText(const string &text) { state.text = text; return p.setText(text); }
RadioBox::RadioBox() : state(*new State), base_from_member<pRadioBox&>(*new pRadioBox(*this)), Widget(base_from_member<pRadioBox&>::value), p(base_from_member<pRadioBox&>::value) {}

void TextEdit::setCursorPosition(unsigned position) { state.cursorPosition = position; return p.setCursorPosition(position); }
void TextEdit::setEditable(bool editable) { state.editable = editable; return p.setEditable(editable); }
void TextEdit::setText(const string &text) { state.text = text; return p.setText(text); }
void TextEdit::setWordWrap(bool wordWrap) { state.wordWrap = wordWrap; return p.setWordWrap(wordWrap); }
string TextEdit::text() { return p.text(); }
TextEdit::TextEdit() : state(*new State), base_from_member<pTextEdit&>(*new pTextEdit(*this)), Widget(base_from_member<pTextEdit&>::value), p(base_from_member<pTextEdit&>::value) {}

unsigned VerticalSlider::position() { return p.position(); }
void VerticalSlider::setLength(unsigned length) { state.length = length; return p.setLength(length); }
void VerticalSlider::setPosition(unsigned position) { state.position = position; return p.setPosition(position); }
VerticalSlider::VerticalSlider() : state(*new State), base_from_member<pVerticalSlider&>(*new pVerticalSlider(*this)), Widget(base_from_member<pVerticalSlider&>::value), p(base_from_member<pVerticalSlider&>::value) {}

uintptr_t Viewport::handle() { return p.handle(); }
Viewport::Viewport() : base_from_member<pViewport&>(*new pViewport(*this)), Widget(base_from_member<pViewport&>::value), p(base_from_member<pViewport&>::value) {}

