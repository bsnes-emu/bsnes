#include <gba/interface/interface.hpp>

void Application::bootstrap() {
  interface = new Interface;

  emulator.append(new GameBoyAdvance::Interface);

  for(auto &system : emulator) {
    system->callback.videoColor = {&Interface::videoColor, interface};
    system->callback.videoRefresh = {&Interface::videoRefresh, interface};
    system->callback.audioSample = {&Interface::audioSample, interface};
    system->callback.inputPoll = {&Interface::inputPoll, interface};
    system->updatePalette();

    string basepath = path({system->information.name, ".sys/"});

    string manifest;
    manifest.readfile({basepath, "manifest.xml"});
    XML::Document document(manifest);

    for(auto &firmware : system->firmware) {
      string path = firmware.name;
      path.lower();
      for(auto &root : document) {
        for(auto &node : root) {
          if(node.name == path) {
            filestream fs({basepath, node["firmware"].data});
            system->load(firmware.id, fs);
          }
        }
      }
    }
  }
}
