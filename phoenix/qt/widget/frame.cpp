namespace phoenix {

void pFrame::setEnabled(bool enabled) {
  if(frame.state.layout) frame.state.layout->setEnabled(frame.state.layout->enabled());
  pWidget::setEnabled(enabled);
}

void pFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  if(frame.state.layout == nullptr) return;
  Size size = pFont::size(widget.state.font, frame.state.text);
  if(frame.state.text.empty()) size.height = 8;
  geometry.x += 1, geometry.width -= 2;
  geometry.y += size.height, geometry.height -= size.height + 1;
  frame.state.layout->setGeometry(geometry);
}

void pFrame::setText(string text) {
  qtFrame->setTitle(QString::fromUtf8(text));
}

void pFrame::setVisible(bool visible) {
  if(frame.state.layout) frame.state.layout->setVisible(frame.state.layout->visible());
  pWidget::setVisible(visible);
}

void pFrame::constructor() {
  qtWidget = qtFrame = new QGroupBox;
  if(QApplication::style()->objectName() == "gtk+") {
    //QGtkStyle (gtk+) theme disrespects font weight and omits the border, even if native GTK+ theme does not
    //bold Label controls already exist; so this style sheet forces QGtkStyle to look like a Frame instead
    qtFrame->setStyleSheet(
      "QGroupBox { border: 1px solid #aaa; border-radius: 5px; margin-top: 0.5em; }\n"
      "QGroupBox::title { left: 5px; subcontrol-origin: margin; }\n"
    );
  }

  pWidget::synchronizeState();
  setText(frame.state.text);
}

void pFrame::destructor() {
  delete qtFrame;
  qtWidget = qtFrame = nullptr;
}

void pFrame::orphan() {
  destructor();
  constructor();
}

}
