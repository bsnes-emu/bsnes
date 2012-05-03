#include "../ethos.hpp"

Utility *utility = nullptr;

void Utility::setInterface(Emulator::Interface *emulator) {
  application->active = emulator;
  presentation->synchronize();
}

void Utility::loadSchema(Emulator::Interface *emulator, Emulator::Interface::Schema &schema) {
  string pathname;
  if(!schema.path.empty()) pathname = application->path(schema.path);
  if(!directory::exists(pathname)) pathname = browser->select(schema.displayname, schema.filter);
  if(!directory::exists(pathname)) return;

  loadMedia(emulator, schema, pathname);
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

  system().updatePalette();
  dspaudio.setFrequency(emulator->information.frequency);

  string displayname = pathname;
  displayname.rtrim<1>("/");
  presentation->setTitle(notdir(nall::basename(displayname)));
  presentation->setSystemName(media.displayname);
  resize();
}

void Utility::saveMedia() {
  for(auto &memory : system().memory) {
    filestream fs({pathname, memory.name}, file::mode::write);
    system().save(memory.id, fs);
  }
}

void Utility::power() {
  if(application->active == nullptr) return;
  system().power();
}

void Utility::reset() {
  if(application->active == nullptr) return;
  system().reset();
}

void Utility::unload() {
  if(application->active) {
    saveMedia();
    system().unload();
    setInterface(nullptr);
  }
  presentation->setTitle({Emulator::Name, " v", Emulator::Version});
  video.clear();
}

void Utility::resize(bool resizeWindow) {
  if(application->active == nullptr) return;
  Geometry geometry = presentation->geometry();
  unsigned width  = system().information.width;
  unsigned height = system().information.height;

  unsigned scaledWidth  = geometry.width  / width;
  unsigned scaledHeight = geometry.height / height;
  unsigned multiplier   = max(1u, min(scaledWidth, scaledHeight));

  if(config->video.aspectCorrection) {
    width *= system().information.aspectRatio;
  }

  width  *= multiplier;
  height *= multiplier;

  unsigned scaleMode = 0;

  if(config->video.scaleMode == 1) {
    width  = (double)width * ((double)geometry.height / height);
    height = geometry.height;
  }

  if(config->video.scaleMode == 2) {
    width  = geometry.width;
    height = geometry.height;
  }

  if(resizeWindow == false) {
    if(geometry.width  < width ) width  = geometry.width;
    if(geometry.height < height) height = geometry.height;

    presentation->viewport.setGeometry({
      (geometry.width - width) / 2, (geometry.height - height) / 2, width, height
    });
  } else {
    presentation->setGeometry({geometry.x, geometry.y, width, height});
    presentation->viewport.setGeometry({0, 0, width, height});
  }

  presentation->synchronize();
}

void Utility::toggleFullScreen() {
  static Geometry geometry;

  if(presentation->fullScreen() == false) {
    geometry = presentation->geometry();
    presentation->setMenuVisible(false);
    presentation->setStatusVisible(false);
    presentation->setFullScreen(true);
    input.acquire();
  } else {
    input.unacquire();
    presentation->setMenuVisible(true);
    presentation->setStatusVisible(true);
    presentation->setFullScreen(false);
    presentation->setGeometry(geometry);
  }

  resize();
}

void Utility::setStatusText(const string &text) {
  presentation->setStatusText(text);
}
