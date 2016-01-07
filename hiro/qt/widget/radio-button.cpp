#if defined(Hiro_RadioButton)

namespace hiro {

auto pRadioButton::construct() -> void {
  qtWidget = qtRadioButton = new QtRadioButton(*this);
  qtRadioButton->setCheckable(true);
  qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  qtRadioButton->connect(qtRadioButton, SIGNAL(toggled(bool)), SLOT(onActivate()));

  pWidget::construct();
  setGroup(state().group);
  _setState();
}

auto pRadioButton::destruct() -> void {
  if(qtRadioButton) delete qtRadioButton;
  qtWidget = qtRadioButton = nullptr;
}

auto pRadioButton::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text);

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().icon.width());
    size.setHeight(max(size.height(), state().icon.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width()));
    size.setHeight(size.height() + state().icon.height());
  }

  return {size.width() + 20, size.height() + 12};
}

auto pRadioButton::setBordered(bool bordered) -> void {
}

auto pRadioButton::setChecked() -> void {
  _setState();
}

auto pRadioButton::setGroup(sGroup group) -> void {
  bool first = true;
  if(auto& group = state().group) {
    group->self()->lock();
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioButton = dynamic_cast<mRadioButton*>(object.data())) {
          if(auto self = radioButton->self()) {
            self->qtRadioButton->setChecked(radioButton->state.checked = first);
            first = false;
          }
        }
      }
    }
    group->self()->unlock();
  }
}

auto pRadioButton::setIcon(const image& icon) -> void {
  _setState();
}

auto pRadioButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pRadioButton::setText(const string& text) -> void {
  _setState();
}

auto pRadioButton::_setState() -> void {
  if(auto& group = state().group) {
    group->self()->lock();
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioButton = dynamic_cast<mRadioButton*>(object.data())) {
          if(auto self = radioButton->self()) {
            self->qtRadioButton->setChecked(radioButton->state.checked);
          }
        }
      }
    }
    group->self()->unlock();
  }
  qtRadioButton->setIconSize(QSize(state().icon.width(), state().icon.height()));
  qtRadioButton->setIcon(CreateIcon(state().icon));
  qtRadioButton->setStyleSheet("text-align: top;");
  switch(state().orientation) {
  case Orientation::Horizontal: qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); break;
  case Orientation::Vertical:   qtRadioButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  break;
  }
  qtRadioButton->setText(QString::fromUtf8(state().text));
}

auto QtRadioButton::onActivate() -> void {
  if(auto& group = p.state().group) {
    if(!group->self()->locked()) {
      bool wasChecked = p.state().checked;
      p.self().setChecked();
      if(!wasChecked) p.self().doActivate();
    }
  }
}

}

#endif
