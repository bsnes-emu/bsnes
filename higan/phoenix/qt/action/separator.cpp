namespace phoenix {

void pSeparator::constructor() {
  qtAction = new QAction(0);
  qtAction->setSeparator(true);
}

void pSeparator::destructor() {
  if(action.state.menu) action.state.menu->remove(separator);
  delete qtAction;
}

}
