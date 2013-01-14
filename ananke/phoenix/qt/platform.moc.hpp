static QApplication *qtApplication = nullptr;

struct Settings : public configuration {
  bidirectional_map<Keyboard::Scancode, unsigned> keymap;

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

struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pFont {
  static Geometry geometry(const string &description, const string &text);

  static QFont create(const string &description);
  static Geometry geometry(const QFont &qtFont, const string &text);
};

struct pDesktop {
  static Size size();
  static Geometry workspace();
};

struct pKeyboard {
  static bool pressed(Keyboard::Scancode scancode);
  static vector<bool> state();

  static void initialize();
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

  pObject(Object &object) : object(object), locked(false) {}
  virtual ~pObject() {}
  void constructor() {}
  void destructor() {}
};

struct pOS : public pObject {
  static XlibDisplay *display;

  static void main();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
  static void syncX();
};

struct pTimer : public QObject, public pObject {
  Q_OBJECT

public:
  Timer &timer;
  QTimer *qtTimer;

  void setEnabled(bool enabled);
  void setInterval(unsigned milliseconds);

  pTimer(Timer &timer) : pObject(timer), timer(timer) {}
  void constructor();
  void destructor();

public slots:
  void onTimeout();
};

struct pWindow : public QObject, public pObject {
  Q_OBJECT

public:
  Window &window;
  struct QtWindow : public QWidget {
    pWindow &self;
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void moveEvent(QMoveEvent*);
    void resizeEvent(QResizeEvent*);
    QSize sizeHint() const;
    QtWindow(pWindow &self) : self(self) {}
  } *qtWindow;
  QVBoxLayout *qtLayout;
  QMenuBar *qtMenu;
  QStatusBar *qtStatus;
  QWidget *qtContainer;

  static Window& none();

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Color backgroundColor();
  Geometry frameMargin();
  bool focused();
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
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(const string &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(const string &font);

  pWindow(Window &window) : pObject(window), window(window) {}
  void constructor();
  void destructor();
  void updateFrameGeometry();
};

struct pAction : public pObject {
  Action &action;

  void setEnabled(bool enabled);
  void setFont(const string &font);
  void setVisible(bool visible);

  pAction(Action &action) : pObject(action), action(action) {}
  void constructor();
  void destructor();
};

struct pMenu : public pAction {
  Menu &menu;
  QMenu *qtMenu;

  void append(Action &action);
  void remove(Action &action);
  void setFont(const string &font);
  void setImage(const image &image);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu) {}
  void constructor();
  void destructor();
};

struct pSeparator : public pAction {
  Separator &separator;
  QAction *qtAction;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

struct pItem : public QObject, public pAction {
  Q_OBJECT

public:
  Item &item;
  QAction *qtAction;

  void setImage(const image &image);
  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item) {}
  void constructor();
  void destructor();

public slots:
  void onActivate();
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
  void destructor();

public slots:
  void onToggle();
};

struct pRadioItem : public QObject, public pAction {
  Q_OBJECT

public:
  RadioItem &radioItem;
  QAction *qtAction;
  QActionGroup *qtGroup;

  bool checked();
  void setChecked();
  void setGroup(const set<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();

public slots:
  void onActivate();
};

struct pSizable : public pObject {
  Sizable &sizable;

  pSizable(Sizable &sizable) : pObject(sizable), sizable(sizable) {}

  void constructor() {}
  void destructor() {}
};

struct pLayout : public pSizable {
  Layout &layout;

  pLayout(Layout &layout) : pSizable(layout), layout(layout) {}

  void constructor() {}
  void destructor() {}
};

struct pWidget : public pSizable {
  Widget &widget;
  QWidget *qtWidget;

  bool focused();
  virtual Geometry minimumGeometry();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(const string &font);
  virtual void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : pSizable(widget), widget(widget) {}
  void constructor();
  void synchronizeState();
  void destructor();
  virtual void orphan();
};

struct pButton : public QObject, public pWidget {
  Q_OBJECT

public:
  Button &button;
  QToolButton *qtButton;

  Geometry minimumGeometry();
  void setImage(const image &image, Orientation orientation);
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
};

struct pCanvas : public QObject, public pWidget {
  Q_OBJECT

public:
  Canvas &canvas;
  QImage *qtImage;
  struct QtCanvas : public QWidget {
    pCanvas &self;
    void leaveEvent(QEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    QtCanvas(pCanvas &self);
  } *qtCanvas;

  void setSize(const Size &size);
  void update();

  pCanvas(Canvas &canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
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
  void destructor();
  void orphan();

public slots:
  void onToggle();
};

struct pComboBox : public QObject, public pWidget {
  Q_OBJECT

public:
  ComboBox &comboBox;
  QComboBox *qtComboBox;

  void append(const string &text);
  void modify(unsigned row, const string &text);
  void remove(unsigned row);
  Geometry minimumGeometry();
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboBox(ComboBox &comboBox) : pWidget(comboBox), comboBox(comboBox) {}
  void constructor();
  void destructor();
  void orphan();

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
  void destructor();
  void orphan();
  void keyPressEvent(QKeyEvent*);

public slots:
  void onScroll();
};

struct pHorizontalScrollBar : public QObject, public pWidget {
  Q_OBJECT

public:
  HorizontalScrollBar &horizontalScrollBar;
  QScrollBar *qtScrollBar;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScrollBar(HorizontalScrollBar &horizontalScrollBar) : pWidget(horizontalScrollBar), horizontalScrollBar(horizontalScrollBar) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onChange();
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
  void destructor();
  void orphan();

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
  void destructor();
  void orphan();
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
  void destructor();
  void orphan();

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
  void remove(unsigned row);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned row, unsigned column, const nall::image &image);
  void setSelected(bool selected);
  void setSelection(unsigned row);

  pListView(ListView &listView) : pWidget(listView), listView(listView) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
  void onChange(QTreeWidgetItem *item);
  void onToggle(QTreeWidgetItem *item);
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;
  QProgressBar *qtProgressBar;

  Geometry minimumGeometry();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
  void orphan();
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
  void setGroup(const set<RadioBox&> &group);
  void setText(const string &text);

  pRadioBox(RadioBox &radioBox) : pWidget(radioBox), radioBox(radioBox) {}
  void constructor();
  void destructor();
  void orphan();

public slots:
  void onActivate();
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
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pVerticalScrollBar : public QObject, public pWidget {
  Q_OBJECT

public:
  VerticalScrollBar &verticalScrollBar;
  QScrollBar *qtScrollBar;

  Geometry minimumGeometry();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScrollBar(VerticalScrollBar &verticalScrollBar) : pWidget(verticalScrollBar), verticalScrollBar(verticalScrollBar) {}
  void constructor();
  void destructor();
  void orphan();

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
  void destructor();
  void orphan();

public slots:
  void onChange();
};

struct pViewport : public pWidget {
  Viewport &viewport;
  struct QtViewport : public QWidget {
    pViewport &self;
    void leaveEvent(QEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    QtViewport(pViewport &self);
  } *qtViewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
  void orphan();
};
