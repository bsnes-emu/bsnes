PathSettings::PathSettings(TabFrame* parent) : TabFrameItem(parent) {
  setIcon(Icon::Emblem::Folder);
  setText("Paths");

  layout.setPadding(5);
  layout.setSize({4, 6});
  layout.column(0).setAlignment(1.0);

  gamesLabel.setText("Games:");
  gamesPath.setEditable(false);
  gamesAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/Games"].setValue(location);
      refreshPaths();
    }
  });
  gamesReset.setText("Reset").onActivate([&] {
    settings["Path/Games"].setValue("");
    refreshPaths();
  });

  patchesLabel.setText("Patches:");
  patchesPath.setEditable(false);
  patchesAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/Patches"].setValue(location);
      refreshPaths();
    }
  });
  patchesReset.setText("Reset").onActivate([&] {
    settings["Path/Patches"].setValue("");
    refreshPaths();
  });

  savesLabel.setText("Saves:");
  savesPath.setEditable(false);
  savesAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/Saves"].setValue(location);
      refreshPaths();
    }
  });
  savesReset.setText("Reset").onActivate([&] {
    settings["Path/Saves"].setValue("");
    refreshPaths();
  });

  cheatsLabel.setText("Cheats:");
  cheatsPath.setEditable(false);
  cheatsAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/Cheats"].setValue(location);
      refreshPaths();
    }
  });
  cheatsReset.setText("Reset").onActivate([&] {
    settings["Path/Cheats"].setValue("");
    refreshPaths();
  });

  statesLabel.setText("States:");
  statesPath.setEditable(false);
  statesAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/States"].setValue(location);
      refreshPaths();
    }
  });
  statesReset.setText("Reset").onActivate([&] {
    settings["Path/States"].setValue("");
    refreshPaths();
  });

  screenshotsLabel.setText("Screenshots:");
  screenshotsPath.setEditable(false);
  screenshotsAssign.setText("Assign ...").onActivate([&] {
    if(auto location = BrowserDialog().setParent(*settingsWindow).selectFolder()) {
      settings["Path/Screenshots"].setValue(location);
      refreshPaths();
    }
  });
  screenshotsReset.setText("Reset").onActivate([&] {
    settings["Path/Screenshots"].setValue("");
    refreshPaths();
  });

  refreshPaths();
}

auto PathSettings::refreshPaths() -> void {
  if(auto location = settings["Path/Games"].text()) {
    gamesPath.setText(location).setForegroundColor();
  } else {
    gamesPath.setText("<last recently used>").setForegroundColor({128, 128, 128});
  }
  if(auto location = settings["Path/Patches"].text()) {
    patchesPath.setText(location).setForegroundColor();
  } else {
    patchesPath.setText("<same as loaded game>").setForegroundColor({128, 128, 128});
  }
  if(auto location = settings["Path/Saves"].text()) {
    savesPath.setText(location).setForegroundColor();
  } else {
    savesPath.setText("<same as loaded game>").setForegroundColor({128, 128, 128});
  }
  if(auto location = settings["Path/Cheats"].text()) {
    cheatsPath.setText(location).setForegroundColor();
  } else {
    cheatsPath.setText("<same as loaded game>").setForegroundColor({128, 128, 128});
  }
  if(auto location = settings["Path/States"].text()) {
    statesPath.setText(location).setForegroundColor();
  } else {
    statesPath.setText("<same as loaded game>").setForegroundColor({128, 128, 128});
  }
  if(auto location = settings["Path/Screenshots"].text()) {
    screenshotsPath.setText(location).setForegroundColor();
  } else {
    screenshotsPath.setText("<same as loaded game>").setForegroundColor({128, 128, 128});
  }
}
