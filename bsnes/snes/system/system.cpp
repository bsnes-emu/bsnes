#include <snes.hpp>

#define SYSTEM_CPP
namespace SNES {

System system;

#include <config/config.cpp>
#include <debugger/debugger.cpp>
#include <scheduler/scheduler.cpp>

#include <video/video.cpp>
#include <audio/audio.cpp>
#include <input/input.cpp>

#include "serialization.cpp"

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
    input.update();
    video.update();
  }
}

void System::runtosave() {
  if(CPU::Threaded == true) {
    scheduler.sync = Scheduler::SynchronizeMode::CPU;
    runthreadtosave();
  }

  if(SMP::Threaded == true) {
    scheduler.thread = smp.thread;
    runthreadtosave();
  }

  if(PPU::Threaded == true) {
    scheduler.thread = ppu.thread;
    runthreadtosave();
  }

  if(DSP::Threaded == true) {
    scheduler.thread = dsp.thread;
    runthreadtosave();
  }

  for(unsigned i = 0; i < cpu.coprocessors.size(); i++) {
    Processor &chip = *cpu.coprocessors[i];
    scheduler.thread = chip.thread;
    runthreadtosave();
  }
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason() == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason() == Scheduler::ExitReason::FrameEvent) {
      input.update();
      video.update();
    }
  }
}

void System::init(Interface *interface_) {
  interface = interface_;
  assert(interface != 0);

  supergameboy.init();
  superfx.init();
  sa1.init();
  upd77c25.init();
  bsxbase.init();
  bsxcart.init();
  bsxflash.init();
  srtc.init();
  sdd1.init();
  spc7110.init();
  cx4.init();
  obc1.init();
  st0010.init();
  st0011.init();
  st0018.init();
  msu1.init();
  serial.init();

  video.init();
  audio.init();
  input.init();

  input.port_set_device(0, config.controller_port1);
  input.port_set_device(1, config.controller_port2);
}

void System::term() {
}

void System::power() {
  region = config.region;
  expansion = config.expansion_port;
  if(region == Region::Autodetect) {
    region = (cartridge.region() == Cartridge::Region::NTSC ? Region::NTSC : Region::PAL);
  }

  cpu_frequency = region() == Region::NTSC ? config.cpu.ntsc_frequency : config.cpu.pal_frequency;
  apu_frequency = region() == Region::NTSC ? config.smp.ntsc_frequency : config.smp.pal_frequency;

  bus.power();
  for(unsigned i = 0x2100; i <= 0x213f; i++) memory::mmio.map(i, ppu);
  for(unsigned i = 0x2140; i <= 0x217f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x2180; i <= 0x2183; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4016; i <= 0x4017; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4200; i <= 0x421f; i++) memory::mmio.map(i, cpu);
  for(unsigned i = 0x4300; i <= 0x437f; i++) memory::mmio.map(i, cpu);

  audio.coprocessor_enable(false);
  if(expansion() == ExpansionPortDevice::BSX) bsxbase.enable();
  if(memory::bsxflash.data()) bsxflash.enable();
  if(cartridge.mode() == Cartridge::Mode::Bsx) bsxcart.enable();
  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) supergameboy.enable();

  if(cartridge.has_superfx()) superfx.enable();
  if(cartridge.has_sa1()) sa1.enable();
  if(cartridge.has_upd77c25()) upd77c25.enable();
  if(cartridge.has_srtc()) srtc.enable();
  if(cartridge.has_sdd1()) sdd1.enable();
  if(cartridge.has_spc7110()) spc7110.enable();
  if(cartridge.has_cx4()) cx4.enable();
  if(cartridge.has_obc1()) obc1.enable();
  if(cartridge.has_st0010()) st0010.enable();
  if(cartridge.has_st0011()) st0011.enable();
  if(cartridge.has_st0018()) st0018.enable();
  if(cartridge.has_msu1()) msu1.enable();
  if(cartridge.has_serial()) serial.enable();

  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();

  if(expansion() == ExpansionPortDevice::BSX) bsxbase.power();
  if(memory::bsxflash.data()) bsxflash.power();
  if(cartridge.mode() == Cartridge::Mode::Bsx) bsxcart.power();
  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) supergameboy.power();

  if(cartridge.has_superfx()) superfx.power();
  if(cartridge.has_sa1()) sa1.power();
  if(cartridge.has_upd77c25()) upd77c25.power();
  if(cartridge.has_srtc()) srtc.power();
  if(cartridge.has_sdd1()) sdd1.power();
  if(cartridge.has_spc7110()) spc7110.power();
  if(cartridge.has_cx4()) cx4.power();
  if(cartridge.has_obc1()) obc1.power();
  if(cartridge.has_st0010()) st0010.power();
  if(cartridge.has_st0011()) st0011.power();
  if(cartridge.has_st0018()) st0018.power();
  if(cartridge.has_msu1()) msu1.power();
  if(cartridge.has_serial()) serial.power();

  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) cpu.coprocessors.append(&supergameboy);
  if(cartridge.has_superfx()) cpu.coprocessors.append(&superfx);
  if(cartridge.has_sa1()) cpu.coprocessors.append(&sa1);
  if(cartridge.has_upd77c25()) cpu.coprocessors.append(&upd77c25);
  if(cartridge.has_msu1()) cpu.coprocessors.append(&msu1);
  if(cartridge.has_serial()) cpu.coprocessors.append(&serial);

  scheduler.init();

  input.update();
//video.update();
}

void System::reset() {
  bus.reset();
  cpu.reset();
  smp.reset();
  dsp.reset();
  ppu.reset();

  if(expansion() == ExpansionPortDevice::BSX) bsxbase.reset();
  if(memory::bsxflash.data()) bsxflash.reset();
  if(cartridge.mode() == Cartridge::Mode::Bsx) bsxcart.reset();
  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) supergameboy.reset();

  if(cartridge.has_superfx()) superfx.reset();
  if(cartridge.has_sa1()) sa1.reset();
  if(cartridge.has_upd77c25()) upd77c25.reset();
  if(cartridge.has_srtc()) srtc.reset();
  if(cartridge.has_sdd1()) sdd1.reset();
  if(cartridge.has_spc7110()) spc7110.reset();
  if(cartridge.has_cx4()) cx4.reset();
  if(cartridge.has_obc1()) obc1.reset();
  if(cartridge.has_st0010()) st0010.reset();
  if(cartridge.has_st0011()) st0011.reset();
  if(cartridge.has_st0018()) st0018.reset();
  if(cartridge.has_msu1()) msu1.reset();
  if(cartridge.has_serial()) serial.reset();

  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) cpu.coprocessors.append(&supergameboy);
  if(cartridge.has_superfx()) cpu.coprocessors.append(&superfx);
  if(cartridge.has_sa1()) cpu.coprocessors.append(&sa1);
  if(cartridge.has_upd77c25()) cpu.coprocessors.append(&upd77c25);
  if(cartridge.has_msu1()) cpu.coprocessors.append(&msu1);
  if(cartridge.has_serial()) cpu.coprocessors.append(&serial);

  scheduler.init();

  input.port_set_device(0, config.controller_port1);
  input.port_set_device(1, config.controller_port2);
  input.update();
//video.update();
}

void System::unload() {
  if(cartridge.mode() == Cartridge::Mode::SuperGameBoy) supergameboy.unload();
}

void System::scanline() {
  video.scanline();
  if(cpu.vcounter() == 241) scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

void System::frame() {
}

System::System() : interface(0) {
  region = Region::Autodetect;
  expansion = ExpansionPortDevice::None;
}

}
