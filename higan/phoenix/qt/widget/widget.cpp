namespace phoenix {

bool pWidget::focused() {
  return qtWidget->hasFocus();
}

Size pWidget::minimumSize() {
  return {0, 0};
}

void pWidget::setEnabled(bool enabled) {
  if(widget.state.abstract) enabled = false;
  if(sizable.state.layout && sizable.state.layout->enabled() == false) enabled = false;
  qtWidget->setEnabled(enabled);
}

void pWidget::setFocused() {
  qtWidget->setFocus(Qt::OtherFocusReason);
}

void pWidget::setFont(string font) {
  qtWidget->setFont(pFont::create(font));
}

void pWidget::setGeometry(Geometry geometry) {
  qtWidget->setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  if(sizable.state.layout == 0) visible = false;
  if(sizable.state.layout && sizable.state.layout->visible() == false) visible = false;
  qtWidget->setVisible(visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) qtWidget = new QWidget;
}

//pWidget::constructor() called before p{Derived}::constructor(); ergo qtWidget is not yet valid
//pWidget::synchronizeState() is called to finish construction of p{Derived}::constructor()
void pWidget::synchronizeState() {
  setEnabled(widget.state.enabled);
  setFont(widget.state.font);
//setVisible(widget.state.visible);
}

void pWidget::destructor() {
  if(widget.state.abstract) {
    delete qtWidget;
    qtWidget = nullptr;
  }
}

void pWidget::orphan() {
  destructor();
  constructor();
}

}
