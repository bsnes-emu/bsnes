#if defined(Hiro_MessageDialog)

MessageDialog::MessageDialog(const string& text) {
  state.text = text;
}

auto MessageDialog::checked() const -> bool {
  return state.checked;
}

auto MessageDialog::error(const vector<string>& buttons) -> string {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Error;
  return _run();
}

auto MessageDialog::information(const vector<string>& buttons) -> string {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Information;
  return _run();
}

auto MessageDialog::question(const vector<string>& buttons) -> string {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Question;
  return _run();
}

auto MessageDialog::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  state.relativeTo = {};
  return *this;
}

auto MessageDialog::setAlignment(sWindow relativeTo, Alignment alignment) -> type& {
  state.alignment = alignment;
  state.relativeTo = relativeTo;
  return *this;
}

auto MessageDialog::setChecked(bool checked) -> type& {
  state.checked = checked;
  return *this;
}

auto MessageDialog::setOption(const string& option) -> type& {
  state.option = option;
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

auto MessageDialog::warning(const vector<string>& buttons) -> string {
  state.buttons = buttons;
  state.icon = Icon::Prompt::Warning;
  return _run();
}

auto MessageDialog::_run() -> string {
  if(!state.buttons) return {};  //nothing to do
  Application::Namespace tr{"MessageDialog"};

  Window window;
    VerticalLayout layout{&window};
      HorizontalLayout messageLayout{&layout, Size{~0, 0}, 5_sy};
        VerticalLayout messageIconLayout{&messageLayout, Size{16_sx, ~0}, 5_sx};
          Canvas messageIcon{&messageIconLayout, Size{16_sx, 16_sy}, 0};
          Widget messageIconSpacer{&messageIconLayout, Size{16_sx, ~0}};
        Label messageText{&messageLayout, Size{~0, 0}};
      Widget optionSpacer{&layout, Size{0, 0}};
      CheckLabel optionText{&layout, Size{~0, 0}};
      HorizontalLayout controlLayout{&layout, Size{~0, 0}};
        Widget controlSpacer{&controlLayout, Size{~0, 0}};

  layout.setPadding(5_sx, 5_sy);
  image icon{state.icon};
  icon.scale(16_sx, 16_sy);
  messageIcon.setIcon(icon);
  messageText.setText(state.text);
  optionSpacer.setCollapsible().setVisible((bool)state.option);
  optionText.setCollapsible().setChecked(state.checked).setText(state.option).setVisible((bool)state.option).onToggle([&] {
    state.checked = optionText.checked();
  });
  for(auto n : range(state.buttons.size())) {
    Button button{&controlLayout, Size{80_sx, 0}, 5_sx};
    button.onActivate([&, n] {
      state.response = state.buttons[n];
      window.setModal(false);
    });
    button.setText(tr(state.buttons[n]));
    button.setFocused();  //the last button will have effective focus
  }

  int widthMessage = 5_sx + 16 + 5_sx + Font().size(state.text).width() + 5_sx;
  int widthButtons = 5_sx + state.buttons.size() * 85_sx;
  int width = max(320_sx, widthMessage, widthButtons);

  window.onClose([&] {
    //if the dialog is dismissed (escape pressed, or window manager close button clicked),
    //set the response to the last button shown (which is also the default selected button),
    //and set a flag to indicate that the dialog was dismissed to the caller.
    //note that the safest option should always be the last option in the buttons list.
    if(!state.response) {
      state.response = state.buttons.last();
      state.dismissed = true;
    }
    window.setModal(false);
  });

  window.setTitle(state.title);
  window.setResizable(false);
  window.setSize({width, layout.minimumSize().height()});
  window.setAlignment(state.relativeTo, state.alignment);
  window.setDismissable();
  window.setVisible();
  window.setModal();
  window.setVisible(false);

  return state.response;
}

#endif
