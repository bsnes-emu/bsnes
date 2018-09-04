auto EmulatorSettings::create() -> void {
  setIcon(Icon::Action::Settings);
  setText("Emulator");

  layout.setPadding(5);

  optionsLabel.setText("Options").setFont(Font().setBold());
  inputFocusLabel.setText("When focus is lost:");
  pauseEmulation.setText("Pause emulation").onActivate([&] {
    settings.input.defocus = "Pause";
  });
  blockInput.setText("Block input").onActivate([&] {
    settings.input.defocus = "Block";
  });
  allowInput.setText("Allow input").onActivate([&] {
    settings.input.defocus = "Allow";
  });
  if(settings.input.defocus == "Pause") pauseEmulation.setChecked();
  if(settings.input.defocus == "Block") blockInput.setChecked();
  if(settings.input.defocus == "Allow") allowInput.setChecked();
  warnOnUnverifiedGames.setText("Warn when loading games that have not been verified").setChecked(settings.emulator.warnOnUnverifiedGames).onToggle([&] {
    settings.emulator.warnOnUnverifiedGames = warnOnUnverifiedGames.checked();
  });
  autoSaveMemory.setText("Auto-save memory periodically").setChecked(settings.emulator.autoSaveMemory.enable).onToggle([&] {
    settings.emulator.autoSaveMemory.enable = autoSaveMemory.checked();
  });
  autoSaveStateOnUnload.setText("Auto-save undo state when unloading games").setChecked(settings.emulator.autoSaveStateOnUnload).onToggle([&] {
    settings.emulator.autoSaveStateOnUnload = autoSaveStateOnUnload.checked();
    if(!autoSaveStateOnUnload.checked()) {
      autoLoadStateOnLoad.setEnabled(false).setChecked(false).doToggle();
    } else {
      autoLoadStateOnLoad.setEnabled(true);
    }
  }).doToggle();
  autoLoadStateOnLoad.setText("Auto-resume on load").setChecked(settings.emulator.autoLoadStateOnLoad).onToggle([&] {
    settings.emulator.autoLoadStateOnLoad = autoLoadStateOnLoad.checked();
  });
  optionsSpacer.setColor({192, 192, 192});

  hacksLabel.setText("Hacks").setFont(Font().setBold());
  fastPPUOption.setText("Fast PPU").setChecked(settings.emulator.hack.fastPPU.enable).onToggle([&] {
    settings.emulator.hack.fastPPU.enable = fastPPUOption.checked();
    if(!fastPPUOption.checked()) {
      noSpriteLimit.setEnabled(false).setChecked(false).doToggle();
      hiresMode7.setEnabled(false).setChecked(false).doToggle();
    } else {
      noSpriteLimit.setEnabled(true);
      hiresMode7.setEnabled(true);
    }
  }).doToggle();
  noSpriteLimit.setText("No sprite limit").setChecked(settings.emulator.hack.fastPPU.noSpriteLimit).onToggle([&] {
    settings.emulator.hack.fastPPU.noSpriteLimit = noSpriteLimit.checked();
  });
  hiresMode7.setText("Hires mode 7").setChecked(settings.emulator.hack.fastPPU.hiresMode7).onToggle([&] {
    settings.emulator.hack.fastPPU.hiresMode7 = hiresMode7.checked();
  });
  fastDSPOption.setText("Fast DSP").setChecked(settings.emulator.hack.fastDSP.enable).onToggle([&] {
    settings.emulator.hack.fastDSP.enable = fastDSPOption.checked();
  });
  coprocessorsDelayedSyncOption.setText("Fast coprocessors (delayed sync)").setChecked(settings.emulator.hack.coprocessors.delayedSync).onToggle([&] {
    settings.emulator.hack.coprocessors.delayedSync = coprocessorsDelayedSyncOption.checked();
  });
  superFXLabel.setText("SuperFX clock speed:");
  superFXValue.setAlignment(0.5);
  superFXClock.setLength(71).setPosition((settings.emulator.hack.fastSuperFX - 100) / 10).onChange([&] {
    settings.emulator.hack.fastSuperFX = superFXClock.position() * 10 + 100;
    superFXValue.setText({settings.emulator.hack.fastSuperFX, "%"});
  }).doChange();
  hacksNote.setForegroundColor({224, 0, 0}).setText("Note: some hack setting changes do not take effect until after reloading games.");
}

auto EmulatorSettings::updateConfiguration() -> void {
  emulator->configure("Hacks/FastPPU/Enable", fastPPUOption.checked());
  emulator->configure("Hacks/FastPPU/NoSpriteLimit", noSpriteLimit.checked());
  emulator->configure("Hacks/FastPPU/HiresMode7", hiresMode7.checked());
  emulator->configure("Hacks/FastDSP/Enable", fastDSPOption.checked());
  emulator->configure("Hacks/Coprocessor/DelayedSync", coprocessorsDelayedSyncOption.checked());
}
