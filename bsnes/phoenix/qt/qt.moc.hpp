struct Settings : public configuration {
  unsigned frameGeometryX;
  unsigned frameGeometryY;
  unsigned frameGeometryWidth;
  unsigned frameGeometryHeight;

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
  static QApplication *application;
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
  QFont *qtFont;

  Geometry geometry(const string &text);
  void setBold(bool bold);
  void setFamily(const string &family);
  void setItalic(bool italic);
  void setSize(unsigned size);
  void setUnderline(bool underline);

  pFont(Font &font) : font(font) {}
  void constructor();
  void update();
};

struct pMessageWindow : public pObject {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pWindow : public QObject, public pObject {
  Q_OBJECT

public:
  Window &window;
  struct QtWindow : public QWidget {
    pWindow &self;
    void closeEvent(QCloseEvent*);
    void moveEvent(QMoveEvent*);
    void resizeEvent(QResizeEvent*);
    QSize sizeHint() const;
    QtWindow(pWindow &self) : self(self) {}
  } *qtWindow;
  QVBoxLayout *qtLayout;
  QMenuBar *qtMenu;
  QStatusBar *qtStatus;
  QWidget *qtContainer;

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Geometry frameMargin();
  bool focused();
  Geometry geometry();
  void setBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
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
  void updateFrameGeometry();
};

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setFont(Font &font);
  void setVisible(bool visible);

  pAction(Action &action) : action(action) {}
  void constructor();
};

struct pMenu : public pAction {
  Menu &menu;
  QMenu *qtMenu;

  void append(Action &action);
  void setFont(Font &font);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
};

struct pSeparator : public pAction {
  Separator &separator;
  QAction *qtAction;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
};

struct pItem : public QObject, public pAction {
  Q_OBJECT

public:
  Item &item;
  QAction *qtAction;

  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item) {}
  void constructor();

public slots:
  void onTick();
};

struct pCheckItem : public QObject, public pAction {
  Q_OBJECT

public:
  CheckItem &checkItem;
  QAction *qtAction;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckItem(CheckItem &checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();

public slots:
  void onTick();
};

struct pRadioItem : public QObject, public pAction {
  Q_OBJECT

public:
  RadioItem &radioItem;
  QAction *qtAction;
  QActionGroup *qtGroup;

  bool checked();
  void setChecked();
  void setGroup(const reference_array<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();

public slots:
  void onTick();
};

struct pWidget : public pObject {
  Widget &widget;
  QWidget *qtWidget;

  Font& font();
  virtual Geometry minimumGeometry();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(Font &font);
  void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : widget(widget) {}
  void constructor();
};

struct pButton : public QObject, public pWidget {
  Q_OBJECT

public:
  Button &button;
  QPushButton *qtButton;

  Geometry minimumGeometry();
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();

public slots:
  void onTick();
};

struct pCheckBox : public QObject, public pWidget {
  Q_OBJECT

public:
  CheckBox &checkBox;
  QCheckBox *qtCheckBox;

  bool checked();
  Geometry minimumGeometry();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckBox(CheckBox &checkBox) : pWidget(checkBox), checkBox(checkBox) {}
  void constructor();

public slots:
  void onTick();
};

struct pComboBox : public QObject, public pWidget {
  Q_OBJECT

public:
  ComboBox &comboBox;
  QComboBox *qtComboBox;

  void append(const string &text);
  Geometry minimumGeometry();
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {}
  void constructor();

public slots:
  void onChange();
};

struct pHexEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  HexEdit &hexEdit;
  struct QtHexEdit : public QTextEdit {
    pHexEdit &self;
    void keyPressEvent(QKeyEvent*);
    void keyPressEventAcknowledge(QKeyEvent*);
    QtHexEdit(pHexEdit &self) : self(self) {}
  } *qtHexEdit;
  QHBoxLayout *qtLayout;
  QScrollBar *qtScroll;

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void keyPressEvent(QKeyEvent*);

public slots:
  void onScroll();
};

struct pHorizontalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  HorizontalSlider &horizontalSlider;
  QSlider *qtSlider;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();

public slots:
  void onChange();
};

struct pLabel : public pWidget {
  Label &label;
  QLabel *qtLabel;

  Geometry minimumGeometry();
  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
};

struct pLineEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  LineEdit &lineEdit;
  QLineEdit *qtLineEdit;

  Geometry minimumGeometry();
  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();

public slots:
  void onActivate();
  void onChange();
};

struct pListView : public QObject, public pWidget {
  Q_OBJECT

public:
  ListView &listView;
  QTreeWidget *qtListView;

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

public slots:
  void onActivate();
  void onChange();
  void onTick(QTreeWidgetItem *item);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;
  QProgressBar *qtProgressBar;

  Geometry minimumGeometry();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
};

struct pRadioBox : public QObject, public pWidget {
  Q_OBJECT

public:
  RadioBox &radioBox;
  QRadioButton *qtRadioBox;
  QButtonGroup *qtGroup;

  bool checked();
  Geometry minimumGeometry();
  void setChecked();
  void setGroup(const reference_array<RadioBox&> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {}
  void constructor();

public slots:
  void onTick();
};

struct pTextEdit : public QObject, public pWidget {
  Q_OBJECT

public:
  TextEdit &textEdit;
  QTextEdit *qtTextEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();

public slots:
  void onChange();
};

struct pVerticalSlider : public QObject, public pWidget {
  Q_OBJECT

public:
  VerticalSlider &verticalSlider;
  QSlider *qtSlider;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();

public slots:
  void onChange();
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
};
