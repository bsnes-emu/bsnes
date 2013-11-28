void VerticalLayout::append(Sizable& sizable, Size size, unsigned spacing) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  children.append({&sizable, size.width, size.height, spacing});
  synchronizeLayout();
  if(window()) window()->synchronizeLayout();
}

void VerticalLayout::append(Sizable& sizable) {
  for(auto& child : children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

Size VerticalLayout::minimumSize() {
  unsigned width = 0, height = 0;

  for(auto& child : children) {
    if(child.width == Size::Minimum || child.width == Size::Maximum) {
      width = max(width, child.sizable->minimumSize().width);
      continue;
    }
    width = max(width, child.width);
  }

  for(auto& child : children) {
    height += child.spacing;
    if(child.height == Size::Minimum || child.height == Size::Maximum) {
      height += child.sizable->minimumSize().height;
      continue;
    }
    height += child.height;
  }

  return {state.margin * 2 + width, state.margin * 2 + height};
}

void VerticalLayout::remove(Sizable& sizable) {
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

void VerticalLayout::reset() {
  for(auto& child : children) {
    if(window() && dynamic_cast<Layout*>(child.sizable)) ((Layout*)child.sizable)->reset();
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void VerticalLayout::setAlignment(double alignment) {
  state.alignment = max(0.0, min(1.0, alignment));
}

void VerticalLayout::setEnabled(bool enabled) {
  Sizable::state.enabled = enabled;
  for(auto& child : children) {
    child.sizable->setEnabled(child.sizable->enabled());
  }
}

void VerticalLayout::setGeometry(Geometry containerGeometry) {
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

  unsigned minimumHeight = 0, maximumHeightCounter = 0;
  for(auto& child : children) {
    if(child.height == Size::Maximum) maximumHeightCounter++;
    if(child.height != Size::Maximum) minimumHeight += child.height;
    minimumHeight += child.spacing;
  }

  for(auto& child : children) {
    if(child.width  == Size::Maximum) child.width  = geometry.width;
    if(child.height == Size::Maximum) child.height = (geometry.height - minimumHeight) / maximumHeightCounter;
  }

  unsigned maximumWidth = 0;
  for(auto& child : children) maximumWidth = max(maximumWidth, child.width);

  for(auto& child : children) {
    unsigned pivot = (maximumWidth - child.width) * state.alignment;
    Geometry childGeometry = {geometry.x + pivot, geometry.y, child.width, child.height};
    if((signed)childGeometry.width < 1) childGeometry.width = 1;
    if((signed)childGeometry.height < 1) childGeometry.height = 1;
    child.sizable->setGeometry(childGeometry);

    geometry.y += child.height + child.spacing;
    geometry.height -= child.height + child.spacing;
  }
}

void VerticalLayout::setMargin(unsigned margin) {
  state.margin = margin;
}

void VerticalLayout::setVisible(bool visible) {
  Sizable::state.visible = visible;
  for(auto& child : children) {
    child.sizable->setVisible(child.sizable->visible());
  }
}

void VerticalLayout::synchronizeLayout() {
  for(auto& child : children) Layout::append(*child.sizable);
}

VerticalLayout::~VerticalLayout() {
  while(children.size()) remove(*children[0].sizable);
}
