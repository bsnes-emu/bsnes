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

void pMenu::setFont(Font &font) {
  qtMenu->setFont(*font.p.qtFont);
  foreach(item, menu.state.action) item.p.setFont(font);
}

void pMenu::setText(const string &text) {
  qtMenu->setTitle(QString::fromUtf8(text));
}

void pMenu::constructor() {
  qtMenu = new QMenu;
}
