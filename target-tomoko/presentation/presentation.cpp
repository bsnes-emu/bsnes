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

  systemMenu.setVisible(false);
  powerSystem.setText("Power");
  resetSystem.setText("Reset");
  unloadSystem.setText("Unload").onActivate([&] { program->unloadMedia(); });

  settingsMenu.setText("Settings");
  showConfiguration.setText("Configuration ...").onActivate([&] {
    settingsManager->setVisible();
    settingsManager->setFocused();
  });

  toolsMenu.setText("Tools");

  statusBar.setFont(Font::sans(8, "Bold"));

  onClose([&] { program->quit(); });

  setTitle({"tomoko v", Emulator::Version});
//setResizable(false);
  setSize({512, 480});
  setCentered();
}
