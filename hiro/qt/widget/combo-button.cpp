#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButton::construct() -> void {
  qtWidget = qtComboButton = new QtComboButton(*this);
  qtComboButton->connect(qtComboButton, SIGNAL(currentIndexChanged(int)), SLOT(onChange(int)));

  pWidget::construct();
}

auto pComboButton::destruct() -> void {
  delete qtComboButton;
  qtWidget = qtComboButton = nullptr;
}

auto pComboButton::append(sComboButtonItem item) -> void {
}

auto pComboButton::minimumSize() const -> Size {
  signed maximumWidth = 0;
  for(auto& item : state().items) {
    maximumWidth = max(maximumWidth, pFont::size(qtWidget->font(), item->state.text).width());
  }
  auto size = pFont::size(qtWidget->font(), " ");
  return {maximumWidth + 32, size.height() + 12};
}

auto pComboButton::remove(sComboButtonItem item) -> void {
}

auto pComboButton::reset() -> void {
  lock();
  while(qtComboButton->count()) qtComboButton->removeItem(0);
  unlock();
}

auto QtComboButton::onChange(int offset) -> void {
  for(auto& item : p.state().items) {
    item->state.selected = false;
  }
  if(auto item = p.self().item(offset)) {
    item->state.selected = true;
  }
  if(!p.locked()) p.self().doChange();
}

}

#endif
