void pLayout::append(Widget &widget) {
  if(!widget.state.font && layout.state.parent->state.widgetFont) {
    widget.p.qtWidget->setFont(*layout.state.parent->state.widgetFont->p.qtFont);
  }
  widget.p.qtWidget->setParent(layout.state.parent->p.qtContainer);
  widget.p.qtWidget->setVisible(true);
}

pLayout::pLayout(Layout &layout) : layout(layout) {
}
