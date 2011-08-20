struct Settings : public configuration {
  unsigned frameGeometryX;
  unsigned frameGeometryY;
  unsigned frameGeometryWidth;
  unsigned frameGeometryHeight;
  unsigned menuGeometryHeight;
  unsigned statusGeometryHeight;

  void load();
  void save();
  Settings();
};

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
  static Font defaultFont;

  static Geometry availableGeometry();
  static Geometry desktopGeometry();
  static string fileLoad(Window &parent, const string &path, const lstring &filter);
  static string fileSave(Window &parent, const string &path, const lstring &filter);
  static string folderSelect(Window &parent, const string &path);
  static void main();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

struct pFont : public pObject {
  Font &font;
  PangoFontDescription *gtkFont;
  PangoLayout *gtkLayout;

  Geometry geometry(const string &text);
  void setBold(bool bold);
  void setFamily(const string &family);
  void setItalic(bool italic);
  void setSize(unsigned size);
  void setUnderline(bool underline);

  pFont(Font &font) : font(font) {}
  void constructor();
};

struct pTimer : public pObject {
  Timer &timer;

  void setEnabled(bool enabled);
  void setInterval(unsigned milliseconds);

  pTimer(Timer &timer) : timer(timer) {}
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
  GtkWidget *widget;
  GtkWidget *menuContainer;
  GtkWidget *formContainer;
  GtkWidget *statusContainer;
  GtkWidget *menu;
  GtkWidget *status;
  GdkEventConfigure lastConfigure;

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Color backgroundColor();
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void setBackgroundColor(const Color &color);
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
  unsigned menuHeight();
  unsigned statusHeight();
};

struct pAction : public pObject {
  Action &action;
  GtkWidget *widget;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action) : action(action) {}
  void constructor();
  virtual void setFont(Font &font);
};

struct pMenu : public pAction {
  Menu &menu;
  GtkWidget *submenu;

  void append(Action &action);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void setFont(Font &font);
};

struct pSeparator : public pAction {
  Separator &separator;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
};

struct pItem : public pAction {
  Item &item;

  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item) {}
  void constructor();
};

struct pCheckItem : public pAction {
  CheckItem &checkItem;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckItem(CheckItem &checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
};

struct pRadioItem : public pAction {
  RadioItem &radioItem;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
};

struct pWidget : public pObject {
  Widget &widget;
  GtkWidget *gtkWidget;
  pWindow *parentWindow;

  bool enabled();
  Font& font();
  virtual Geometry minimumGeometry();
  void setEnabled(bool enabled);
  virtual void setFocused();
  virtual void setFont(Font &font);
  virtual void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : widget(widget) {}
  void constructor();
};

struct pButton : public pWidget {
  Button &button;

  Geometry minimumGeometry();
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();
};

struct pCanvas : public pWidget {
  Canvas &canvas;
  cairo_surface_t *surface;

  uint32_t* buffer();
  void setGeometry(const Geometry &geometry);
  void update();

  pCanvas(Canvas &canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
};

struct pCheckBox : public pWidget {
  CheckBox &checkBox;

  bool checked();
  Geometry minimumGeometry();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox) : pWidget(checkBox), checkBox(checkBox) {}
  void constructor();
};

struct pComboBox : public pWidget {
  ComboBox &comboBox;
  unsigned itemCounter;

  void append(const string &text);
  Geometry minimumGeometry();
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {}
  void constructor();
};

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;
  GtkWidget *container;
  GtkWidget *subWidget;
  GtkWidget *scrollBar;
  GtkTextBuffer *textBuffer;
  GtkTextMark *textCursor;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  unsigned cursorPosition();
  bool keyPress(unsigned scancode);
  void scroll(unsigned position);
  void setCursorPosition(unsigned position);
  void setScroll();
  void updateScroll();
};

struct pHorizontalScrollBar : public pWidget {
  HorizontalScrollBar &horizontalScrollBar;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScrollBar(HorizontalScrollBar &horizontalScrollBar) : pWidget(horizontalScrollBar), horizontalScrollBar(horizontalScrollBar) {}
  void constructor();
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
};

struct pLabel : public pWidget {
  Label &label;

  Geometry minimumGeometry();
  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
};

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  Geometry minimumGeometry();
  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
};

struct pListView : public pWidget {
  ListView &listView;
  GtkWidget *subWidget;
  GtkListStore *store;
  struct GtkColumn {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *label;
  };
  linear_vector<GtkColumn> column;

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
  void create();
  void setFocused();
  void setFont(Font &font);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;

  Geometry minimumGeometry();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
};

struct pRadioBox : public pWidget {
  RadioBox &radioBox;

  bool checked();
  Geometry minimumGeometry();
  void setChecked();
  void setGroup(const reference_array<RadioBox&> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {}
  void constructor();
};

struct pTextEdit : public pWidget {
  TextEdit &textEdit;
  GtkWidget *subWidget;
  GtkTextBuffer *textBuffer;

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

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScrollBar(VerticalScrollBar &verticalScrollBar) : pWidget(verticalScrollBar), verticalScrollBar(verticalScrollBar) {}
  void constructor();
};

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  Geometry minimumGeometry();
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
