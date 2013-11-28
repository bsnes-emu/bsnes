void HorizontalLayout::append(Sizable& sizable, Size size, unsigned spacing) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  children.append({&sizable, size.width, size.height, spacing});
  synchronizeLayout();
  if(window()) window()->synchronizeLayout();
}

void HorizontalLayout::append(Sizable& sizable) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

Size HorizontalLayout::minimumSize() {
  unsigned width = 0, height = 0;

  for(auto& child : children) {
    width += child.spacing;
    if(child.width == Size::Minimum || child.width == Size::Maximum) {
      width += child.sizable->minimumSize().width;
      continue;
    }
    width += child.width;
  }

  for(auto& child : children) {
    if(child.height == Size::Minimum || child.height == Size::Maximum) {
      height = max(height, child.sizable->minimumSize().height);
      continue;
    }
    height = max(height, child.height);
  }

  return {state.margin * 2 + width, state.margin * 2 + height};
}

void HorizontalLayout::remove(Sizable& sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      if(dynamic_cast<Layout*>(children[n].sizable)) {
        Layout* layout = (Layout*)children[n].sizable;
        layout->reset();
      }
      children.remove(n);
      Layout::remove(sizable);
      if(window()) window()->synchronizeLayout();
      break;
    }
  }
}

void HorizontalLayout::reset() {
  for(auto& child : children) {
    if(window() && dynamic_cast<Layout*>(child.sizable)) ((Layout*)child.sizable)->reset();
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void HorizontalLayout::setAlignment(double alignment) {
  state.alignment = max(0.0, min(1.0, alignment));
}

void HorizontalLayout::setEnabled(bool enabled) {
  Sizable::state.enabled = enabled;
  for(auto& child : children) {
    child.sizable->setEnabled(child.sizable->enabled());
  }
}

void HorizontalLayout::setGeometry(Geometry containerGeometry) {
  auto children = this->children;
  for(auto& child : children) {
    if(child.width  == Size::Minimum) child.width  = child.sizable->minimumSize().width;
    if(child.height == Size::Minimum) child.height = child.sizable->minimumSize().height;
  }

  Geometry geometry = containerGeometry;
  geometry.x      += state.margin;
  geometry.y      += state.margin;
  geometry.width  -= state.margin * 2;
  geometry.height -= state.margin * 2;

  unsigned minimumWidth = 0, maximumWidthCounter = 0;
  for(auto& child : children) {
    if(child.width == Size::Maximum) maximumWidthCounter++;
    if(child.width != Size::Maximum) minimumWidth += child.width;
    minimumWidth += child.spacing;
  }

  for(auto& child : children) {
    if(child.width  == Size::Maximum) child.width  = (geometry.width - minimumWidth) / maximumWidthCounter;
    if(child.height == Size::Maximum) child.height = geometry.height;
  }

  unsigned maximumHeight = 0;
  for(auto& child : children) maximumHeight = max(maximumHeight, child.height);

  for(auto& child : children) {
    unsigned pivot = (maximumHeight - child.height) * state.alignment;
    Geometry childGeometry = {geometry.x, geometry.y + pivot, child.width, child.height};
    if((signed)childGeometry.width < 1) childGeometry.width = 1;
    if((signed)childGeometry.height < 1) childGeometry.height = 1;
    child.sizable->setGeometry(childGeometry);

    geometry.x += child.width + child.spacing;
    geometry.width -= child.width + child.spacing;
  }
}

void HorizontalLayout::setMargin(unsigned margin) {
  state.margin = margin;
}

void HorizontalLayout::setVisible(bool visible) {
  Sizable::state.visible = visible;
  for(auto& child : children) {
    child.sizable->setVisible(child.sizable->visible());
  }
}

void HorizontalLayout::synchronizeLayout() {
  for(auto& child : children) Layout::append(*child.sizable);
}

HorizontalLayout::~HorizontalLayout() {
  while(children.size()) remove(*children[0].sizable);
}
