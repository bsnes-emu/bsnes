#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
//#include <nds/interface/interface.hpp>

void Program::bootstrap() {
  interface = new Interface;

  emulator.append(new Famicom::Interface);
  emulator.append(new SuperFamicom::Interface);
  emulator.append(new GameBoy::Interface);
  emulator.append(new GameBoyAdvance::Interface);
//emulator.append(new NintendoDS::Interface);

  for(auto& system : emulator) system->bind = interface;
}
