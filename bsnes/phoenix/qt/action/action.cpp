void pAction::setEnabled(bool enabled) {
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->setEnabled(enabled);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setEnabled(enabled);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setEnabled(enabled);
  }
}

void pAction::setFont(Font &font) {
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.setFont(font);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setFont(*font.p.qtFont);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setFont(*font.p.qtFont);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setFont(*font.p.qtFont);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setFont(*font.p.qtFont);
  }
}

void pAction::setVisible(bool visible) {
  if(dynamic_cast<Menu*>(&action)) {
    ((Menu&)action).p.qtMenu->menuAction()->setVisible(visible);
  } else if(dynamic_cast<Separator*>(&action)) {
    ((Separator&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<Item*>(&action)) {
    ((Item&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<CheckItem*>(&action)) {
    ((CheckItem&)action).p.qtAction->setVisible(visible);
  } else if(dynamic_cast<RadioItem*>(&action)) {
    ((RadioItem&)action).p.qtAction->setVisible(visible);
  }
}

void pAction::constructor() {
}
