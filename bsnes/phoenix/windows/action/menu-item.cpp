void pMenuItem::setText(const string &text) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pMenuItem::constructor() {
}
