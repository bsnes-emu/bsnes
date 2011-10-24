void VerticalLayout::append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing) {
  for(auto &child : children) if(child.sizable == &sizable) return;
  children.append({ &sizable, width, height, spacing });
  synchronizeLayout();
  if(window()) window()->synchronizeLayout();
}

void VerticalLayout::append(Sizable &sizable) {
  for(auto &child : children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

bool VerticalLayout::enabled() {
  if(layout()) return state.enabled && layout()->enabled();
  return state.enabled;
}

Geometry VerticalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  for(auto &child : children) {
    if(child.width == MinimumSize || child.width == MaximumSize) {
      width = max(width, child.sizable->minimumGeometry().width);
      continue;
    }
    width = max(width, child.width);
  }

  for(auto &child : children) {
    height += child.spacing;
    if(child.height == MinimumSize || child.height == MaximumSize) {
      height += child.sizable->minimumGeometry().height;
      continue;
    }
    height += child.height;
  }

  return { 0, 0, state.margin * 2 + width, state.margin * 2 + height };
}

void VerticalLayout::remove(Sizable &sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      if(dynamic_cast<Layout*>(children[n].sizable)) {
        Layout *layout = (Layout*)children[n].sizable;
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
  for(auto &child : children) {
    if(window() && dynamic_cast<Layout*>(child.sizable)) ((Layout*)child.sizable)->reset();
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void VerticalLayout::setAlignment(double alignment) {
  state.alignment = max(0.0, min(1.0, alignment));
}

void VerticalLayout::setEnabled(bool enabled) {
  state.enabled = enabled;
  for(auto &child : children) {
    child.sizable->setEnabled(dynamic_cast<Widget*>(child.sizable) ? child.sizable->enabled() : enabled);
  }
}

void VerticalLayout::setGeometry(const Geometry &containerGeometry) {
  auto children = this->children;
  for(auto &child : children) {
    if(child.width  == MinimumSize) child.width  = child.sizable->minimumGeometry().width;
    if(child.height == MinimumSize) child.height = child.sizable->minimumGeometry().height;
  }

  Geometry geometry = containerGeometry;
  geometry.x      += state.margin;
  geometry.y      += state.margin;
  geometry.width  -= state.margin * 2;
  geometry.height -= state.margin * 2;

  unsigned minimumHeight = 0, maximumHeightCounter = 0;
  for(auto &child : children) {
    if(child.height == MaximumSize) maximumHeightCounter++;
    if(child.height != MaximumSize) minimumHeight += child.height;
    minimumHeight += child.spacing;
  }

  for(auto &child : children) {
    if(child.width  == MaximumSize) child.width  = geometry.width;
    if(child.height == MaximumSize) child.height = (geometry.height - minimumHeight) / maximumHeightCounter;
  }

  unsigned maximumWidth = 0;
  for(auto &child : children) maximumWidth = max(maximumWidth, child.width);

  for(auto &child : children) {
    unsigned pivot = (maximumWidth - child.width) * state.alignment;
    Geometry childGeometry = { geometry.x + pivot, geometry.y, child.width, child.height };
    child.sizable->setGeometry(childGeometry);

    geometry.y += child.height + child.spacing;
    geometry.height -= child.height + child.spacing;
  }
}

void VerticalLayout::setMargin(unsigned margin) {
  state.margin = margin;
}

void VerticalLayout::setVisible(bool visible) {
  state.visible = visible;
  for(auto &child : children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

void VerticalLayout::synchronizeLayout() {
  for(auto &child : children) Layout::append(*child.sizable);
}

bool VerticalLayout::visible() {
  if(layout()) return state.visible && layout()->visible();
  return state.visible;
}

VerticalLayout::VerticalLayout() {
  state.alignment = 0.0;
  state.enabled = true;
  state.margin = 0;
  state.visible = true;
}

VerticalLayout::~VerticalLayout() {
  while(children.size()) remove(*children[0].sizable);
}
