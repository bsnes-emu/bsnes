namespace phoenix {

QWidget* pWidget::container(Widget& widget) {
  return nullptr;
}

bool pWidget::focused() {
  return qtWidget->hasFocus();
}

Size pWidget::minimumSize() {
  return {0, 0};
}

void pWidget::setEnabled(bool enabled) {
  if(!widget.parent()) enabled = false;
  if(widget.state.abstract) enabled = false;
  if(!widget.enabledToAll()) enabled = false;
  qtWidget->setEnabled(enabled);
}

void pWidget::setFocused() {
  qtWidget->setFocus(Qt::OtherFocusReason);
}

void pWidget::setFont(string font) {
  qtWidget->setFont(pFont::create(font));
}

void pWidget::setGeometry(Geometry geometry) {
  Position displacement = GetDisplacement(&widget);
  geometry.x -= displacement.x;
  geometry.y -= displacement.y;

  qtWidget->setGeometry(geometry.x, geometry.y, geometry.width, geometry.height);
  if(widget.onSize) widget.onSize();
}

void pWidget::setVisible(bool visible) {
  if(!widget.parent()) visible = false;
  if(widget.state.abstract) visible = false;
  if(!widget.visibleToAll()) visible = false;
  qtWidget->setVisible(visible);
}

void pWidget::constructor() {
  if(widget.state.abstract) qtWidget = new QWidget;
}

//pWidget::constructor() called before p{Derived}::constructor(); ergo qtWidget is not yet valid
//pWidget::synchronizeState() is called to finish construction of p{Derived}::constructor()
void pWidget::synchronizeState() {
  setFont(widget.font());
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
