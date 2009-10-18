HtmlViewerWindow::HtmlViewerWindow() {
  window = new QbWindow(config.geometry.htmlViewerWindow);
  window->setObjectName("html-window");
  window->resize(560, 480);

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0);
  window->setLayout(layout);

  document = new QTextBrowser;
  layout->addWidget(document);
}

void HtmlViewerWindow::show(const char *title, const char *htmlData) {
  document->setHtml(utf8() << htmlData);
  window->setWindowTitle(title);
  window->show();
}
