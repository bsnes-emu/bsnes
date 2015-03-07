#include "../tomoko.hpp"
Presentation* presentation = nullptr;

Presentation::Presentation() {
  presentation = this;

  libraryMenu.setText("Library");
  for(auto& emulator : program->emulators) {
    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      auto item = new MenuItem{&libraryMenu};
      item->setText({media.name, " ..."}).onActivate([=] {
        libraryManager->show(media.type);
      });
      loadBootableMedia.append(item);
    }
  }

  systemMenu.setText("System").setVisible(false);
  powerSystem.setText("Power");
  resetSystem.setText("Reset");
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedia(); drawSplashScreen(); });

  settingsMenu.setText("Settings");
  videoScaleMenu.setText("Video Scale");
  MenuRadioItem::group({videoScaleSmall, videoScaleNormal, videoScaleLarge});
  if(config().video.scale == "Small") videoScaleSmall.setChecked();
  if(config().video.scale == "Normal") videoScaleNormal.setChecked();
  if(config().video.scale == "Large") videoScaleLarge.setChecked();
  videoScaleSmall.setText("Small").onActivate([&] {
    config().video.scale = "Small";
    resizeViewport();
  });
  videoScaleNormal.setText("Normal").onActivate([&] {
    config().video.scale = "Normal";
    resizeViewport();
  });
  videoScaleLarge.setText("Large").onActivate([&] {
    config().video.scale = "Large";
    resizeViewport();
  });
  videoFilterMenu.setText("Video Filter");
  MenuRadioItem::group({videoFilterNone, videoFilterBlur});
  if(config().video.filter == "None") videoFilterNone.setChecked();
  if(config().video.filter == "Blur") videoFilterBlur.setChecked();
  videoFilterNone.setText("None").onActivate([&] { config().video.filter = "None"; program->updateVideoFilter(); });
  videoFilterBlur.setText("Blur").onActivate([&] { config().video.filter = "Blur"; program->updateVideoFilter(); });
  colorEmulation.setText("Color Emulation").setChecked(config().video.colorEmulation).onToggle([&] {
    config().video.colorEmulation = colorEmulation.checked();
    program->updateVideoPalette();
  });
  aspectCorrection.setText("Aspect Correction").setChecked(config().video.aspectCorrection).onToggle([&] {
    config().video.aspectCorrection = aspectCorrection.checked();
    resizeViewport();
  });
  maskOverscan.setText("Mask Overscan").onToggle([&] {
  });
  synchronizeVideo.setText("Synchronize Video").setChecked(config().video.synchronize).onToggle([&] {
    config().video.synchronize = synchronizeVideo.checked();
    video.set(Video::Synchronize, config().video.synchronize);
  });
  synchronizeAudio.setText("Synchronize Audio").setChecked(config().audio.synchronize).onToggle([&] {
    config().audio.synchronize = synchronizeAudio.checked();
    audio.set(Audio::Synchronize, config().audio.synchronize);
  });
  muteAudio.setText("Mute Audio").setChecked(config().audio.mute).onToggle([&] {
    config().audio.mute = muteAudio.checked();
    program->dsp.setVolume(config().audio.mute ? 0.0 : 1.0);
  });
  showConfiguration.setText("Configuration ...").onActivate([&] {
    settingsManager->setVisible();
    settingsManager->setFocused();
  });

  toolsMenu.setText("Tools").setVisible(false);
  saveStateMenu.setText("Save State");
  saveSlot1.setText("Slot 1").onActivate([&] {});
  saveSlot2.setText("Slot 2").onActivate([&] {});
  saveSlot3.setText("Slot 3").onActivate([&] {});
  saveSlot4.setText("Slot 4").onActivate([&] {});
  saveSlot5.setText("Slot 5").onActivate([&] {});
  loadStateMenu.setText("Load State");
  loadSlot1.setText("Slot 1").onActivate([&] {});
  loadSlot2.setText("Slot 2").onActivate([&] {});
  loadSlot3.setText("Slot 3").onActivate([&] {});
  loadSlot4.setText("Slot 4").onActivate([&] {});
  loadSlot5.setText("Slot 5").onActivate([&] {});
  stateManager.setText("State Manager").onActivate([&] {});
  cheatEditor.setText("Cheat Editor").onActivate([&] {});

  statusBar.setFont(Font::sans(8, "Bold"));

  onClose([&] { program->quit(); });

  setTitle({"tomoko v", Emulator::Version});
  setResizable(false);
  resizeViewport();
}

auto Presentation::resizeViewport() -> void {
  signed width  = 256;
  signed height = 240;

  if(program->activeEmulator) {
    width  = program->emulator().information.width;
    height = program->emulator().information.height;
  }

  if(config().video.scale == "Small" ) width *= 1, height *= 1;
  if(config().video.scale == "Normal") width *= 2, height *= 2;
  if(config().video.scale == "Large" ) width *= 4, height *= 4;
  if(config().video.aspectCorrection) {
    if(!program->activeEmulator || program->emulator().information.aspectRatio != 1.0) width = width * 5 / 4;
  }

  setSize({width, height});
  setPlacement(0.5, 0.5);
  if(!program->activeEmulator) drawSplashScreen();
}

auto Presentation::drawSplashScreen() -> void {
  uint32* output;
  unsigned length;
  if(video.lock(output, length, 256, 240)) {
    for(auto y : range(240)) {
      uint32* dp = output + y * (length >> 2);
      for(auto x : range(256)) *dp++ = 0xff000000;
    }
    video.unlock();
    video.refresh();
  }
}
