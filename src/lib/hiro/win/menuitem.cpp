void pMenuItem::create(const char *text_) {
  text = strdup(text_);
}

pMenuItem::pMenuItem(MenuItem &self_) : pMenuControl(self_), self(self_) {
}
