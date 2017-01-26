//PC Engine HuCards lack save RAM on them due to the card size and cost savings.
//The PC Engine CD adds 2KB of backup RAM that most HuCard games can use for saves.
//However, all games must share this small amount of RAM.
//Since this is an emulator, we can make this process nicer by storing BRAM per-game.

//This does hard-code the save.ram name, rather than using a manifest file name.
//It also creates a save.ram file no matter what, even for games that don't save data.
//Unfortunately, we can't know in advance if a game supports BRAM saves or not.
//So because of this, we have to always create it.
//Thankfully, the file is very small so it should not prove to be a burden in practice.

auto CPU::load() -> void {
  for(auto& byte : bram) byte = 0xff;
  if(auto fp = platform->open(cartridge.pathID(), "save.ram", File::Read)) {
    fp->read(bram, 0x800);
  }
}

auto CPU::save() -> void {
  if(auto fp = platform->open(cartridge.pathID(), "save.ram", File::Write)) {
    fp->write(bram, 0x800);
  }
}
