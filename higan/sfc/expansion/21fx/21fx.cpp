#include <sfc/sfc.hpp>

namespace SuperFamicom {

S21FX s21fx;

auto S21FX::Enter() -> void {
  while(true) scheduler.synchronize(), s21fx.main();
}

auto S21FX::main() -> void {
  if(linkInit) linkInit(
    {&S21FX::quit, this},
    {&S21FX::usleep, this},
    {&S21FX::readable, this},
    {&S21FX::writable, this},
    {&S21FX::read, this},
    {&S21FX::write, this}
  );
  if(linkMain) linkMain({});
  while(true) step(10'000'000);
}

auto S21FX::init() -> void {
}

auto S21FX::load() -> void {
  resetVector.byte(0) = bus.read(0xfffc, 0x00);
  resetVector.byte(1) = bus.read(0xfffd, 0x00);

  for(auto& byte : ram) byte = 0xdb;  //stp
  ram[0] = 0x6c;  //jmp ($fffc)
  ram[1] = 0xfc;
  ram[2] = 0xff;

  bus.map({&S21FX::read, &s21fx}, {&S21FX::write, &s21fx}, 0x00, 0x00, 0xfffc, 0xfffd);
  bus.map({&S21FX::read, &s21fx}, {&S21FX::write, &s21fx}, 0x00, 0x3f, 0x2184, 0x21ff);
  bus.map({&S21FX::read, &s21fx}, {&S21FX::write, &s21fx}, 0x80, 0xbf, 0x2184, 0x21ff);

  if(auto buffer = file::read({interface->path(ID::System), "21fx.rom"})) {
    memory::copy(ram, sizeof(ram), buffer.data(), buffer.size());
  }

  string filename{interface->path(ID::SuperFamicom), "21fx.so"};
  if(link.openAbsolute(filename)) {
    linkInit = link.sym("fx_init");
    linkMain = link.sym("fx_main");
  }
}

auto S21FX::unload() -> void {
  if(link.open()) link.close();
  linkInit.reset();
  linkMain.reset();
}

auto S21FX::power() -> void {
}

auto S21FX::reset() -> void {
  create(S21FX::Enter, 10'000'000);
  booted = false;
}

auto S21FX::read(uint24 addr, uint8 data) -> uint8 {
  addr &= 0x40ffff;

  if(addr == 0xfffc) return booted ? resetVector.byte(0) : (uint8)0x84;
  if(addr == 0xfffd) return booted ? resetVector.byte(1) : (booted = true, (uint8)0x21);

  if(addr >= 0x2184 && addr <= 0x21fd) return ram[addr - 0x2184];

  if(addr == 0x21fe) return (
    (linkBuffer.size() >     0) << 7  //1 = readable
  | (snesBuffer.size() < 65536) << 6  //1 = writable
  | (link.open())               << 5  //1 = connected
  );

  if(addr == 0x21ff) {
    if(linkBuffer.size() > 0) {
      data = linkBuffer.takeFirst();
    }
  }

  return data;
}

auto S21FX::write(uint24 addr, uint8 data) -> void {
  addr &= 0x40ffff;

  if(addr == 0x21ff) {
    if(snesBuffer.size() < 65536) {
      snesBuffer.append(data);
    }
  }
}

auto S21FX::quit() -> bool {
  step(1);
  return false;
}

auto S21FX::usleep(uint microseconds) -> void {
  step(10 * microseconds);
}

auto S21FX::readable() -> bool {
  step(1);
  return snesBuffer.size() > 0;
}

auto S21FX::writable() -> bool {
  step(1);
  return linkBuffer.size() < 65536;
}

//SNES -> Link
auto S21FX::read() -> uint8 {
  step(1);
  if(snesBuffer.size() > 0) {
    return snesBuffer.takeFirst();
  }
  return 0x00;
}

//Link -> SNES
auto S21FX::write(uint8 data) -> void {
  step(1);
  if(linkBuffer.size() < 65536) {
    linkBuffer.append(data);
  }
}

}
