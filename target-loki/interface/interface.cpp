#include "../loki.hpp"
Interface* interface = nullptr;
SuperFamicom::Interface* emulator = nullptr;

Interface::Interface() {
  interface = this;
  emulator = new SuperFamicom::Interface;
  emulator->bind = this;
}

bool Interface::load(string pathname) {
  pathname.transform("\\", "/").rtrim("/");
  if(!directory::exists(pathname)) return false;

  string type = extension(pathname);

  for(auto& media : emulator->media) {
    if(media.bootable == false) continue;
    if(type != media.type) continue;

    this->pathname = pathname.append("/");
    pathnames.reset();
    pathnames(0) = program->path({media.name, ".sys/"});
    pathnames(media.id) = pathname;
    echo("Loaded ", pathname, "\n");

    emulator->load(media.id);
    emulator->paletteUpdate(Emulator::Interface::PaletteMode::Standard);
    emulator->power();
    presentation->setTitle(emulator->title());

    return true;
  }

  return false;
}

void Interface::unload() {
  emulator->unload();
}

void Interface::inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(device.isKeyboard() == false) return;

  switch(input) {
  case 84: gamepad.up = newValue; break;
  case 85: gamepad.down = newValue; break;
  case 86: gamepad.left = newValue; break;
  case 87: gamepad.right = newValue; break;
  case 60: gamepad.b = newValue; break;
  case 58: gamepad.a = newValue; break;
  case 35: gamepad.y = newValue; break;
  case 53: gamepad.x = newValue; break;
  case 38: gamepad.l = newValue; break;
  case 37: gamepad.r = newValue; break;
  case 65: gamepad.select = newValue; break;
  case 89: gamepad.start = newValue; break;
  }
}

//bindings

void Interface::loadRequest(unsigned id, string name, string type) {
}

void Interface::loadRequest(unsigned id, string path) {
  string pathname = {pathnames(emulator->group(id)), path};
  if(file::exists(pathname) == false) return;
  mmapstream stream(pathname);
  emulator->load(id, stream);
  echo("Loaded ", path, "\n");
}

void Interface::saveRequest(unsigned id, string path) {
  string pathname = {pathnames(emulator->group(id)), path};
  filestream stream(pathname, file::mode::write);
  emulator->save(id, stream);
  echo("Saved ", path, "\n");
}

uint32_t Interface::videoColor(unsigned source, uint16_t alpha, uint16_t red, uint16_t green, uint16_t blue) {
  return ((alpha >> 8) << 24) | ((red >> 8) << 16) | ((green >> 8) << 8) | ((blue >> 8) << 0);
}

void Interface::videoRefresh(const uint32_t* palette, const uint32_t* data, unsigned pitch, unsigned width, unsigned height) {
  uint32_t* output;
  unsigned outputPitch;

  if(video.lock(output, outputPitch, width, height)) {
    pitch >>= 2, outputPitch >>= 2;

    for(unsigned y = 0; y < height; y++) {
      const uint32_t* sp = data + y * pitch;
      uint32_t* dp = output + y * outputPitch;
      for(unsigned x = 0; x < width; x++) {
        *dp++ = palette[*sp++];
      }
    }

    video.unlock();
    video.refresh();
  }

  input.poll();
}

void Interface::audioSample(int16_t lsample, int16_t rsample) {
  if(settings->audio.mute) lsample = 0, rsample = 0;
  signed samples[] = {lsample, rsample};
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

int16_t Interface::inputPoll(unsigned port, unsigned device, unsigned input) {
  if(presentation->focused() == false) return 0;
  if(port != 0 || device != 0) return 0;

  switch(input) {
  case  0: return gamepad.b;
  case  1: return gamepad.y;
  case  2: return gamepad.select;
  case  3: return gamepad.start;
  case  4: return gamepad.up;
  case  5: return gamepad.down;
  case  6: return gamepad.left;
  case  7: return gamepad.right;
  case  8: return gamepad.a;
  case  9: return gamepad.x;
  case 10: return gamepad.l;
  case 11: return gamepad.r;
  }

  return 0;
}

void Interface::inputRumble(unsigned port, unsigned device, unsigned input, bool enable) {
}

unsigned Interface::dipSettings(const Markup::Node& node) {
  return 0;
}

string Interface::path(unsigned group) {
  return pathnames(group);
}

string Interface::server() {
  return "";
}

void Interface::notify(string text) {
}
