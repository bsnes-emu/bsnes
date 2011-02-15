pMenuSeparator::pMenuSeparator(MenuSeparator &menuSeparator) : menuSeparator(menuSeparator), pAction(menuSeparator) {
  qtAction = new QAction(0);
  qtAction->setSeparator(true);
}
