namespace phoenix {

void pTabFrame::append() {
  qtTabFrame->addTab(new QWidget, "");
}

QWidget* pTabFrame::container(Widget& widget) {
  Layout* widgetLayout = GetParentWidgetLayout(&widget);
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == widgetLayout) return qtTabFrame->widget(selection);
    selection++;
  }
  return nullptr;
}

Position pTabFrame::displacement() {
  return {5, 33};
}

void pTabFrame::remove(unsigned selection) {
  qtTabFrame->removeTab(selection);
}

void pTabFrame::setEnabled(bool enabled) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}

void pTabFrame::setGeometry(Geometry geometry) {
  pWidget::setGeometry(geometry);
  geometry.x += 0, geometry.width -= 5;
  geometry.y += 29, geometry.height -= 33;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setGeometry(geometry);
  }
  synchronizeLayout();
}

void pTabFrame::setImage(unsigned selection, const image& image) {
  qtTabFrame->setTabIcon(selection, CreateIcon(image));
}

void pTabFrame::setSelected(unsigned selection) {
  lock();
  qtTabFrame->setCurrentIndex(selection);
  synchronizeLayout();
  unlock();
}

void pTabFrame::setText(unsigned selection, string text) {
  qtTabFrame->setTabText(selection, QString::fromUtf8(text));
}

void pTabFrame::setVisible(bool visible) {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}

void pTabFrame::constructor() {
  qtWidget = qtTabFrame = new QTabWidget;
  connect(qtTabFrame, SIGNAL(currentChanged(int)), SLOT(onChange(int)));

  setSelected(tabFrame.state.selection);
}

void pTabFrame::destructor() {
  delete qtTabFrame;
  qtWidget = qtTabFrame = nullptr;
}

void pTabFrame::orphan() {
  destructor();
  constructor();
}

void pTabFrame::synchronizeLayout() {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}

void pTabFrame::onChange(int selection) {
  tabFrame.state.selection = selection;
  synchronizeLayout();
  if(!locked() && tabFrame.onChange) tabFrame.onChange();
}

}
