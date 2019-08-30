auto EmulatorSettings::create() -> void {
  setCollapsible();
  setVisible(false);

  optionsLabel.setText("Options").setFont(Font().setBold());
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

  entropyLabel.setText("Entropy (randomness)").setFont(Font().setBold());
  entropyNone.setText("None").setToolTip(
    "All memory and registers are initialized to constant values at startup.\n"
    "Use this for movie recording and compatibility with very old demoscene homebrew games."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "None";
  });
  entropyLow.setText("Low").setToolTip(
    "All memory is randomized with repeating patterns, all registers are randomized at startup.\n"
    "Use this for the most accurate representation of a real SNES."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "Low";
  });
  entropyHigh.setText("High").setToolTip(
    "All memory and registers are randomized as much as possible.\n"
    "Use this when developing new SNES software to ensure maximum compatibility with real hardware."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "High";
  });
  if(settings.emulator.hack.entropy == "None") entropyNone.setChecked();
  if(settings.emulator.hack.entropy == "Low") entropyLow.setChecked();
  if(settings.emulator.hack.entropy == "High") entropyHigh.setChecked();

  ppuLabel.setText("PPU (video)").setFont(Font().setBold());
  fastPPU.setText("Fast mode").setChecked(settings.emulator.hack.ppu.fast).onToggle([&] {
    settings.emulator.hack.ppu.fast = fastPPU.checked();
    if(!fastPPU.checked()) {
      noSpriteLimit.setEnabled(false);
      deinterlace.setEnabled(false);
      mode7Layout.setEnabled(false);
    } else {
      noSpriteLimit.setEnabled(true);
      deinterlace.setEnabled(true);
      mode7Layout.setEnabled(true);
    }
  }).doToggle();
  deinterlace.setText("Deinterlace").setChecked(settings.emulator.hack.ppu.deinterlace).onToggle([&] {
    settings.emulator.hack.ppu.deinterlace = deinterlace.checked();
    emulator->configure("Hacks/PPU/Deinterlace", settings.emulator.hack.ppu.deinterlace);
  });
  noSpriteLimit.setText("No sprite limit").setChecked(settings.emulator.hack.ppu.noSpriteLimit).onToggle([&] {
    settings.emulator.hack.ppu.noSpriteLimit = noSpriteLimit.checked();
  });
  mode7Label.setText("HD Mode 7 (fast PPU only)").setFont(Font().setBold());
  mode7ScaleLabel.setText("Scale:");
  mode7Scale.append(ComboButtonItem().setText( "240p").setProperty("multiplier", 1));
  mode7Scale.append(ComboButtonItem().setText( "480p").setProperty("multiplier", 2));
  mode7Scale.append(ComboButtonItem().setText( "720p").setProperty("multiplier", 3));
  mode7Scale.append(ComboButtonItem().setText( "960p").setProperty("multiplier", 4));
  mode7Scale.append(ComboButtonItem().setText("1200p").setProperty("multiplier", 5));
  mode7Scale.append(ComboButtonItem().setText("1440p").setProperty("multiplier", 6));
  mode7Scale.append(ComboButtonItem().setText("1680p").setProperty("multiplier", 7));
  mode7Scale.append(ComboButtonItem().setText("1920p").setProperty("multiplier", 8));
  mode7Scale.append(ComboButtonItem().setText("2160p").setProperty("multiplier", 9));
  for(uint n = 1; n <= 9; n++) {
    if(settings.emulator.hack.ppu.mode7.scale == n) mode7Scale.item(n - 1).setSelected();
  }
  mode7Scale.onChange([&] {
    settings.emulator.hack.ppu.mode7.scale = mode7Scale.selected().property("multiplier").natural();
    emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale);
  });
  mode7Perspective.setText("Perspective correction").setChecked(settings.emulator.hack.ppu.mode7.perspective).onToggle([&] {
    settings.emulator.hack.ppu.mode7.perspective = mode7Perspective.checked();
    emulator->configure("Hacks/PPU/Mode7/Perspective", settings.emulator.hack.ppu.mode7.perspective);
  });
  mode7Supersample.setText("Supersampling").setChecked(settings.emulator.hack.ppu.mode7.supersample).onToggle([&] {
    settings.emulator.hack.ppu.mode7.supersample = mode7Supersample.checked();
    emulator->configure("Hacks/PPU/Mode7/Supersample", settings.emulator.hack.ppu.mode7.supersample);
  });
  mode7Mosaic.setText("HD->SD Mosaic").setChecked(settings.emulator.hack.ppu.mode7.mosaic).onToggle([&] {
    settings.emulator.hack.ppu.mode7.mosaic = mode7Mosaic.checked();
    emulator->configure("Hacks/PPU/Mode7/Mosaic", settings.emulator.hack.ppu.mode7.mosaic);
  });
  dspLabel.setText("DSP (audio)").setFont(Font().setBold());
  fastDSP.setText("Fast mode").setChecked(settings.emulator.hack.dsp.fast).onToggle([&] {
    settings.emulator.hack.dsp.fast = fastDSP.checked();
    emulator->configure("Hacks/DSP/Fast", settings.emulator.hack.dsp.fast);
  });
  cubicInterpolation.setText("Cubic interpolation").setChecked(settings.emulator.hack.dsp.cubic).onToggle([&] {
    settings.emulator.hack.dsp.cubic = cubicInterpolation.checked();
    emulator->configure("Hacks/DSP/Cubic", settings.emulator.hack.dsp.cubic);
  });
  coprocessorLabel.setText("Coprocessors").setFont(Font().setBold());
  coprocessorDelayedSyncOption.setText("Fast mode").setChecked(settings.emulator.hack.coprocessor.delayedSync).onToggle([&] {
    settings.emulator.hack.coprocessor.delayedSync = coprocessorDelayedSyncOption.checked();
  });
  coprocessorPreferHLEOption.setText("Prefer HLE").setChecked(settings.emulator.hack.coprocessor.preferHLE).setToolTip(
    "When checked, less accurate HLE emulation will always be used when available.\n"
    "When unchecked, HLE will only be used when LLE firmware is missing."
  ).onToggle([&] {
    settings.emulator.hack.coprocessor.preferHLE = coprocessorPreferHLEOption.checked();
  });
  hacksNote.setText("Note: some hack setting changes do not take effect until after reloading games.");
}
