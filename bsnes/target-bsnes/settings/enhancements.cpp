auto EnhancementSettings::create() -> void {
  setCollapsible();
  setVisible(false);

  runAheadLabel.setText("Run-Ahead").setFont(Font().setBold());
  runAhead0.setText("Disabled").onActivate([&] {
    settings.emulator.runAhead.frames = 0;
  });
  runAhead1.setText("One Frame").onActivate([&] {
    settings.emulator.runAhead.frames = 1;
  });
  runAhead2.setText("Two Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 2;
  });
  runAhead3.setText("Three Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 3;
  });
  runAhead4.setText("Four Frames").onActivate([&] {
    settings.emulator.runAhead.frames = 4;
  });
  if(settings.emulator.runAhead.frames == 0) runAhead0.setChecked();
  if(settings.emulator.runAhead.frames == 1) runAhead1.setChecked();
  if(settings.emulator.runAhead.frames == 2) runAhead2.setChecked();
  if(settings.emulator.runAhead.frames == 3) runAhead3.setChecked();
  if(settings.emulator.runAhead.frames == 4) runAhead4.setChecked();
  runAheadSpacer.setColor({192, 192, 192});

  overclockingLabel.setText("Overclocking").setFont(Font().setBold());
  overclockingLayout.setSize({3, 3});
  overclockingLayout.column(0).setAlignment(1.0);
  overclockingLayout.column(1).setAlignment(0.5);

  cpuLabel.setText("CPU:");
  cpuClock.setLength(301).setPosition((settings.emulator.hack.cpu.overclock - 100)).onChange([&] {
    settings.emulator.hack.cpu.overclock = cpuClock.position() + 100;
    emulator->configure("Hacks/CPU/Overclock", settings.emulator.hack.cpu.overclock);
    cpuValue.setText({settings.emulator.hack.cpu.overclock, "%"});
  }).doChange();

  sa1Label.setText("SA-1:");
  sa1Clock.setLength(301).setPosition((settings.emulator.hack.sa1.overclock - 100)).onChange([&] {
    settings.emulator.hack.sa1.overclock = sa1Clock.position() + 100;
    emulator->configure("Hacks/SA1/Overclock", settings.emulator.hack.sa1.overclock);
    sa1Value.setText({settings.emulator.hack.sa1.overclock, "%"});
  }).doChange();

  sfxLabel.setText("SuperFX:");
  sfxClock.setLength(141).setPosition((settings.emulator.hack.superfx.overclock - 100) / 5).onChange([&] {
    settings.emulator.hack.superfx.overclock = sfxClock.position() * 5 + 100;
    emulator->configure("Hacks/SuperFX/Overclock", settings.emulator.hack.superfx.overclock);
    sfxValue.setText({settings.emulator.hack.superfx.overclock, "%"});
  }).doChange();
  note.setText("Note: Overclocking will break games, cause bugs and reduce performance.");
  overclockingSpacer.setColor({192, 192, 192});

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
  mode7Scale.append(ComboButtonItem().setText( "240p").setAttribute("multiplier", 1));
  mode7Scale.append(ComboButtonItem().setText( "480p").setAttribute("multiplier", 2));
  mode7Scale.append(ComboButtonItem().setText( "720p").setAttribute("multiplier", 3));
  mode7Scale.append(ComboButtonItem().setText( "960p").setAttribute("multiplier", 4));
  mode7Scale.append(ComboButtonItem().setText("1200p").setAttribute("multiplier", 5));
  mode7Scale.append(ComboButtonItem().setText("1440p").setAttribute("multiplier", 6));
  mode7Scale.append(ComboButtonItem().setText("1680p").setAttribute("multiplier", 7));
  mode7Scale.append(ComboButtonItem().setText("1920p").setAttribute("multiplier", 8));
  for(uint n = 1; n <= 8; n++) {
    if(settings.emulator.hack.ppu.mode7.scale == n) mode7Scale.item(n - 1).setSelected();
  }
  mode7Scale.onChange([&] {
    settings.emulator.hack.ppu.mode7.scale = mode7Scale.selected().attribute("multiplier").natural();
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
  coprocessorSpacer.setColor({192, 192, 192});

  gameLabel.setText("Game Enhancements").setFont(Font().setBold());
  hotfixes.setText("Hotfixes").setToolTip({
    "Even commercially licensed and officially released software sometimes shipped with bugs.\n"
    "This option will correct certain issues that occurred even on real hardware."
  }).setChecked(settings.emulator.hack.hotfixes).onToggle([&] {
    settings.emulator.hack.hotfixes = hotfixes.checked();
  });
  hotfixesSpacer.setColor({192, 192, 192});

  ppuModeLabel.setText("Mode Presets:").setFont(Font().setBold());
  ppuModeRequirements.setText(
    "Accuracy Mode: Maximum hardware accuracy, disables performance shortcuts and all enhancements.\n"
    "Performance Mode: Best compromise between hardware accuracy and performance."
  );
  accuracyMode.setText("Accuracy Mode").onActivate([&] {
    runAhead0.setChecked(); settings.emulator.runAhead.frames = 0;
    cpuClock.setPosition(0).doChange();
    sa1Clock.setPosition(0).doChange();
    sfxClock.setPosition(0).doChange();
    fastPPU.setChecked(false).doToggle();
    fastDSP.setChecked(false).doToggle();
    cubicInterpolation.setChecked(false).doToggle();
    coprocessorDelayedSyncOption.setChecked(false).doToggle();
    coprocessorPreferHLEOption.setChecked(false).doToggle();
    hotfixes.setChecked(false).doToggle();

    if(!emulator->loaded()) return;
    MessageDialog().setAlignment(settingsWindow).setTitle("Success").setText({
      "Accuracy Mode applied.\n"
      "You must reload the game in order for all changes to take effect."
    }).information();
  });

  performanceMode.setText("Performance Mode").onActivate([&] {
    runAhead0.setChecked(); settings.emulator.runAhead.frames = 0;
    cpuClock.setPosition(0).doChange();
    sa1Clock.setPosition(0).doChange();
    sfxClock.setPosition(0).doChange();
    fastPPU.setChecked(true).doToggle();
    deinterlace.setChecked(true).doToggle();
    noSpriteLimit.setChecked(false).doToggle();
    mode7Scale.item(0).setSelected(); emulator->configure("Hacks/PPU/Mode7/Scale", settings.emulator.hack.ppu.mode7.scale = 1);
    mode7Perspective.setChecked(true).doToggle();
    mode7Supersample.setChecked(false).doToggle();
    mode7Mosaic.setChecked(true).doToggle();
    fastDSP.setChecked(true).doToggle();
    cubicInterpolation.setChecked(false).doToggle();
    coprocessorDelayedSyncOption.setChecked(true).doToggle();
    coprocessorPreferHLEOption.setChecked(false).doToggle();
    hotfixes.setChecked(true).doToggle();

    if(!emulator->loaded()) return;
    MessageDialog().setAlignment(settingsWindow).setTitle("Success").setText({
      "Performance Mode applied.\n"
      "You must reload the game in order for all changes to take effect."
    }).information();
  });
}
