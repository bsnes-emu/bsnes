AdvancedSettings::AdvancedSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Action::Settings);
  setText("Advanced");

  layout.setMargin(5);
  driversLabel.setText("Drivers").setFont(Font().setBold());

  videoDriverLabel.setText("Video:");
  for(auto& driver : Video::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    videoDriverOption.append(item);
    if(video->driver() == driver) item.setSelected();
  }
  videoDriverOption.onChange([&] {
    auto item = videoDriverOption.selected();
    settings["Video/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the video driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveRecoveryState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->initializeVideoDriver();
      if(!video->ready()) {
        MessageDialog({
          "Error: failed to initialize ", item.text(), " video driver."
        }).setParent(*settingsWindow).error();
        settings["Video/Driver"].setValue("None");
        program->initializeVideoDriver();
        for(auto item : videoDriverOption.items()) {
          if(video->driver() == item.text()) item.setSelected();
        }
      }
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  audioDriverLabel.setText("Audio:");
  for(auto& driver : Audio::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    audioDriverOption.append(item);
    if(audio->driver() == driver) item.setSelected();
  }
  audioDriverOption.onChange([&] {
    auto item = audioDriverOption.selected();
    settings["Audio/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the audio driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveRecoveryState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->initializeAudioDriver();
      if(!audio->ready()) {
        MessageDialog({
          "Error: failed to initialize ", item.text(), " audio driver."
        }).setParent(*settingsWindow).error();
        settings["Audio/Driver"].setValue("None");
        program->initializeAudioDriver();
        for(auto item : audioDriverOption.items()) {
          if(audio->driver() == item.text()) item.setSelected();
        }
      }
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  inputDriverLabel.setText("Input:");
  for(auto& driver : Input::availableDrivers()) {
    ComboButtonItem item;
    item.setText(driver);
    inputDriverOption.append(item);
    if(input->driver() == driver) item.setSelected();
  }
  inputDriverOption.onChange([&] {
    auto item = inputDriverOption.selected();
    settings["Input/Driver"].setValue(item.text());
    if(!emulator->loaded() || item.text() == "None" || MessageDialog(
      "Warning: incompatible drivers may cause bsnes to crash.\n"
      "It is highly recommended you unload your game first to avoid data loss.\n"
      "Do you wish to proceed with the input driver change now anyway?"
    ).setParent(*settingsWindow).question() == "Yes") {
      program->save();
      program->saveRecoveryState();
      settings["Crashed"].setValue(true);
      settings.save();
      program->initializeInputDriver();
      if(!input->ready()) {
        MessageDialog({
          "Error: failed to initialize ", item.text(), " input driver."
        }).setParent(*settingsWindow).error();
        settings["Input/Driver"].setValue("None");
        program->initializeInputDriver();
        for(auto item : inputDriverOption.items()) {
          if(input->driver() == item.text()) item.setSelected();
        }
      }
      settings["Crashed"].setValue(false);
      settings.save();
    }
  });

  hacksLabel.setText("Emulator Hacks").setFont(Font().setBold());
  fastPPUOption.setText("Fast PPU").setChecked(settings["Emulator/Hack/FastPPU"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastPPU"].setValue(fastPPUOption.checked());
  });
  fastDSPOption.setText("Fast DSP").setChecked(settings["Emulator/Hack/FastDSP"].boolean()).onToggle([&] {
    settings["Emulator/Hack/FastDSP"].setValue(fastDSPOption.checked());
  });
  superFXLabel.setText("SuperFX Clock Speed:");
  superFXValue.setAlignment(0.5);
  superFXClock.setLength(71).setPosition((settings["Emulator/Hack/FastSuperFX"].natural() - 100) / 10).onChange([&] {
    settings["Emulator/Hack/FastSuperFX"].setValue({superFXClock.position() * 10 + 100, "%"});
    superFXValue.setText(settings["Emulator/Hack/FastSuperFX"].text());
  }).doChange();
  hacksNote.setFont(Font().setItalic()).setText("Note: hack setting changes do not take effect until after reloading games.");
}
