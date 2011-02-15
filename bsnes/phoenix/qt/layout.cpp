void pLayout::append(Widget &widget) {
  if(!widget.state.font && OS::state->defaultFont) widget.p.qtWidget->setFont(*OS::state->defaultFont->p.qtFont);
  widget.p.qtWidget->setParent(layout.state.parent->p.qtContainer);
  widget.p.qtWidget->setVisible(true);
}

pLayout::pLayout(Layout &layout) : layout(layout) {
}
