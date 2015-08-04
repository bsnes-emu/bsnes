#include <fc/fc.hpp>

namespace Famicom {

#include "chip/chip.cpp"
#include "board/board.cpp"
Cartridge cartridge;

string Cartridge::title() {
  return information.title;
}

void Cartridge::Main() {
  cartridge.main();
}

void Cartridge::main() {
  board->main();
}

void Cartridge::load() {
  interface->loadRequest(ID::Manifest, "manifest.bml", true);

  Board::load(information.markup);  //this call will set Cartridge::board if successful
  if(board == nullptr) return;

  Hash::SHA256 sha;
  sha.data(board->prgrom.data, board->prgrom.size);
  sha.data(board->chrrom.data, board->chrrom.size);
  sha256 = sha.digest();

  system.load();
  loaded = true;
}

void Cartridge::unload() {
  if(loaded == false) return;
  loaded = false;
  memory.reset();
}

void Cartridge::power() {
  board->power();
}

void Cartridge::reset() {
  create(Cartridge::Main, 21477272);
  board->reset();
}

Cartridge::Cartridge() {
  loaded = false;
}

uint8 Cartridge::prg_read(unsigned addr) {
  return board->prg_read(addr);
}

void Cartridge::prg_write(unsigned addr, uint8 data) {
  return board->prg_write(addr, data);
}

uint8 Cartridge::chr_read(unsigned addr) {
  return board->chr_read(addr);
}

void Cartridge::chr_write(unsigned addr, uint8 data) {
  return board->chr_write(addr, data);
}

void Cartridge::scanline(unsigned y) {
  return board->scanline(y);
}

void Cartridge::serialize(serializer& s) {
  Thread::serialize(s);
  return board->serialize(s);
}

}
