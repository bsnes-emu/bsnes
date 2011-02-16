uintptr_t pViewport::handle() {
  return 0;
}

pViewport::pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {
}
