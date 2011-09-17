void HorizontalLayout::append(Sizable &sizable, unsigned width, unsigned height, unsigned spacing) {
  foreach(child, children) if(child.sizable == &sizable) return;
  children.append({ &sizable, width, height, spacing });
  synchronizeLayout();
}

void HorizontalLayout::append(Sizable &sizable) {
  foreach(child, children) if(child.sizable == &sizable) return;
  Layout::append(sizable);
  if(window()) window()->synchronizeLayout();
}

bool HorizontalLayout::enabled() {
  if(layout()) return state.enabled && layout()->enabled();
  return state.enabled;
}

Geometry HorizontalLayout::minimumGeometry() {
  unsigned width = 0, height = 0;

  foreach(child, children) {
    width += child.spacing;
    if(child.width == MinimumSize || child.width == MaximumSize) {
      width += child.sizable->minimumGeometry().width;
      continue;
    }
    width += child.width;
  }

  foreach(child, children) {
    if(child.height == MinimumSize || child.height == MaximumSize) {
      height = max(height, child.sizable->minimumGeometry().height);
      continue;
    }
    height = max(height, child.height);
  }

  return { 0, 0, state.margin * 2 + width, state.margin * 2 + height };
}

void HorizontalLayout::remove(Sizable &sizable) {
  for(unsigned n = 0; n < children.size(); n++) {
    if(children[n].sizable == &sizable) {
      children.remove(n);
      Layout::remove(sizable);
      break;
    }
  }
}

void HorizontalLayout::reset() {
  foreach(child, children) {
    if(window() && dynamic_cast<Widget*>(child.sizable)) window()->remove((Widget&)*child.sizable);
  }
}

void HorizontalLayout::setAlignment(double alignment) {
  state.alignment = max(0.0, min(1.0, alignment));
}

void HorizontalLayout::setEnabled(bool enabled) {
  state.enabled = enabled;
  foreach(child, children) {
    child.sizable->setEnabled(dynamic_cast<Widget*>(child.sizable) ? child.sizable->enabled() : enabled);
  }
}

void HorizontalLayout::setGeometry(const Geometry &containerGeometry) {
  auto children = this->children;
  foreach(child, children) {
    if(child.width  == MinimumSize) child.width  = child.sizable->minimumGeometry().width;
    if(child.height == MinimumSize) child.height = child.sizable->minimumGeometry().height;
  }

  Geometry geometry = containerGeometry;
  geometry.x      += state.margin;
  geometry.y      += state.margin;
  geometry.width  -= state.margin * 2;
  geometry.height -= state.margin * 2;

  unsigned minimumWidth = 0, maximumWidthCounter = 0;
  foreach(child, children) {
    if(child.width == MaximumSize) maximumWidthCounter++;
    if(child.width != MaximumSize) minimumWidth += child.width;
    minimumWidth += child.spacing;
  }

  foreach(child, children) {
    if(child.width  == MaximumSize) child.width  = (geometry.width - minimumWidth) / maximumWidthCounter;
    if(child.height == MaximumSize) child.height = geometry.height;
  }

  unsigned maximumHeight = 0;
  foreach(child, children) maximumHeight = max(maximumHeight, child.height);

  foreach(child, children) {
    unsigned pivot = (maximumHeight - child.height) * state.alignment;
    Geometry childGeometry = { geometry.x, geometry.y + pivot, child.width, child.height };
    child.sizable->setGeometry(childGeometry);

    geometry.x += child.width + child.spacing;
    geometry.width -= child.width + child.spacing;
  }
}

void HorizontalLayout::setMargin(unsigned margin) {
  state.margin = margin;
}

void HorizontalLayout::setVisible(bool visible) {
  state.visible = visible;
  foreach(child, children) {
    child.sizable->setVisible(dynamic_cast<Widget*>(child.sizable) ? child.sizable->visible() : visible);
  }
}

void HorizontalLayout::synchronizeLayout() {
  foreach(child, children) Layout::append(*child.sizable);
}

bool HorizontalLayout::visible() {
  if(layout()) return state.visible && layout()->visible();
  return state.visible;
}

HorizontalLayout::HorizontalLayout() {
  state.alignment = 0.5;
  state.enabled = true;
  state.margin = 0;
  state.visible = true;
}

HorizontalLayout::~HorizontalLayout() {
  while(children.size()) remove(*children[0].sizable);
}
