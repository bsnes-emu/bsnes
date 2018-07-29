ConfigurationSettings::ConfigurationSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Action::Settings);
  setText("Configuration");

  layout.setPadding(5);

  optionsLabel.setText("Options").setFont(Font().setBold());
  inputFocusLabel.setText("When focus is lost:");
  pauseEmulation.setText("Pause emulation").onActivate([&] {
    settings["Input/Defocus"].setValue("Pause");
  });
  blockInput.setText("Block input").onActivate([&] {
    settings["Input/Defocus"].setValue("Block");
  });
  allowInput.setText("Allow input").onActivate([&] {
    settings["Input/Defocus"].setValue("Allow");
  });
  if(settings["Input/Defocus"].text() == "Pause") pauseEmulation.setChecked();
  if(settings["Input/Defocus"].text() == "Block") blockInput.setChecked();
  if(settings["Input/Defocus"].text() == "Allow") allowInput.setChecked();
  warnOnUnverifiedGames.setText("Warn when loading games that have not been verified").setChecked(settings["Emulator/WarnOnUnverifiedGames"].boolean()).onToggle([&] {
    settings["Emulator/WarnOnUnverifiedGames"].setValue(warnOnUnverifiedGames.checked());
  });
  autoSaveMemory.setText("Auto-save memory periodically").setChecked(settings["Emulator/AutoSaveMemory/Enable"].boolean()).onToggle([&] {
    settings["Emulator/AutoSaveMemory/Enable"].setValue(autoSaveMemory.checked());
  });
  autoSaveStateOnUnload.setText("Auto-save undo state when unloading games").setChecked(settings["Emulator/AutoSaveStateOnUnload"].boolean()).onToggle([&] {
    settings["Emulator/AutoSaveStateOnUnload"].setValue(autoSaveStateOnUnload.checked());
    if(!autoSaveStateOnUnload.checked()) {
      autoLoadStateOnLoad.setEnabled(false).setChecked(false).doToggle();
    } else {
      autoLoadStateOnLoad.setEnabled(true);
    }
  }).doToggle();
  autoLoadStateOnLoad.setText("Auto-resume on load").setChecked(settings["Emulator/AutoLoadStateOnLoad"].boolean()).onToggle([&] {
    settings["Emulator/AutoLoadStateOnLoad"].setValue(autoLoadStateOnLoad.checked());
  });
  suppressScreenSaver.setText("Suppress screen saver").setChecked(settings["UserInterface/SuppressScreenSaver"].boolean()).onToggle([&] {
    settings["UserInterface/SuppressScreenSaver"].setValue(suppressScreenSaver.checked());
    Application::setScreenSaver(!suppressScreenSaver.checked());
  });

  hacksLabel.setText("Emulator Hacks").setFont(Font().setBold());
  fastPPUOption.setText("Fast PPU").setChecked(settings["Emulator/Hack/FastPPU"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastPPU"].setValue(fastPPUOption.checked());
    if(!fastPPUOption.checked()) {
      noSpriteLimit.setEnabled(false).setChecked(false).doToggle();
      hiresMode7.setEnabled(false).setChecked(false).doToggle();
    } else {
      noSpriteLimit.setEnabled(true);
      hiresMode7.setEnabled(true);
    }
  }).doToggle();
  noSpriteLimit.setText("No sprite limit").setChecked(settings["Emulator/Hack/FastPPU/NoSpriteLimit"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastPPU/NoSpriteLimit"].setValue(noSpriteLimit.checked());
  });
  hiresMode7.setText("Hires mode 7").setChecked(settings["Emulator/Hack/FastPPU/HiresMode7"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastPPU/HiresMode7"].setValue(hiresMode7.checked());
  });
  fastDSPOption.setText("Fast DSP").setChecked(settings["Emulator/Hack/FastDSP"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastDSP"].setValue(fastDSPOption.checked());
  });
  superFXLabel.setText("SuperFX clock speed:");
  superFXValue.setAlignment(0.5);
  superFXClock.setLength(71).setPosition((settings["Emulator/Hack/FastSuperFX"].natural() - 100) / 10).onChange([&] {
    settings["Emulator/Hack/FastSuperFX"].setValue({superFXClock.position() * 10 + 100, "%"});
    superFXValue.setText(settings["Emulator/Hack/FastSuperFX"].text());
  }).doChange();
  hacksNote.setForegroundColor({224, 0, 0}).setText("Note: some hack setting changes do not take effect until after reloading games.");
}

auto ConfigurationSettings::updateConfiguration() -> void {
  emulator->configure("hacks/ppuFast/enable", fastPPUOption.checked());
  emulator->configure("hacks/ppuFast/noSpriteLimit", noSpriteLimit.checked());
  emulator->configure("hacks/ppuFast/hiresMode7", hiresMode7.checked());
  emulator->configure("hacks/dspFast/enable", fastDSPOption.checked());
}
