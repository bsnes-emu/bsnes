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

  superFamicomMenu.setText("Super Famicom");

  settingsMenu.setText("Settings");

  toolsMenu.setText("Tools");

  statusBar.setFont(Font::sans(8, "Bold"));

  onClose(&Application::quit);

  setTitle({"tomoko v", Emulator::Version});
  setResizable(false);
  setSize({640, 480});
  setCentered();
}
