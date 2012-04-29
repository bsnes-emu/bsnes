#include "../ethos.hpp"

Utility *utility = nullptr;

void Utility::setInterface(Emulator::Interface *emulator) {
  application->active = emulator;
  presentation->synchronize();
}

void Utility::loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname) {
  unload();
  setInterface(emulator);
  this->pathname = pathname;

  string manifest;
  manifest.readfile({pathname, "manifest.xml"});
  auto memory = file::read({pathname, media.name});
  system().load(media.id, vectorstream{memory}, manifest);

  for(auto &memory : system().memory) {
    filestream fs({pathname, memory.name});
    system().load(memory.id, fs);
  }
}

void Utility::saveMedia() {
  for(auto &memory : system().memory) {
    file::write({pathname, memory.name}, memory.data, memory.size);
  }
}

void Utility::power() {
  system().power();
}

void Utility::reset() {
  system().reset();
}

void Utility::unload() {
  if(application->active) {
    saveMedia();
    system().unload();
    setInterface(nullptr);
  }
  video.clear();
}

void Utility::setStatusText(const string &text) {
  presentation->setStatusText(text);
}
