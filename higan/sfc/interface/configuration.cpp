Configuration configuration;

auto Configuration::read() -> string {
  return {
    "system\n"
    "  cpu version=", system.cpu.version, "\n"
    "  ppu1 version=", system.ppu1.version, "\n"
    "    vram size=0x", hex(system.ppu1.vram.size), "\n"
    "  ppu2 version=", system.ppu2.version, "\n"
    "\n"
    "video\n"
    "  blurEmulation: ", video.blurEmulation, "\n"
    "  colorEmulation: ", video.colorEmulation, "\n"
    "\n"
    "hacks\n"
    "  ppuFast\n"
    "    enable: ", hacks.ppuFast.enable, "\n"
    "    noSpriteLimit: ", hacks.ppuFast.noSpriteLimit, "\n"
    "    hiresMode7: ", hacks.ppuFast.hiresMode7, "\n"
    "  dspFast\n"
    "    enable: ", hacks.dspFast.enable, "\n"
  };
}

auto Configuration::read(string name) -> string {
  #define bind(id) { \
    string key = {string{#id}.transform(".", "/")}; \
    if(name == key) return name; \
  }
  bind(system.cpu.version);
  bind(system.ppu1.version);
  bind(system.ppu1.vram.size);
  bind(system.ppu2.version);

  bind(video.blurEmulation);
  bind(video.colorEmulation);

  bind(hacks.ppuFast.enable);
  bind(hacks.ppuFast.noSpriteLimit);
  bind(hacks.ppuFast.hiresMode7);
  bind(hacks.dspFast.enable);
  #undef bind
  return {};
}

auto Configuration::write(string configuration) -> bool {
  *this = {};

  auto document = BML::unserialize(configuration);
  if(!document) return false;

  #define bind(type, id) { \
    string key = {string{#id}.transform(".", "/")}; \
    if(auto node = document[key]) id = node.type(); \
  }
  bind(natural, system.cpu.version);
  bind(natural, system.ppu1.version);
  bind(natural, system.ppu1.vram.size);
  bind(natural, system.ppu2.version);

  bind(boolean, video.blurEmulation);
  bind(boolean, video.colorEmulation);

  bind(boolean, hacks.ppuFast.enable);
  bind(boolean, hacks.ppuFast.noSpriteLimit);
  bind(boolean, hacks.ppuFast.hiresMode7);
  bind(boolean, hacks.dspFast.enable);
  #undef bind
  return true;
}

auto Configuration::write(string name, string value) -> bool {
  #define bind(type, id) { \
    string key = {string{#id}.transform(".", "/")}; \
    if(name == key) return id = Markup::Node().setValue(value).type(), true; \
  }
  bind(boolean, video.blurEmulation);
  bind(boolean, video.colorEmulation);

  bind(boolean, hacks.ppuFast.enable);
  bind(boolean, hacks.ppuFast.noSpriteLimit);
  bind(boolean, hacks.ppuFast.hiresMode7);
  bind(boolean, hacks.dspFast.enable);
  if(SuperFamicom::system.loaded()) return false;

  bind(natural, system.cpu.version);
  bind(natural, system.ppu1.version);
  bind(natural, system.ppu1.vram.size);
  bind(natural, system.ppu2.version);
  #undef bind
  return false;
}
