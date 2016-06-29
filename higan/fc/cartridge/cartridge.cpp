#include <fc/fc.hpp>

namespace Famicom {

#include "chip/chip.cpp"
#include "board/board.cpp"
Cartridge cartridge;

auto Cartridge::Enter() -> void {
  while(true) scheduler.synchronize(), cartridge.main();
}

auto Cartridge::main() -> void {
  board->main();
}

auto Cartridge::load() -> bool {
  if(auto pathID = interface->load(ID::Famicom, "Famicom", "fc")) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else {
    return false;
  }

  Board::load(information.manifest);  //this call will set Cartridge::board if successful
  if(!board) return false;

  Hash::SHA256 sha;
  sha.data(board->prgrom.data, board->prgrom.size);
  sha.data(board->chrrom.data, board->chrrom.size);
  information.sha256 = sha.digest();
  return true;
}

auto Cartridge::save() -> void {
  board->save();
}

auto Cartridge::unload() -> void {
  delete board;
  board = nullptr;
}

auto Cartridge::power() -> void {
  board->power();
}

auto Cartridge::reset() -> void {
  create(Cartridge::Enter, 21'477'272);
  board->reset();
}

auto Cartridge::readPRG(uint addr) -> uint8 {
  return board->readPRG(addr);
}

auto Cartridge::writePRG(uint addr, uint8 data) -> void {
  return board->writePRG(addr, data);
}

auto Cartridge::readCHR(uint addr) -> uint8 {
  return board->readCHR(addr);
}

auto Cartridge::writeCHR(uint addr, uint8 data) -> void {
  return board->writeCHR(addr, data);
}

auto Cartridge::scanline(uint y) -> void {
  return board->scanline(y);
}

auto Cartridge::serialize(serializer& s) -> void {
  Thread::serialize(s);
  return board->serialize(s);
}

}
