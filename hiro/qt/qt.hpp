//$(moc) -i -o qt.moc qt.hpp

/*
  Qt requires moc in order to bind callbacks, which causes many complications.

  First, moc is not C++11-aware. Thus, all of the "public slots:" functions must
  be declared using C++98 syntax.

  Second, multiple inheritance with QObject (eg pWindow : QObject, pObject)
  seems to cause heap corruption. As such, we need to have separate classes for
  inheriting from QObject, which are defined in this file.

  Third, moc preprocessor output is required for every Q_OBJECT class. So to
  avoid needing to generate several .moc files, all QObject classes are placed
  inside this one file instead.
*/

#if !defined(HIRO_QT)
  #include "../components.hpp"
#endif

namespace hiro {

#if defined(Hiro_Timer)
struct QtTimer : public QTimer {
  Q_OBJECT
public:
  QtTimer(pTimer& p) : p(p) {}
  pTimer& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_Window)
struct QtWindow : public QWidget {
  QtWindow(pWindow& p) : p(p) {}
  auto closeEvent(QCloseEvent*) -> void;
  auto dragEnterEvent(QDragEnterEvent*) -> void;
  auto dropEvent(QDropEvent*) -> void;
  auto keyPressEvent(QKeyEvent*) -> void;
  auto keyReleaseEvent(QKeyEvent*) -> void;
  auto moveEvent(QMoveEvent*) -> void;
  auto resizeEvent(QResizeEvent*) -> void;
  auto sizeHint() const -> QSize;
  pWindow& p;
};
#endif

#if defined(Hiro_MenuItem)
struct QtMenuItem : public QAction {
  Q_OBJECT
public:
  QtMenuItem(pMenuItem& p) : QAction(nullptr), p(p) {}
  pMenuItem& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_MenuCheckItem)
struct QtMenuCheckItem : public QAction {
  Q_OBJECT
public:
  QtMenuCheckItem(pMenuCheckItem& p) : QAction(nullptr), p(p) {}
  pMenuCheckItem& p;
public slots:
  void onToggle();
};
#endif

#if defined(Hiro_MenuRadioItem)
struct QtMenuRadioItem : public QAction {
  Q_OBJECT
public:
  QtMenuRadioItem(pMenuRadioItem& p) : QAction(nullptr), p(p) {}
  pMenuRadioItem& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_Button)
struct QtButton : public QToolButton {
  Q_OBJECT
public:
  QtButton(pButton& p) : p(p) {}
  pButton& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_Canvas)
struct QtCanvas : public QWidget {
  Q_OBJECT
public:
  QtCanvas(pCanvas& p) : p(p) {}
  auto dragEnterEvent(QDragEnterEvent*) -> void;
  auto dropEvent(QDropEvent*) -> void;
  auto leaveEvent(QEvent*) -> void;
  auto mouseMoveEvent(QMouseEvent*) -> void;
  auto mousePressEvent(QMouseEvent*) -> void;
  auto mouseReleaseEvent(QMouseEvent*) -> void;
  auto paintEvent(QPaintEvent*) -> void;
  pCanvas& p;
};
#endif

#if defined(Hiro_CheckButton)
struct QtCheckButton : public QToolButton {
  Q_OBJECT
public:
  QtCheckButton(pCheckButton& p) : p(p) {}
  pCheckButton& p;
public slots:
  void onToggle(bool checked);
};
#endif

#if defined(Hiro_CheckLabel)
struct QtCheckLabel : public QCheckBox {
  Q_OBJECT
public:
  QtCheckLabel(pCheckLabel& p) : p(p) {}
  pCheckLabel& p;
public slots:
  void onToggle();
};
#endif

#if defined(Hiro_ComboButton)
struct QtComboButton : public QComboBox {
  Q_OBJECT
public:
  QtComboButton(pComboButton& p) : p(p) {}
  pComboButton& p;
public slots:
  void onChange(int offset);
};
#endif

#if defined(Hiro_HexEdit)
struct QtHexEdit : public QTextEdit {
  Q_OBJECT
public:
  QtHexEdit(pHexEdit& p) : p(p) {}
  auto keyPressEvent(QKeyEvent*) -> void;
  auto keyPressEventAcknowledge(QKeyEvent*) -> void;
  auto wheelEvent(QWheelEvent*) -> void;
  pHexEdit& p;
};

struct QtHexEditScrollBar : public QScrollBar {
  Q_OBJECT
public:
  QtHexEditScrollBar(pHexEdit& p) : p(p) {}
  auto event(QEvent*) -> bool;
  pHexEdit& p;
public slots:
  void onScroll();
};
#endif

#if defined(Hiro_HorizontalScrollBar)
struct QtHorizontalScrollBar : public QScrollBar {
  Q_OBJECT
public:
  QtHorizontalScrollBar(pHorizontalScrollBar& p) : QScrollBar(Qt::Horizontal), p(p) {}
  pHorizontalScrollBar& p;
public slots:
  void onChange();
};
#endif

#if defined(Hiro_HorizontalSlider)
struct QtHorizontalSlider : public QSlider {
  Q_OBJECT
public:
  QtHorizontalSlider(pHorizontalSlider& p) : QSlider(Qt::Horizontal), p(p) {}
  pHorizontalSlider& p;
public slots:
  void onChange();
};
#endif

#if defined(Hiro_Label)
struct QtLabel : public QWidget {
  Q_OBJECT
public:
  QtLabel(pLabel& p) : p(p) {}
  auto mousePressEvent(QMouseEvent*) -> void;
  auto mouseReleaseEvent(QMouseEvent*) -> void;
  auto paintEvent(QPaintEvent*) -> void;
  pLabel& p;
};
#endif

#if defined(Hiro_LineEdit)
struct QtLineEdit : public QLineEdit {
  Q_OBJECT
public:
  QtLineEdit(pLineEdit& p) : p(p) {}
  pLineEdit& p;
public slots:
  void onActivate();
  void onChange();
};
#endif

#if defined(Hiro_RadioLabel)
struct QtRadioLabel : public QRadioButton {
  Q_OBJECT
public:
  QtRadioLabel(pRadioLabel& p) : p(p) {}
  pRadioLabel& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_RadioButton)
struct QtRadioButton : public QToolButton {
  Q_OBJECT
public:
  QtRadioButton(pRadioButton& p) : p(p) {}
  pRadioButton& p;
public slots:
  void onActivate();
};
#endif

#if defined(Hiro_TabFrame)
struct QtTabFrame : public QTabWidget {
  Q_OBJECT
public:
  QtTabFrame(pTabFrame& p) : p(p) {}
  pTabFrame& p;
  auto showEvent(QShowEvent*) -> void override;
public slots:
  void onChange(int selection);
};
#endif

#if defined(Hiro_TableView)
struct QtTableView : public QTreeWidget {
  Q_OBJECT
public:
  QtTableView(pTableView& p) : p(p) {}
  auto mousePressEvent(QMouseEvent*) -> void override;
  auto resizeEvent(QResizeEvent*) -> void override;
  auto showEvent(QShowEvent*) -> void override;
  pTableView& p;
public slots:
  void onActivate(QTreeWidgetItem* item, int column);
  void onChange();
  void onContext();
  void onSort(int column);
  void onToggle(QTreeWidgetItem* item, int column);
};

struct QtTableViewDelegate : public QStyledItemDelegate {
  QtTableViewDelegate(pTableView& p);
  auto paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const -> void;
  pTableView& p;
};
#endif

#if defined(Hiro_TextEdit)
struct QtTextEdit : public QTextEdit {
  Q_OBJECT
public:
  QtTextEdit(pTextEdit& p) : p(p) {}
  pTextEdit& p;
public slots:
  void onChange();
};
#endif

#if defined(Hiro_VerticalScrollBar)
struct QtVerticalScrollBar : public QScrollBar {
  Q_OBJECT
public:
  QtVerticalScrollBar(pVerticalScrollBar& p) : QScrollBar(Qt::Vertical), p(p) {}
  pVerticalScrollBar& p;
public slots:
  void onChange();
};
#endif

#if defined(Hiro_VerticalSlider)
struct QtVerticalSlider : public QSlider {
  Q_OBJECT
public:
  QtVerticalSlider(pVerticalSlider& p) : QSlider(Qt::Vertical), p(p) {}
  pVerticalSlider& p;
public slots:
  void onChange();
};
#endif

#if defined(Hiro_Viewport)
struct QtViewport : public QWidget {
  Q_OBJECT
public:
  QtViewport(pViewport& p) : p(p) {}
  auto dragEnterEvent(QDragEnterEvent*) -> void;
  auto dropEvent(QDropEvent*) -> void;
  auto leaveEvent(QEvent*) -> void;
  auto mouseMoveEvent(QMouseEvent*) -> void;
  auto mousePressEvent(QMouseEvent*) -> void;
  auto mouseReleaseEvent(QMouseEvent*) -> void;
  pViewport& p;
};
#endif

}
