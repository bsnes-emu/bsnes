void FixedLayout::append(Sizable& sizable, Geometry geometry) {
  children.append({&sizable, geometry});
  synchronizeLayout();
  if(window()) window()->synchronizeLayout();
}

void FixedLayout::append(Sizable& sizable) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

Size FixedLayout::minimumSize() {
  unsigned width = Size::Minimum, height = Size::Minimum;
  for(auto& child : children) {
    width  = max(width,  child.sizable->minimumSize().width);
    height = max(height, child.sizable->minimumSize().height);
  }
  return {width, height};
}

void FixedLayout::remove(Sizable& sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      children.remove(n);
      Layout::remove(sizable);
      if(window()) window()->synchronizeLayout();
      break;
    }
  }
}

void FixedLayout::reset() {
  for(auto& child : children) {
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void FixedLayout::setEnabled(bool enabled) {
  Sizable::state.enabled = enabled;
  for(auto& child : children) {
    child.sizable->setEnabled(child.sizable->enabled());
  }
}

void FixedLayout::setGeometry(Geometry geometry) {
}

void FixedLayout::setVisible(bool visible) {
  Sizable::state.visible = visible;
  for(auto& child : children) {
    child.sizable->setVisible(child.sizable->visible());
  }
}

void FixedLayout::synchronizeLayout() {
  for(auto& child : children) {
    Layout::append(*child.sizable);
    Geometry childGeometry = child.geometry;
    if((signed)childGeometry.width < 1) childGeometry.width = 1;
    if((signed)childGeometry.height < 1) childGeometry.height = 1;
    child.sizable->setGeometry(childGeometry);
  }
}

FixedLayout::~FixedLayout() {
  while(children.size()) remove(*children[0].sizable);
}
