auto Program::updateInputDriver(Window parent) -> void {
  auto changed = (bool)input;
  input.create(settings.input.driver);
  input.setContext(presentation.viewport.handle());
  if(changed) {
  }

  inputManager.initialize();
  inputSettings.reloadPorts();
  hotkeySettings.reloadMappings();

  if(!input.ready()) {
    MessageDialog({
      "Error: failed to initialize [", settings.input.driver, "] input driver."
    }).setParent(parent).error();
    settings.input.driver = "None";
    return updateInputDriver(parent);
  }
}
