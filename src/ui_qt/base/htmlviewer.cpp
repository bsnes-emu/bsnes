HtmlViewerWindow::HtmlViewerWindow() {
  setObjectName("html-window");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0); {
    document = new QTextBrowser;
    layout->addWidget(document);
  }

  setLayout(layout);
  setMinimumSize(560, 480);
}

void HtmlViewerWindow::show(const char *title, const char *htmlData) {
  document->setHtml(utf8() << htmlData);
  setWindowTitle(title);
  showAt(0.0, 0.0);
  setFocus();
}
