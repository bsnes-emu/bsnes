auto mRadioLabel::allocate() -> pObject* {
  return new pRadioLabel(*this);
}

//

auto mRadioLabel::group(const vector<shared_pointer_weak<mRadioLabel>>& group) -> void {
  for(auto& weak : group) {
    if(auto item = weak.acquire()) item->state.group = group;
  }
  for(auto& weak : group) {
    if(auto item = weak.acquire()) {
      if(item->self()) item->self()->setGroup(group);
    }
  }
  if(group.size()) {
    if(auto item = group.first().acquire()) item->setChecked();
  }
}

auto mRadioLabel::checked() const -> bool {
  return state.checked;
}

auto mRadioLabel::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mRadioLabel::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mRadioLabel::setChecked() -> type& {
  for(auto& weak : state.group) {
    if(auto item = weak.acquire()) item->state.checked = false;
  }
  state.checked = true;
  signal(setChecked);
  return *this;
}

auto mRadioLabel::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mRadioLabel::text() const -> string {
  return state.text;
}
