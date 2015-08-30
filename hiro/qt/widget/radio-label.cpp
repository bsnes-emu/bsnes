#if defined(Hiro_RadioLabel)

namespace hiro {

auto pRadioLabel::construct() -> void {
  qtWidget = qtRadioLabel = new QtRadioLabel(*this);
  qtRadioLabel->setAutoExclusive(false);
  qtRadioLabel->connect(qtRadioLabel, SIGNAL(toggled(bool)), SLOT(onActivate()));

  pWidget::construct();
  setGroup(state().group);
  _setState();
}

auto pRadioLabel::destruct() -> void {
  if(qtRadioLabel) delete qtRadioLabel;
  qtWidget = qtRadioLabel = nullptr;
}

auto pRadioLabel::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text);
  return {size.width() + 26, size.height() + 6};
}

auto pRadioLabel::setChecked() -> void {
  _setState();
}

auto pRadioLabel::setGroup(sGroup group) -> void {
  bool first = true;
  if(auto& group = state().group) {
    group->self()->lock();
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioLabel = dynamic_cast<mRadioLabel*>(object.data())) {
          if(auto self = radioLabel->self()) {
            self->qtRadioLabel->setChecked(radioLabel->state.checked = first);
            first = false;
          }
        }
      }
    }
    group->self()->unlock();
  }
}

auto pRadioLabel::setText(const string& text) -> void {
  _setState();
}

auto pRadioLabel::_setState() -> void {
  if(auto& group = state().group) {
    group->self()->lock();
    for(auto& weak : group->state.objects) {
      if(auto object = weak.acquire()) {
        if(auto radioLabel = dynamic_cast<mRadioLabel*>(object.data())) {
          if(auto self = radioLabel->self()) {
            self->qtRadioLabel->setChecked(radioLabel->state.checked);
          }
        }
      }
    }
    group->self()->unlock();
  }
  qtRadioLabel->setText(QString::fromUtf8(state().text));
}

auto QtRadioLabel::onActivate() -> void {
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
