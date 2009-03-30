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
  window->setMinimumSize(560, 480);
}

void HtmlViewerWindow::show(const char *title, const char *htmlData) {
  document->setHtml(utf8() << htmlData);
  window->setWindowTitle(title);
  utility.showCentered(window);
}
