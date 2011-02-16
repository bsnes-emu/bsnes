struct pFont;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pObject {
  bool locked;

  pObject() {
    locked = false;
  }
};

struct pOS : public pObject {
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

  void setBold(bool bold);
  void setFamily(const string &family);
  void setItalic(bool italic);
  void setSize(unsigned size);
  void setUnderline(bool underline);

  pFont(Font &font);
};

struct pMessageWindow : public pObject {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pWindow : public pObject {
  Window &window;

  void append(Menu &menu);
  Geometry frameGeometry();
  bool focused();
  Geometry geometry();
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
  void setFrameGeometry(const Geometry &geometry);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setLayout(Layout &layout);
  void setMenuFont(Font &font);
  void setMenuVisible(bool visible);
  void setResizable(bool resizable);
  void setStatusFont(Font &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(Font &font);

  pWindow(Window &window);
};

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action);
};

struct pMenu : public pAction {
  Menu &menu;

  void append(Action &action);
  void setText(const string &text);

  pMenu(Menu &menu);
};

struct pMenuSeparator : public pAction {
  MenuSeparator &menuSeparator;

  pMenuSeparator(MenuSeparator &menuSeparator);
};

struct pMenuItem : public pAction {
  MenuItem &menuItem;

  void setText(const string &text);

  pMenuItem(MenuItem &menuItem);
};

struct pMenuCheckItem : public pAction {
  MenuCheckItem &menuCheckItem;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pMenuCheckItem(MenuCheckItem &menuCheckItem);
};

struct pMenuRadioItem : public pAction {
  MenuRadioItem &menuRadioItem;

  bool checked();
  void setChecked();
  void setGroup(const array<MenuRadioItem*> &group);
  void setText(const string &text);

  pMenuRadioItem(MenuRadioItem &menuRadioItem);
};

struct pLayout : public pObject {
  Layout &layout;

  void append(Widget &widget);

  pLayout(Layout &layout);
};

struct pWidget : public pObject {
  Widget &widget;

  bool enabled();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(Font &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget);
};

struct pButton : public pWidget {
  Button &button;

  void setText(const string &text);

  pButton(Button &button);
};

struct pCheckBox : public pWidget {
  CheckBox &checkBox;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox);
};

struct pComboBox : public pWidget {
  ComboBox &comboBox;

  void append(const string &text);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox);
};

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit);
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider);
};

struct pLabel : public pWidget {
  Label &label;

  void setText(const string &text);

  pLabel(Label &label);
};

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit);
};

struct pListView : public pWidget {
  ListView &listView;

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

  pListView(ListView &listView);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar);
};

struct pRadioBox : public pWidget {
  RadioBox &radioBox;

  bool checked();
  void setChecked();
  void setGroup(const array<RadioBox*> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox);
};

struct pTextEdit : public pWidget {
  TextEdit &textEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit);
};

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider);
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport);
};
