class HtmlViewerWindow : public QObject {
  Q_OBJECT

public:
  QbWindow *window;
  QVBoxLayout *layout;
    QTextBrowser *document;

  void show(const char *title, const char *htmlData);
  HtmlViewerWindow();

public slots:
} *htmlViewerWindow;
