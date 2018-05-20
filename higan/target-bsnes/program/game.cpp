auto Program::load(string location) -> void {
  if(!file::exists(location)) return;
  unload();

  //Heuristics::SuperFamicom() call will remove copier header from rom if present
  auto rom = file::read(location);
  auto heuristics = Heuristics::SuperFamicom(rom, location);
  context.game.manifest = heuristics.manifest();
  context.game.document = BML::unserialize(context.game.manifest);
  context.game.location = location;

  uint offset = 0;
  if(auto size = heuristics.programRomSize()) {
    context.game.program.resize(size);
    memory::copy(&context.game.program[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.dataRomSize()) {
    context.game.data.resize(size);
    memory::copy(&context.game.data[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.expansionRomSize()) {
    context.game.expansion.resize(size);
    memory::copy(&context.game.expansion[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.firmwareRomSize()) {
    context.game.firmware.resize(size);
    memory::copy(&context.game.firmware[0], &rom[offset], size);
    offset += size;
  }

  auto type = Location::suffix(location).trimLeft(".", 1L);
  for(auto& media : emulator->media) {
    if(media.type != type) continue;

    Emulator::audio.reset(2, audio->frequency());
    if(emulator->load(media.id)) {
      emulator->power();
      presentation->setTitle(emulator->title());
      presentation->reset.setEnabled(true);
      presentation->saveState.setEnabled(true);
      presentation->loadState.setEnabled(true);
    }

    break;
  }
}

auto Program::loadGameBoy(string location) -> void {
  if(!file::exists(location)) return;

  auto rom = file::read(location);
  auto heuristics = Heuristics::GameBoy(rom, location);
  context.gameBoy.manifest = heuristics.manifest();
  context.gameBoy.document = BML::unserialize(context.game.manifest);
  context.gameBoy.location = location;

  context.gameBoy.program = rom;
}

auto Program::save() -> void {
  if(!emulator->loaded()) return;
  emulator->save();
}

auto Program::unload() -> void {
  if(!emulator->loaded()) return;
  emulator->unload();
  context = {};
  presentation->setTitle({"bsnes v", Emulator::Version});
  presentation->reset.setEnabled(false);
  presentation->saveState.setEnabled(false);
  presentation->loadState.setEnabled(false);
  presentation->clearViewport();
}
