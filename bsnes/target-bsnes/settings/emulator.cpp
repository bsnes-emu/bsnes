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
  nativeFileDialogs.setText("Use native file dialogs").setChecked(settings.general.nativeFileDialogs).onToggle([&] {
    settings.general.nativeFileDialogs = nativeFileDialogs.checked();
  });
  screenSaver.setText("Allow screensaver during emulation").setChecked(settings.general.screenSaver).onToggle([&] {
    settings.general.screenSaver = screenSaver.checked();
  });
  optionsSpacer.setColor({192, 192, 192});

  fastForwardLabel.setText("Fast Forward").setFont(Font().setBold());

  frameSkipLabel.setText("Frame skip:").setToolTip({
    "Set how many frames to skip while fast forwarding.\n"
    "Frame skipping allows a higher maximum fast forwarding frame rate."
  });

  frameSkipAmount.append(ComboButtonItem().setText("None"));
  frameSkipAmount.append(ComboButtonItem().setText("1 frame"));
  frameSkipAmount.append(ComboButtonItem().setText("2 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("3 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("4 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("5 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("6 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("7 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("8 frames"));
  frameSkipAmount.append(ComboButtonItem().setText("9 frames"));
  frameSkipAmount.item(settings.fastForward.frameSkip).setSelected();
  frameSkipAmount.onChange([&] {
    settings.fastForward.frameSkip = frameSkipAmount.selected().offset();
  });

  limiterLabel.setText("Limiter:").setToolTip({
    "Set the maximum speed when fast forwarding."
  });

  limiterAmount.append(ComboButtonItem().setText("None"));
  limiterAmount.append(ComboButtonItem().setText("200%"));
  limiterAmount.append(ComboButtonItem().setText("300%"));
  limiterAmount.append(ComboButtonItem().setText("400%"));
  limiterAmount.append(ComboButtonItem().setText("500%"));
  limiterAmount.append(ComboButtonItem().setText("600%"));
  limiterAmount.append(ComboButtonItem().setText("700%"));
  limiterAmount.append(ComboButtonItem().setText("800%"));
  if(settings.fastForward.limiter == 0) limiterAmount.item(0).setSelected();
  if(settings.fastForward.limiter == 2) limiterAmount.item(1).setSelected();
  if(settings.fastForward.limiter == 3) limiterAmount.item(2).setSelected();
  if(settings.fastForward.limiter == 4) limiterAmount.item(3).setSelected();
  if(settings.fastForward.limiter == 5) limiterAmount.item(4).setSelected();
  if(settings.fastForward.limiter == 6) limiterAmount.item(5).setSelected();
  if(settings.fastForward.limiter == 7) limiterAmount.item(6).setSelected();
  if(settings.fastForward.limiter == 8) limiterAmount.item(7).setSelected();
  limiterAmount.onChange([&] {
    auto index = limiterAmount.selected().offset();
    if(index == 0) settings.fastForward.limiter = 0;
    if(index == 1) settings.fastForward.limiter = 2;
    if(index == 2) settings.fastForward.limiter = 3;
    if(index == 3) settings.fastForward.limiter = 4;
    if(index == 4) settings.fastForward.limiter = 5;
    if(index == 5) settings.fastForward.limiter = 6;
    if(index == 6) settings.fastForward.limiter = 7;
    if(index == 7) settings.fastForward.limiter = 8;
  });

  fastForwardMute.setText("Mute while fast forwarding").setChecked(settings.fastForward.mute).onToggle([&] {
    settings.fastForward.mute = fastForwardMute.checked();
  });

  fastForwardSpacer.setColor({192, 192, 192});

  rewindLabel.setText("Rewind").setFont(Font().setBold());

  rewindFrequencyLabel.setText("Frequency:");
  rewindFrequencyOption.append(ComboButtonItem().setText("Disabled"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 10 frames"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 20 frames"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 30 frames"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 40 frames"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 50 frames"));
  rewindFrequencyOption.append(ComboButtonItem().setText("Every 60 frames"));
  if(settings.rewind.frequency ==  0) rewindFrequencyOption.item(0).setSelected();
  if(settings.rewind.frequency == 10) rewindFrequencyOption.item(1).setSelected();
  if(settings.rewind.frequency == 20) rewindFrequencyOption.item(2).setSelected();
  if(settings.rewind.frequency == 30) rewindFrequencyOption.item(3).setSelected();
  if(settings.rewind.frequency == 40) rewindFrequencyOption.item(4).setSelected();
  if(settings.rewind.frequency == 50) rewindFrequencyOption.item(5).setSelected();
  if(settings.rewind.frequency == 60) rewindFrequencyOption.item(6).setSelected();
  rewindFrequencyOption.onChange([&] {
    settings.rewind.frequency = rewindFrequencyOption.selected().offset() * 10;
    program.rewindReset();
  });

  rewindLengthLabel.setText("Length:");
  rewindLengthOption.append(ComboButtonItem().setText( "10 states"));
  rewindLengthOption.append(ComboButtonItem().setText( "20 states"));
  rewindLengthOption.append(ComboButtonItem().setText( "40 states"));
  rewindLengthOption.append(ComboButtonItem().setText( "80 states"));
  rewindLengthOption.append(ComboButtonItem().setText("160 states"));
  rewindLengthOption.append(ComboButtonItem().setText("320 states"));
  if(settings.rewind.length ==  10) rewindLengthOption.item(0).setSelected();
  if(settings.rewind.length ==  20) rewindLengthOption.item(1).setSelected();
  if(settings.rewind.length ==  40) rewindLengthOption.item(2).setSelected();
  if(settings.rewind.length ==  80) rewindLengthOption.item(3).setSelected();
  if(settings.rewind.length == 160) rewindLengthOption.item(4).setSelected();
  if(settings.rewind.length == 320) rewindLengthOption.item(5).setSelected();
  rewindLengthOption.onChange([&] {
    settings.rewind.length = 10 << rewindLengthOption.selected().offset();
    program.rewindReset();
  });

  rewindMute.setText("Mute while rewinding").setChecked(settings.rewind.mute).onToggle([&] {
    settings.rewind.mute = rewindMute.checked();
  });
}
