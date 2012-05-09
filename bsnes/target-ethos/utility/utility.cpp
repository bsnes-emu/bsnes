#include "../ethos.hpp"

Utility *utility = nullptr;

void Utility::setInterface(Emulator::Interface *emulator) {
  application->active = emulator;
  presentation->synchronize();
}

void Utility::loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media) {
  string pathname = application->path({media.name, ".", media.type, "/"});
  if(!file::exists({pathname, media.path})) pathname = browser->select({"Load ", media.name}, media.extension);
  if(!file::exists({pathname, media.path})) return;
  loadMedia(emulator, media, pathname);
}

void Utility::loadMedia(Emulator::Interface *emulator, Emulator::Interface::Media &media, const string &pathname) {
  unload();
  setInterface(emulator);
  path(system().group(media.id)) = pathname;
  if(media.type == "sys") this->pathname.append(pathname);

  string manifest;
  manifest.readfile({pathname, "manifest.xml"});
  auto memory = file::read({pathname, media.path});
  system().load(media.id, vectorstream{memory}, manifest);

  if(this->pathname.size() == 0) this->pathname.append(pathname);
  presentation->setSystemName(media.name);
  load();
}

void Utility::loadMedia(unsigned id, const string &path) {
  string pathname = {this->path(system().group(id)), path};
  if(file::exists(pathname)) {
    mmapstream stream(pathname);
    return system().load(id, stream);
  }
}

void Utility::loadMedia(unsigned id, const string &name, const string &type, const string &path) {
  string pathname = browser->select({"Load ", name}, type);
  if(pathname.empty()) return;
  this->path(system().group(id)) = pathname;
  this->pathname.append(pathname);

  string markup;
  markup.readfile({pathname, "manifest.xml"});
  mmapstream stream({pathname, path});
  system().load(id, stream, markup);
}

void Utility::loadMemory() {
  for(auto &memory : system().memory) {
    string pathname = path(system().group(memory.id));
    filestream fs({pathname, memory.name});
    system().load(memory.id, fs);
  }

  cheatEditor->load({pathname[0], "cheats.xml"});
  stateManager->load({pathname[0], "bsnes/states.bsa"}, 1);
}

void Utility::saveMemory() {
  for(auto &memory : system().memory) {
    string pathname = path(system().group(memory.id));
    filestream fs({pathname, memory.name}, file::mode::write);
    system().save(memory.id, fs);
  }

  cheatEditor->save({pathname[0], "cheats.xml"});
  stateManager->save({pathname[0], "bsnes/states.bsa"}, 1);
}

void Utility::connect(unsigned port, unsigned device) {
  if(application->active == nullptr) return;
  system().connect(port, device);
}

void Utility::power() {
  if(application->active == nullptr) return;
  system().power();
}

void Utility::reset() {
  if(application->active == nullptr) return;
  system().reset();
}

void Utility::load() {
  string title;
  for(auto &path : pathname) {
    string name = path;
    name.rtrim<1>("/");
    title.append(notdir(nall::basename(name)), " + ");
  }
  title.rtrim<1>(" + ");
  presentation->setTitle(title);

  loadMemory();

  system().updatePalette();
  synchronizeDSP();

  resize();
  cheatEditor->synchronize();
  cheatEditor->refresh();
}

void Utility::unload() {
  if(application->active == nullptr) return;

  saveMemory();
  system().unload();
  path.reset();
  pathname.reset();
  cheatEditor->reset();
  stateManager->reset();
  setInterface(nullptr);

  video.clear();
  audio.clear();
  presentation->setTitle({Emulator::Name, " v", Emulator::Version});
  cheatEditor->synchronize();
  stateManager->synchronize();
}

void Utility::saveState(unsigned slot) {
  if(application->active == nullptr) return;
  serializer s = system().serialize();
  if(s.size() == 0) return;
  directory::create({pathname[0], "bsnes/"});
  if(file::write({pathname[0], "bsnes/state-", slot, ".bsa"}, s.data(), s.size()) == false);
  showMessage({"Save to slot ", slot});
}

void Utility::loadState(unsigned slot) {
  if(application->active == nullptr) return;
  auto memory = file::read({pathname[0], "bsnes/state-", slot, ".bsa"});
  if(memory.size() == 0) return showMessage({"Unable to locate slot ", slot, " state"});
  serializer s(memory.data(), memory.size());
  if(system().unserialize(s) == false) return showMessage({"Slot ", slot, " state incompatible"});
  showMessage({"Loaded from slot ", slot});
}

void Utility::synchronizeDSP() {
  if(application->active == nullptr) return;

  if(config->video.synchronize == false) {
    return dspaudio.setFrequency(system().audioFrequency());
  }

  double inputRatio = system().audioFrequency() / system().videoFrequency();
  double outputRatio = config->timing.audio / config->timing.video;
  double frequency = inputRatio / outputRatio * config->audio.frequency;

  dspaudio.setFrequency(frequency);
}

void Utility::synchronizeRuby() {
  video.set(Video::Synchronize, config->video.synchronize);
  audio.set(Audio::Synchronize, config->audio.synchronize);
  audio.set(Audio::Frequency, config->audio.frequency);
  audio.set(Audio::Latency, config->audio.latency);

  switch(config->audio.resampler) {
  case 0: dspaudio.setResampler(DSP::ResampleEngine::Linear);  break;
  case 1: dspaudio.setResampler(DSP::ResampleEngine::Hermite); break;
  case 2: dspaudio.setResampler(DSP::ResampleEngine::Sinc);    break;
  }
  dspaudio.setResamplerFrequency(config->audio.frequency);
  dspaudio.setVolume(config->audio.mute ? 0.0 : config->audio.volume * 0.01);
  synchronizeDSP();
}

void Utility::updateShader() {
  if(config->video.shader == "None") {
    video.set(Video::Shader, (const char*)"");
    video.set(Video::Filter, 0u);
    return;
  }
  if(config->video.shader == "Blur") {
    video.set(Video::Shader, (const char*)"");
    video.set(Video::Filter, 1u);
    return;
  }
  string data;
  data.readfile(config->video.shader);
  video.set(Video::Shader, (const char*)data);
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

void Utility::updateStatus() {
  time_t currentTime = time(0);
  string text;
  if((currentTime - statusTime) <= 2) {
    text = statusMessage;
  } else if(application->active == nullptr) {
    text = "No cartridge loaded";
  } else if(application->pause || application->autopause) {
    text = "Paused";
  } else {
    text = statusText;
  }
  if(text != presentation->statusText()) {
    presentation->setStatusText(text);
  }
}

void Utility::setStatusText(const string &text) {
  statusText = text;
}

void Utility::showMessage(const string &message) {
  statusTime = time(0);
  statusMessage = message;
}

Utility::Utility() {
  statusTime = 0;
}
