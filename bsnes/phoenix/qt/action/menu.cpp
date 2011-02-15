void pMenu::append(Action &action) {
  if(dynamic_cast<Menu*>(&action)) {
    qtMenu->addMenu(((Menu&)action).p.qtMenu);
  } else if(dynamic_cast<MenuSeparator*>(&action)) {
    qtMenu->addAction(((MenuSeparator&)action).p.qtAction);
  } else if(dynamic_cast<MenuItem*>(&action)) {
    qtMenu->addAction(((MenuItem&)action).p.qtAction);
  } else if(dynamic_cast<MenuCheckItem*>(&action)) {
    qtMenu->addAction(((MenuCheckItem&)action).p.qtAction);
  } else if(dynamic_cast<MenuRadioItem*>(&action)) {
    qtMenu->addAction(((MenuRadioItem&)action).p.qtAction);
  }
}

void pMenu::setText(const string &text) {
  qtMenu->setTitle(QString::fromUtf8(text));
}

pMenu::pMenu(Menu &menu) : menu(menu), pAction(menu) {
  qtMenu = new QMenu;
}
