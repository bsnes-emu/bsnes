#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrame::construct() -> void {
  qtWidget = qtTabFrame = new QtTabFrame(*this);
  qtTabFrame->connect(qtTabFrame, SIGNAL(currentChanged(int)), SLOT(onChange(int)));

  pWidget::construct();
  _setState();
}

auto pTabFrame::destruct() -> void {
  delete qtTabFrame;
  qtWidget = qtTabFrame = nullptr;
}

auto pTabFrame::append(sTabFrameItem item) -> void {
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
}

auto pTabFrame::setEdge(Edge edge) -> void {
}

/*
auto pTabFrame::container(Widget& widget) -> QWidget* {
  Layout* widgetLayout = GetParentWidgetLayout(&widget);
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout == widgetLayout) return qtTabFrame->widget(selection);
    selection++;
  }
  return nullptr;
}

auto pTabFrame::displacement() -> Position {
  return {5, 33};
}

auto pTabFrame::remove(unsigned selection) -> void {
  qtTabFrame->removeTab(selection);
}

auto pTabFrame::setEnabled(bool enabled) -> void {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setEnabled(layout->enabled());
  }
  pWidget::setEnabled(enabled);
}

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  geometry.x += 0, geometry.width -= 5;
  geometry.y += 29, geometry.height -= 33;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setGeometry(geometry);
  }
  synchronizeLayout();
}
*/

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);

//  geometry.setPosition({0, 0});
//  geometry.setWidth(geometry.width() - 4);
//  geometry.setHeight(geometry.height() - 25);

  for(auto& item : state().items) {
    if(auto self = item->self()) self->setGeometry(geometry);
  }
}

/*
auto pTabFrame::setIcon(unsigned selection, const image& icon) -> void {
  qtTabFrame->setTabIcon(selection, CreateIcon(image));
}

auto pTabFrame::setSelected(unsigned selection) -> void {
  lock();
  qtTabFrame->setCurrentIndex(selection);
  synchronizeLayout();
  unlock();
}

auto pTabFrame::setVisible(bool visible) -> void {
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(layout->visible());
  }
  pWidget::setVisible(visible);
}
*/

/*
auto pTabFrame::synchronizeLayout() -> void {
  unsigned selection = 0;
  for(auto& layout : tabFrame.state.layout) {
    if(layout) layout->setVisible(selection == tabFrame.state.selection);
    selection++;
  }
}
*/

auto pTabFrame::_setState() -> void {
  for(auto& item : state().items) {
    if(auto self = item->self()) self->_setState();
//    if(auto layout = item->state.layout) {
//      item->setVisible(item->visible(true));
//    }
  }
}

auto QtTabFrame::showEvent(QShowEvent* event) -> void {
  QTabWidget::showEvent(event);
  p._setState();  //needed to capture geometry of TabFrame for TabFrameItem layouts
}

auto QtTabFrame::onChange(int selection) -> void {
//  state().selection = selection;
//  synchronizeLayout();
//  if(!p.locked()) p.self().doChange();
}

}

#endif
