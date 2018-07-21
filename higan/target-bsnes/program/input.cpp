auto Program::updateInputDriver() -> void {
  input = Input::create(settings["Input/Driver"].text());
  input->setContext(presentation->viewport.handle());

  inputManager->initialize();
  settingsWindow->input.reloadPorts();
  settingsWindow->hotkeys.reloadMappings();
  settingsWindow->advanced.updateInputDriver();

  if(!input->ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings["Input/Driver"].text(), "] input driver."
    }).error();
    settings["Input/Driver"].setValue("None");
    return updateInputDriver();
  }
}
