void pMenu::append(Action &action) {
  if(dynamic_cast<Menu*>(&action)) {
    qtMenu->addMenu(((Menu&)action).p.qtMenu);
  } else if(dynamic_cast<Separator*>(&action)) {
    qtMenu->addAction(((Separator&)action).p.qtAction);
  } else if(dynamic_cast<Item*>(&action)) {
    qtMenu->addAction(((Item&)action).p.qtAction);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    qtMenu->addAction(((CheckItem&)action).p.qtAction);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    qtMenu->addAction(((RadioItem&)action).p.qtAction);
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
