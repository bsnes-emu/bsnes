void AboutWindow::setup() {
  window = new QWidget;
  window->setObjectName("about-window");
  window->setWindowTitle("About bsnes ...");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(0); {
    logo = new Logo;
    logo->setFixedSize(600, 106);
    layout->addWidget(logo);
    layout->addSpacing(Style::WidgetSpacing);

    info = new QLabel(utf8() <<
      "<table>"
      "<tr><td align='right'><b>Version: </b></td><td>" << bsnesVersion << "</td></tr>"
      "<tr><td align='right'><b>Author: </b></td><td>byuu</td></tr>"
      "<tr><td align='right'><b>Homepage: </b></td><td>http://byuu.org/</td></tr>"
      "</table>"
    );
    layout->addWidget(info);

    spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(spacer);
  }

  window->setLayout(layout);
}

void AboutWindow::Logo::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  QPixmap pixmap(":/logo.png");
  painter.drawPixmap(0, 0, pixmap);
}
