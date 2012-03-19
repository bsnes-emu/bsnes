#include "../base.hpp"
#include "palette.cpp"
#include "nes/nes.cpp"
#include "snes/snes.cpp"
#include "gb/gb.cpp"
Interface *interface = nullptr;

Filter filter;

void Filter::render(const uint32_t *input, unsigned inputPitch, unsigned inputWidth, unsigned inputHeight) {
  width = inputWidth, height = inputHeight;
  dl_size(width, height);
  dl_render(data, pitch, input, inputPitch, inputWidth, inputHeight);
}

Filter::Filter() {
  data = new uint32_t[2048 * 2048];
  pitch = 2048 * sizeof(uint32_t);
}

Filter::~Filter() {
  delete[] data;
}

string CartridgePath::filename(const string &folderName, const string &fileName) const {
  if(name.empty()) return "";
  if(folder) return { name, folderName };
  return { name, fileName };
}

string CartridgePath::title() const {
  if(name.empty()) return "";
  if(folder) {
    string title = name;
    title.rtrim<1>("/");
    title = notdir(nall::basename(title));
    return title;
  }
  return notdir(nall::basename(name));
}

void Interface::bindControllers() {
  switch(mode()) {
  case Mode::NES:
    nes.setController(0, config->nes.controllerPort1Device);
    nes.setController(1, config->nes.controllerPort2Device);
    break;

  case Mode::SNES:
    snes.setController(0, config->snes.controllerPort1Device);
    snes.setController(1, config->snes.controllerPort2Device);
    break;
  }
}

void Interface::setController(unsigned port, unsigned device) {
  switch(mode()) {
  case Mode::NES: return nes.setController(port, device);
  case Mode::SNES: return snes.setController(port, device);
  }
}

void Interface::updateDSP() {
  audio.set(Audio::Frequency, config->audio.frequency);
  audio.set(Audio::Latency, config->audio.latency);

  if(config->audio.resampler == "linear" ) dspaudio.setResampler(DSP::ResampleEngine::Linear);
  if(config->audio.resampler == "hermite") dspaudio.setResampler(DSP::ResampleEngine::Hermite);
  if(config->audio.resampler == "sinc"   ) dspaudio.setResampler(DSP::ResampleEngine::Sinc);
  dspaudio.setResamplerFrequency(config->audio.frequency);
  dspaudio.setVolume(config->audio.mute == false ? (double)config->audio.volume / 100.0 : 0.0);

  switch(mode()) {
  case Mode::NES:  return dspaudio.setFrequency(config->audio.frequencyNES);
  case Mode::SNES: return dspaudio.setFrequency(config->audio.frequencySNES);
  case Mode::GB:   return dspaudio.setFrequency(config->audio.frequencyGB);
  }
}

bool Interface::cartridgeLoaded() {
  switch(mode()) {
  case Mode::NES:  return nes.cartridgeLoaded();
  case Mode::SNES: return snes.cartridgeLoaded();
  case Mode::GB:   return gb.cartridgeLoaded();
  }
  return false;
}

void Interface::loadCartridge(Mode mode) {
  utility->setMode(this->mode = mode);
  switch(mode) {
  case Mode::NES:  core = &nes; break;
  case Mode::SNES: core = &snes; break;
  case Mode::GB:   core = &gb; break;
  default:         core = nullptr; break;
  }

  bindControllers();
  cheatEditor->load(game.filename("cheats.xml", ".cht"));
  stateManager->load(game.filename("states.bsa", ".bsa"), 0u);
  dipSwitches->load();
  utility->showMessage({ "Loaded ", cartridgeTitle });
}

bool Interface::loadCartridge(string filename) {
  filename.trim<1>("\"");
  filename.transform("\\", "/");
  bool result = false;
  if(filename.endswith(".nes") || filename.endswith(".nes/")) result = nes.loadCartridge(filename);
  if(filename.endswith(".sfc") || filename.endswith(".sfc/")) result = snes.loadCartridge(filename);
  if(filename.endswith(".gb" ) || filename.endswith(".gb/" )) result = gb.loadCartridge(GB::System::Revision::GameBoy, filename);
  if(filename.endswith(".gbc") || filename.endswith(".gbc/")) result = gb.loadCartridge(GB::System::Revision::GameBoyColor, filename);
  return result;
}

void Interface::unloadCartridge() {
  if(cartridgeLoaded() == false) return;
  cheatDatabase->setVisible(false);
  cheatEditor->save(game.filename("cheats.xml", ".cht"));
  stateManager->save(game.filename("states.bsa", ".bsa"), 0u);
  setCheatCodes();

  switch(mode()) {
  case Mode::NES:  nes.unloadCartridge(); break;
  case Mode::SNES: snes.unloadCartridge(); break;
  case Mode::GB:   gb.unloadCartridge(); break;
  }

  cartridgeTitle = "";
  utility->setMode(mode = Mode::None);
}

void Interface::power() {
  if(core == nullptr) return;
  core->power();
  utility->showMessage("System power was cycled");
}

void Interface::reset() {
  if(core == nullptr) return;
  core->reset();
  utility->showMessage("System was reset");
}

void Interface::run() {
  if(core == nullptr) return;
  core->run();
}

serializer Interface::serialize() {
  if(core == nullptr) return serializer();
  return core->serialize();
}

bool Interface::unserialize(serializer &s) {
  if(core == nullptr) return false;
  return core->unserialize(s);
}

bool Interface::saveState(unsigned slot) {
  string filename = game.filename({ "state-", slot, ".bst" }, { "-", slot, ".bst" });
  serializer s = serialize();
  bool result = file::write(filename, s.data(), s.size());
  utility->showMessage(result == true ? string{ "Saved state ", slot } : "Failed to save state");
  return result;
}

bool Interface::loadState(unsigned slot) {
  string filename = game.filename({ "state-", slot, ".bst" }, { "-", slot, ".bst" });
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) {
    utility->showMessage(string{ "Slot ", slot, " save file not found" });
    return false;
  }
  serializer s(data, size);
  bool result = unserialize(s);
  utility->showMessage(result == true ? string{ "Loaded state ", slot } : "Failed to load state");
  return result;
}

void Interface::setCheatCodes(const lstring &list) {
  switch(mode()) {
  case Mode::NES:  return nes.setCheats(list);
  case Mode::SNES: return snes.setCheats(list);
  case Mode::GB:   return gb.setCheats(list);
  }
}

string Interface::sha256() {
  switch(mode()) {
  case Mode::NES:  return NES::cartridge.sha256();
  case Mode::SNES: return SNES::cartridge.sha256();
  case Mode::GB:   return GB::cartridge.sha256();
  }
  return "{None}";
}

Interface::Interface() : core(nullptr) {
  mode = Mode::None;
  nes.initialize();
  snes.initialize();
  gb.initialize();
}

//internal

bool Interface::applyPatch(CartridgePath &filepath, uint8_t *&data, unsigned &size) {
  string patchname = filepath.filename("patch.bps", ".bps");
  if(file::exists(patchname) == false) return false;

  bpspatch bps;
  bps.modify(patchname);
  bps.source(data, size);
  unsigned targetSize = bps.size();
  uint8_t *targetData = new uint8_t[targetSize];
  bps.target(targetData, targetSize);
  if(bps.apply() != bpspatch::result::success) {
    delete[] targetData;
    return false;
  }

  delete[] data;
  data = targetData;
  size = targetSize;
  return true;
}

void Interface::videoRefresh(const uint32_t *input, unsigned inputPitch, unsigned width, unsigned height) {
  uint32_t *output;
  unsigned outputPitch;

  if(filter.opened()) {
    filter.render(input, inputPitch, width, height);
    input = filter.data;
    inputPitch = filter.pitch;
    width = filter.width;
    height = filter.height;
  }

  if(video.lock(output, outputPitch, width, height)) {
    inputPitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      const uint32_t *sp = input + y * inputPitch;
      uint32_t *dp = output + y * outputPitch;
      for(unsigned x = 0; x < width; x++) {
        uint32_t color = *sp++;
        *dp++ = palette((color >> 20) & 1023, (color >> 10) & 1023, (color >> 0) & 1023);
      }
    }

    video.unlock();
    video.refresh();
  }

  static unsigned frameCounter = 0;
  static time_t previous, current;
  frameCounter++;

  time(&current);
  if(current != previous) {
    previous = current;
    utility->setStatusText({ "FPS: ", frameCounter });
    frameCounter = 0;
  }
}
