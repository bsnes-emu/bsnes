#include <fc/fc.hpp>

namespace Famicom {

#include "chip/chip.cpp"
#include "board/board.cpp"
Cartridge cartridge;

auto Cartridge::loaded() const -> bool {
  return _loaded;
}

auto Cartridge::sha256() const -> string {
  return _sha256;
}

auto Cartridge::manifest() const -> string {
  return information.markup;
}

auto Cartridge::title() const -> string {
  return information.title;
}

auto Cartridge::Main() -> void {
  cartridge.main();
}

auto Cartridge::main() -> void {
  board->main();
}

auto Cartridge::load() -> void {
  interface->loadRequest(ID::Manifest, "manifest.bml", true);

  Board::load(information.markup);  //this call will set Cartridge::board if successful
  if(!board) return;

  Hash::SHA256 sha;
  sha.data(board->prgrom.data, board->prgrom.size);
  sha.data(board->chrrom.data, board->chrrom.size);
  _sha256 = sha.digest();

  system.load();
  _loaded = true;
}

auto Cartridge::unload() -> void {
  if(!loaded()) return;
  _loaded = false;
  memory.reset();
}

auto Cartridge::power() -> void {
  board->power();
}

auto Cartridge::reset() -> void {
  create(Cartridge::Main, 21477272);
  board->reset();
}

auto Cartridge::prg_read(uint addr) -> uint8 {
  return board->prg_read(addr);
}

auto Cartridge::prg_write(uint addr, uint8 data) -> void {
  return board->prg_write(addr, data);
}

auto Cartridge::chr_read(uint addr) -> uint8 {
  return board->chr_read(addr);
}

auto Cartridge::chr_write(uint addr, uint8 data) -> void {
  return board->chr_write(addr, data);
}

auto Cartridge::scanline(uint y) -> void {
  return board->scanline(y);
}

auto Cartridge::serialize(serializer& s) -> void {
  Thread::serialize(s);
  return board->serialize(s);
}

}
