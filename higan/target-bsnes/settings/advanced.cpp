AdvancedSettings::AdvancedSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Action::Settings);
  setText("Advanced");

  layout.setMargin(5);
  driversLabel.setText("Drivers").setFont(Font().setBold());

  videoDriverLabel.setText("Video:");
  videoDriverOption.onChange([&] {
    auto item = videoDriverOption.selected();
    settings["Video/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the video driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveUndoState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->updateVideoDriver();
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  audioDriverLabel.setText("Audio:");
  audioDriverOption.onChange([&] {
    auto item = audioDriverOption.selected();
    settings["Audio/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the audio driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveUndoState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->updateAudioDriver();
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  inputDriverLabel.setText("Input:");
  inputDriverOption.onChange([&] {
    auto item = inputDriverOption.selected();
    settings["Input/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the input driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveUndoState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->updateInputDriver();
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  optionsLabel.setText("Options").setFont(Font().setBold());
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
    emulator->set("Fast PPU/Hires Mode 7", hiresMode7.checked());
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

auto AdvancedSettings::updateVideoDriver() -> void {
  videoDriverOption.reset();
  for(auto& driver : Video::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    videoDriverOption.append(item);
    if(video && video->driver() == driver) item.setSelected();
  }
}

auto AdvancedSettings::updateAudioDriver() -> void {
  audioDriverOption.reset();
  for(auto& driver : Audio::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    audioDriverOption.append(item);
    if(audio && audio->driver() == driver) item.setSelected();
  }
}

auto AdvancedSettings::updateInputDriver() -> void {
  inputDriverOption.reset();
  for(auto& driver : Input::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    inputDriverOption.append(item);
    if(input && input->driver() == driver) item.setSelected();
  }
}
