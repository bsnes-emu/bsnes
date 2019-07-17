#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrame::construct() -> void {
  qtWidget = qtTabFrame = new QtTabFrame(*this);
  qtTabFrame->connect(qtTabFrame, SIGNAL(currentChanged(int)), SLOT(onChange(int)));

  pWidget::construct();
  _setState();
}

auto pTabFrame::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtTabFrame;
  qtWidget = qtTabFrame = nullptr;
}

auto pTabFrame::append(sTabFrameItem item) -> void {
  setGeometry(self().geometry());
}

auto pTabFrame::remove(sTabFrameItem item) -> void {
}

auto pTabFrame::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);

  for(auto& item : state().items) {
    if(auto self = item->self()) self->setGeometry(geometry);
  }
}

auto pTabFrame::setNavigation(Navigation navigation) -> void {
  _setState();
}

auto pTabFrame::_setState() -> void {
  switch(state().navigation) { default:
  case Navigation::Top: qtTabFrame->setTabPosition(QTabWidget::TabPosition::North); break;
  case Navigation::Bottom: qtTabFrame->setTabPosition(QTabWidget::TabPosition::South); break;
  case Navigation::Left: qtTabFrame->setTabPosition(QTabWidget::TabPosition::West); break;
  case Navigation::Right: qtTabFrame->setTabPosition(QTabWidget::TabPosition::East); break;
  }

  for(auto& item : state().items) {
    if(auto self = item->self()) self->_setState();
  }
}

auto QtTabFrame::showEvent(QShowEvent* event) -> void {
  QTabWidget::showEvent(event);
  p._setState();  //needed to capture geometry of TabFrame for TabFrameItem layouts
}

auto QtTabFrame::onChange(int selection) -> void {
  //geometry of tab frames is only valid once said tab frame is visible
  //as such, as need to call _setState() to update the TabFrameItem's geometry here
  if(auto item = p.self().item(selection)) {
    if(auto self = item->self()) self->_setState();
  }
  if(!p.locked()) p.self().doChange();
}

}

#endif
