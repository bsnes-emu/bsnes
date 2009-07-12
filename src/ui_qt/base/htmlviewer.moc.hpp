class HtmlViewerWindow : public QObject {
  Q_OBJECT

public:
  QWidget *window;
    QVBoxLayout *layout;
      QTextBrowser *document;

  void setup();
  void show(const char *title, const char *htmlData);

public slots:
} *htmlViewerWindow;
