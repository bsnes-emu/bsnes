namespace phoenix {

void pAction::setEnabled(bool enabled) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pAction::setVisible(bool visible) {
  if(parentWindow) parentWindow->p.updateMenu();
}

void pAction::constructor() {
  parentMenu = 0;
  parentWindow = 0;
}

}
