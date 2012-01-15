struct pFont;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pFont {
  static Geometry geometry(const string &description, const string &text);
};

struct pDesktop {
  static Size size();
  static Geometry workspace();
};

struct pKeyboard {
  static bool pressed(Keyboard::Scancode scancode);
  static array<bool> state();
};

struct pMouse {
  static Position position();
  static bool pressed(Mouse::Button button);
};

struct pDialogWindow {
  static string fileOpen(Window &parent, const string &path, const lstring &filter);
  static string fileSave(Window &parent, const string &path, const lstring &filter);
  static string folderSelect(Window &parent, const string &path);
};

struct pMessageWindow {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pObject {
  Object &object;
  bool locked;

  pObject(Object &object) : object(object), locked(locked) {}
  virtual ~pObject() {}

  void constructor() {}
  void destructor() {}
};

struct pOS : public pObject {
  static void main();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

struct pTimer : public pObject {
  Timer &timer;

  void setEnabled(bool enabled);
  void setInterval(unsigned milliseconds);

  pTimer(Timer &timer) : pObject(timer), timer(timer) {}
  void constructor();
};

struct pWindow : public pObject {
  Window &window;

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Color backgroundColor();
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout &layout);
  void remove(Menu &menu);
  void remove(Widget &widget);
  void setBackgroundColor(const Color &color);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setMenuFont(const string &font);
  void setMenuVisible(bool visible);
  void setResizable(bool resizable);
  void setStatusFont(const string &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(const string &font);

  pWindow(Window &window) : pObject(window), window(window) {}
  void constructor();
};

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action) : pObject(action), action(action) {}
  void constructor();
};

struct pMenu : public pAction {
  Menu &menu;

  void append(Action &action);
  void remove(Action &action);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
};

struct pSeparator : public pAction {
  Separator &separator;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

struct pItem : public pAction {
  Item &item;

  void setImage(const image &image);
  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item) {}
  void constructor();
  void destructor();
};

struct pCheckItem : public pAction {
  CheckItem &checkItem;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckItem(CheckItem &checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();
};

struct pRadioItem : public pAction {
  RadioItem &radioItem;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
};

struct pSizable : public pObject {
  Sizable &sizable;

  pSizable(Sizable &sizable) : pObject(sizable), sizable(sizable) {}
};

struct pLayout : public pSizable {
  Layout &layout;

  pLayout(Layout &layout) : pSizable(layout), layout(layout) {}
};

struct pWidget : public pSizable {
  Widget &widget;

  bool enabled();
  Geometry minimumGeometry();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(const string &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : pSizable(widget), widget(widget) {}
  void constructor();
};

struct pButton : public pWidget {
  Button &button;

  void setImage(const image &image, Orientation orientation);
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();
};

struct pCanvas : public pWidget {
  Canvas &canvas;

  void setSize(const Size &size);
  void update();

  pCanvas(Canvas &canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
};

struct pCheckBox : public pWidget {
  CheckBox &checkBox;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox) : pWidget(checkBox), checkBox(checkBox) {}
  void constructor();
};

struct pComboBox : public pWidget {
  ComboBox &comboBox;

  void append(const string &text);
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {}
  void constructor();
};

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
};

struct pHorizontalScrollBar : public pWidget {
  HorizontalScrollBar &horizontalScrollBar;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScrollBar(HorizontalScrollBar &horizontalScrollBar) : pWidget(horizontalScrollBar), horizontalScrollBar(horizontalScrollBar) {}
  void constructor();
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
};

struct pLabel : public pWidget {
  Label &label;

  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
};

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
};

struct pListView : public pWidget {
  ListView &listView;

  void append(const lstring &text);
  void autoSizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring &text);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setSelected(bool selected);
  void setSelection(unsigned row);

  pListView(ListView &listView) : pWidget(listView), listView(listView) {}
  void constructor();
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;

  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
};

struct pRadioBox : public pWidget {
  RadioBox &radioBox;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<RadioBox&> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {}
  void constructor();
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
};

struct pVerticalScrollBar : public pWidget {
  VerticalScrollBar &verticalScrollBar;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScrollBar(VerticalScrollBar &verticalScrollBar) : pWidget(verticalScrollBar), verticalScrollBar(verticalScrollBar) {}
  void constructor();
};

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
};
