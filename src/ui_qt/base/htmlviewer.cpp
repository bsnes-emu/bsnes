void HtmlViewerWindow::setup() {
  window = new QWidget;
  window->setObjectName("html-window");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0); {
    document = new QTextBrowser;
    layout->addWidget(document);
  }

  window->setLayout(layout);
  window->resize(560, 480);
}

void HtmlViewerWindow::show(const char *title, const char *htmlData) {
  document->setHtml(utf8() << htmlData);
  window->setWindowTitle(title);
  window->show();

  static bool firstShow = true;
  if(firstShow == true) {
    firstShow = false;
    utility.centerWindow(window);
  }

  application.processEvents();
  window->activateWindow();
  window->raise();
}
