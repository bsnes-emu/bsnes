MessageDialog::MessageDialog(const string& text) {
  state.text = text;
}

auto MessageDialog::error(const lstring& buttons) -> signed {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Error;
  return _run();
}

auto MessageDialog::information(const lstring& buttons) -> signed {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Information;
  return _run();
}

auto MessageDialog::question(const lstring& buttons) -> signed {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Question;
  return _run();
}

auto MessageDialog::setParent(shared_pointer<mWindow> parent) -> type& {
  state.parent = parent;
  return *this;
}

auto MessageDialog::setText(const string& text) -> type& {
  state.text = text;
  return *this;
}

auto MessageDialog::setTitle(const string& title) -> type& {
  state.title = title;
  return *this;
}

auto MessageDialog::warning(const lstring& buttons) -> signed {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Warning;
  return _run();
}

auto MessageDialog::_run() -> signed {
  Window window;
    VerticalLayout layout{&window};
      HorizontalLayout messageLayout{&layout, Size{~0, 0}, 8};
        Canvas messageIcon{&messageLayout, Size{16, 16}, 8};
        Label messageText{&messageLayout, Size{~0, 0}};
      HorizontalLayout controlLayout{&layout, Size{~0, 0}};
        Widget controlSpacer{&controlLayout, Size{~0, 0}};

  layout.setMargin(8);
  messageIcon.setIcon(state.icon);
  messageText.setText(state.text);
  for(auto n : range(state.buttons)) {
    Button button{&controlLayout, Size{80, 0}, 8};
    button.onActivate([&, n] { state.response = n; window.setModal(false); });
    button.setText(state.buttons[n]);
    button.setFocused();  //the last button will have effective focus
  }

  signed widthMessage = 8 + 16 + 8 + Font::size(Font::sans(), state.text).width() + 8;
  signed widthButtons = 8 + state.buttons.size() * 88;
  signed width = max(320, widthMessage, widthButtons);

  window.onClose([&] { state.response = -1; window.setModal(false); });
  window.setTitle(state.title);
  window.setResizable(false);
  window.setSize({width, layout.minimumSize().height()});
  window.setCentered(state.parent);
  window.setVisible();
  window.setModal();
  window.setVisible(false);

  return state.response;
}
