void pMenuControl::enable(bool) {
}

void pMenuControl::disable() {
}

bool pMenuControl::enabled() {
  return true;
}

pMenuControl::pMenuControl(MenuControl &self_) : pWidget(self_), self(self_) {
  parent = 0;
  text = 0;
}

pMenuControl::~pMenuControl() {
  if(text) free(text);
}
