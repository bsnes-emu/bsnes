struct pFont;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pObject {
  unsigned id;
  bool locked;
  static array<pObject*> objects;

  pObject();
  static pObject* find(unsigned id);
  virtual void unused() {}
};

struct pOS : public pObject {
  struct State {
    Font defaultFont;
  };
  static State *state;

  static unsigned desktopWidth();
  static unsigned desktopHeight();
  static string fileLoad(Window &parent, const string &path, const lstring &filter);
  static string fileSave(Window &parent, const string &path, const lstring &filter);
  static string folderSelect(Window &parent, const string &path);
  static void main();
  static bool pending();
  static void process();
  static void quit();

  static void initialize();
};

struct pFont : public pObject {
  Font &font;
  HFONT hfont;

  void setBold(bool bold);
  void setFamily(const string &family);
  void setItalic(bool italic);
  void setSize(unsigned size);
  void setUnderline(bool underline);

  pFont(Font &font) : font(font) {}
  void constructor();
};

struct pMessageWindow : public pObject {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pWindow : public pObject {
  Window &window;
  HWND hwnd;
  HMENU hmenu;
  HWND hstatus;
  HBRUSH brush;
  COLORREF brushColor;

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Geometry frameGeometry();
  bool focused();
  Geometry geometry();
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setFrameGeometry(const Geometry &geometry);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setMenuFont(Font &font);
  void setMenuVisible(bool visible);
  void setResizable(bool resizable);
  void setStatusFont(Font &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(Font &font);

  pWindow(Window &window) : window(window) {}
  void constructor();
  Geometry frameMargin();
  void updateMenu();
};

struct pAction : public pObject {
  Action &action;
  HMENU parentMenu;
  Window *parentWindow;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action) : action(action) {}
  void constructor();
};

struct pMenu : public pAction {
  Menu &menu;
  HMENU hmenu;

  void append(Action &action);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void update(Window &parentWindow, HMENU parentMenu);
};

struct pMenuSeparator : public pAction {
  MenuSeparator &menuSeparator;

  pMenuSeparator(MenuSeparator &menuSeparator) : pAction(menuSeparator), menuSeparator(menuSeparator) {}
  void constructor();
};

struct pMenuItem : public pAction {
  MenuItem &menuItem;

  void setText(const string &text);

  pMenuItem(MenuItem &menuItem) : pAction(menuItem), menuItem(menuItem) {}
  void constructor();
};

struct pMenuCheckItem : public pAction {
  MenuCheckItem &menuCheckItem;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pMenuCheckItem(MenuCheckItem &menuCheckItem) : pAction(menuCheckItem), menuCheckItem(menuCheckItem) {}
  void constructor();
};

struct pMenuRadioItem : public pAction {
  MenuRadioItem &menuRadioItem;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<MenuRadioItem&> &group);
  void setText(const string &text);

  pMenuRadioItem(MenuRadioItem &menuRadioItem) : pAction(menuRadioItem), menuRadioItem(menuRadioItem) {}
  void constructor();
};

struct pWidget : public pObject {
  Widget &widget;
  HWND hwnd;

  bool enabled();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(Font &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : widget(widget) {}
  void constructor();
  void setDefaultFont();
  virtual void setParent(Window &parent);
};

struct pButton : public pWidget {
  Button &button;

  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();
  void setParent(Window &parent);
};

struct pCheckBox : public pWidget {
  CheckBox &checkBox;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox) : pWidget(checkBox), checkBox(checkBox) {}
  void constructor();
  void setParent(Window &parent);
};

struct pComboBox : public pWidget {
  ComboBox &comboBox;

  void append(const string &text);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {}
  void constructor();
  void setParent(Window &parent);
};

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;
  LRESULT CALLBACK (*windowProc)(HWND, UINT, LPARAM, WPARAM);

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  bool keyPress(unsigned key);
  void setParent(Window &parent);
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void setParent(Window &parent);
};

struct pLabel : public pWidget {
  Label &label;

  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
  void setParent(Window &parent);
};

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void setParent(Window &parent);
};

struct pListView : public pWidget {
  ListView &listView;
  bool lostFocus;

  void append(const lstring &text);
  void autosizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring &text);
  void modify(unsigned row, unsigned column, const string &text);
  void reset();
  optional<unsigned> selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setSelection(unsigned row);

  pListView(ListView &listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void setParent(Window &parent);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void setParent(Window &parent);
};

struct pRadioBox : public pWidget {
  RadioBox &radioBox;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<RadioBox&> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {}
  void constructor();
  void setParent(Window &parent);
};

struct pTextEdit : public pWidget {
  TextEdit &textEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void setParent(Window &parent);
};

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void setParent(Window &parent);
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void setParent(Window &parent);
};
