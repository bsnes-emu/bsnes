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
  geometry.x += 1, geometry.width -= 3;
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
