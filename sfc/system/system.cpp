#include <sfc/sfc.hpp>

#define SYSTEM_CPP
namespace SuperFamicom {

System system;
Configuration configuration;
Random random;

#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "serialization.cpp"

#include <sfc/scheduler/scheduler.cpp>

void System::run() {
  scheduler.sync = Scheduler::SynchronizeMode::None;

  scheduler.enter();
  if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
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
    auto& chip = *cpu.coprocessors[i];
    scheduler.thread = chip.thread;
    runthreadtosave();
  }
}

void System::runthreadtosave() {
  while(true) {
    scheduler.enter();
    if(scheduler.exit_reason == Scheduler::ExitReason::SynchronizeEvent) break;
    if(scheduler.exit_reason == Scheduler::ExitReason::FrameEvent) {
      video.update();
    }
  }
}

void System::init() {
  assert(interface != nullptr);

  satellaviewbaseunit.init();
  icd2.init();
  mcc.init();
  nss.init();
  event.init();
  sa1.init();
  superfx.init();
  armdsp.init();
  hitachidsp.init();
  necdsp.init();
  epsonrtc.init();
  sharprtc.init();
  spc7110.init();
  sdd1.init();
  obc1.init();
  msu1.init();
  satellaviewcartridge.init();

  video.init();
  audio.init();

  input.connect(0, configuration.controller_port1);
  input.connect(1, configuration.controller_port2);
}

void System::term() {
}

void System::load() {
//string manifest = string::read({interface->path(ID::System), "manifest.bml"});
  interface->loadRequest(ID::SystemManifest, "manifest.bml", true);
  auto document = BML::unserialize(information.manifest);

  if(auto iplrom = document["system/smp/rom/name"].text()) {
    interface->loadRequest(ID::IPLROM, iplrom, true);
  }

  region = configuration.region;
  expansion = configuration.expansion_port;
  if(region == Region::Autodetect) {
    region = (cartridge.region() == Cartridge::Region::NTSC ? Region::NTSC : Region::PAL);
  }

  cpu_frequency = region() == Region::NTSC ? 21477272 : 21281370;
  apu_frequency = 24607104;

  audio.coprocessor_enable(false);

  bus.reset();
  bus.map();

  cpu.enable();
  ppu.enable();

  if(expansion() == ExpansionPortDevice::Satellaview) satellaviewbaseunit.load();
  if(cartridge.hasICD2()) icd2.load();
  if(cartridge.hasMCC()) mcc.load();
  if(cartridge.hasNSSDIP()) nss.load();
  if(cartridge.hasEvent()) event.load();
  if(cartridge.hasSA1()) sa1.load();
  if(cartridge.hasSuperFX()) superfx.load();
  if(cartridge.hasARMDSP()) armdsp.load();
  if(cartridge.hasHitachiDSP()) hitachidsp.load();
  if(cartridge.hasNECDSP()) necdsp.load();
  if(cartridge.hasEpsonRTC()) epsonrtc.load();
  if(cartridge.hasSharpRTC()) sharprtc.load();
  if(cartridge.hasSPC7110()) spc7110.load();
  if(cartridge.hasSDD1()) sdd1.load();
  if(cartridge.hasOBC1()) obc1.load();
  if(cartridge.hasMSU1()) msu1.load();

  if(cartridge.hasSatellaviewSlot()) satellaviewcartridge.load();
  if(cartridge.hasSufamiTurboSlots()) sufamiturboA.load(), sufamiturboB.load();

  serialize_init();
}

void System::unload() {
  if(expansion() == ExpansionPortDevice::Satellaview) satellaviewbaseunit.unload();
  if(cartridge.hasICD2()) icd2.unload();
  if(cartridge.hasMCC()) mcc.unload();
  if(cartridge.hasNSSDIP()) nss.unload();
  if(cartridge.hasEvent()) event.unload();
  if(cartridge.hasSA1()) sa1.unload();
  if(cartridge.hasSuperFX()) superfx.unload();
  if(cartridge.hasARMDSP()) armdsp.unload();
  if(cartridge.hasHitachiDSP()) hitachidsp.unload();
  if(cartridge.hasNECDSP()) necdsp.unload();
  if(cartridge.hasEpsonRTC()) epsonrtc.unload();
  if(cartridge.hasSharpRTC()) sharprtc.unload();
  if(cartridge.hasSPC7110()) spc7110.unload();
  if(cartridge.hasSDD1()) sdd1.unload();
  if(cartridge.hasOBC1()) obc1.unload();
  if(cartridge.hasMSU1()) msu1.unload();

  if(cartridge.hasSatellaviewSlot()) satellaviewcartridge.unload();
  if(cartridge.hasSufamiTurboSlots()) sufamiturboA.unload(), sufamiturboB.unload();
}

void System::power() {
  random.seed((unsigned)time(0));

  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();

  if(expansion() == ExpansionPortDevice::Satellaview) satellaviewbaseunit.power();
  if(cartridge.hasICD2()) icd2.power();
  if(cartridge.hasMCC()) mcc.power();
  if(cartridge.hasNSSDIP()) nss.power();
  if(cartridge.hasEvent()) event.power();
  if(cartridge.hasSA1()) sa1.power();
  if(cartridge.hasSuperFX()) superfx.power();
  if(cartridge.hasARMDSP()) armdsp.power();
  if(cartridge.hasHitachiDSP()) hitachidsp.power();
  if(cartridge.hasNECDSP()) necdsp.power();
  if(cartridge.hasEpsonRTC()) epsonrtc.power();
  if(cartridge.hasSharpRTC()) sharprtc.power();
  if(cartridge.hasSPC7110()) spc7110.power();
  if(cartridge.hasSDD1()) sdd1.power();
  if(cartridge.hasOBC1()) obc1.power();
  if(cartridge.hasMSU1()) msu1.power();

  if(cartridge.hasSatellaviewSlot()) satellaviewcartridge.power();

  reset();
}

void System::reset() {
  cpu.reset();
  smp.reset();
  dsp.reset();
  ppu.reset();

  if(expansion() == ExpansionPortDevice::Satellaview) satellaviewbaseunit.reset();
  if(cartridge.hasICD2()) icd2.reset();
  if(cartridge.hasMCC()) mcc.reset();
  if(cartridge.hasNSSDIP()) nss.reset();
  if(cartridge.hasEvent()) event.reset();
  if(cartridge.hasSA1()) sa1.reset();
  if(cartridge.hasSuperFX()) superfx.reset();
  if(cartridge.hasARMDSP()) armdsp.reset();
  if(cartridge.hasHitachiDSP()) hitachidsp.reset();
  if(cartridge.hasNECDSP()) necdsp.reset();
  if(cartridge.hasEpsonRTC()) epsonrtc.reset();
  if(cartridge.hasSharpRTC()) sharprtc.reset();
  if(cartridge.hasSPC7110()) spc7110.reset();
  if(cartridge.hasSDD1()) sdd1.reset();
  if(cartridge.hasOBC1()) obc1.reset();
  if(cartridge.hasMSU1()) msu1.reset();

  if(cartridge.hasSatellaviewSlot()) satellaviewcartridge.reset();

  if(cartridge.hasICD2()) cpu.coprocessors.append(&icd2);
  if(cartridge.hasEvent()) cpu.coprocessors.append(&event);
  if(cartridge.hasSA1()) cpu.coprocessors.append(&sa1);
  if(cartridge.hasSuperFX()) cpu.coprocessors.append(&superfx);
  if(cartridge.hasARMDSP()) cpu.coprocessors.append(&armdsp);
  if(cartridge.hasHitachiDSP()) cpu.coprocessors.append(&hitachidsp);
  if(cartridge.hasNECDSP()) cpu.coprocessors.append(&necdsp);
  if(cartridge.hasEpsonRTC()) cpu.coprocessors.append(&epsonrtc);
  if(cartridge.hasSharpRTC()) cpu.coprocessors.append(&sharprtc);
  if(cartridge.hasSPC7110()) cpu.coprocessors.append(&spc7110);
  if(cartridge.hasMSU1()) cpu.coprocessors.append(&msu1);

  scheduler.init();
  input.connect(0, configuration.controller_port1);
  input.connect(1, configuration.controller_port2);
}

void System::scanline() {
  video.scanline();
  if(cpu.vcounter() == 241) scheduler.exit(Scheduler::ExitReason::FrameEvent);
}

void System::frame() {
}

System::System() {
  region = Region::Autodetect;
  expansion = ExpansionPortDevice::Satellaview;
}

}
