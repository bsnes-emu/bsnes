auto Program::updateInputDriver(Window parent) -> void {
  auto changed = (bool)input;
  input = Input::create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());
  if(changed) {
  }

  inputManager->initialize();
  settingsWindow->input.reloadPorts();
  settingsWindow->hotkeys.reloadMappings();

  if(!input->ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Input/Driver"].text(), "] input driver."
    }).setParent(parent).error();
    settings["Input/Driver"].setValue("None");
    return updateInputDriver(parent);
  }
}
