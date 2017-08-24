#include <sfc/sfc.hpp>

namespace SuperFamicom {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "video.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(smp);
  scheduler.synchronize(ppu);
  scheduler.synchronize(dsp);
  for(auto coprocessor : cpu.coprocessors) scheduler.synchronize(*coprocessor);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::init() -> void {
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

  bsmemory.init();
}

auto System::term() -> void {
}

auto System::load(Emulator::Interface* interface) -> bool {
  information = {};

  if(auto fp = platform->open(ID::System, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  auto system = document["system"];

  bus.reset();
  if(!cpu.load(system)) return false;
  if(!smp.load(system)) return false;
  if(!ppu.load(system)) return false;
  if(!dsp.load(system)) return false;
  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.cpuFrequency = Emulator::Constants::Colorburst::NTSC * 6.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.cpuFrequency = Emulator::Constants::Colorburst::PAL * 4.8;
  }

  if(cartridge.has.ICD2) icd2.load();
  if(cartridge.has.MCC) mcc.load();
  if(cartridge.has.NSSDIP) nss.load();
  if(cartridge.has.Event) event.load();
  if(cartridge.has.SA1) sa1.load();
  if(cartridge.has.SuperFX) superfx.load();
  if(cartridge.has.ARMDSP) armdsp.load();
  if(cartridge.has.HitachiDSP) hitachidsp.load();
  if(cartridge.has.NECDSP) necdsp.load();
  if(cartridge.has.EpsonRTC) epsonrtc.load();
  if(cartridge.has.SharpRTC) sharprtc.load();
  if(cartridge.has.SPC7110) spc7110.load();
  if(cartridge.has.SDD1) sdd1.load();
  if(cartridge.has.OBC1) obc1.load();
  if(cartridge.has.MSU1) msu1.load();

  if(cartridge.has.BSMemorySlot) bsmemory.load();
  if(cartridge.has.SufamiTurboSlots) sufamiturboA.load(), sufamiturboB.load();

  serializeInit();
  this->interface = interface;
  return information.loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;

  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;

  cpu.peripherals.reset();
  controllerPort1.unload();
  controllerPort2.unload();
  expansionPort.unload();

  if(cartridge.has.ICD2) icd2.unload();
  if(cartridge.has.MCC) mcc.unload();
  if(cartridge.has.NSSDIP) nss.unload();
  if(cartridge.has.Event) event.unload();
  if(cartridge.has.SA1) sa1.unload();
  if(cartridge.has.SuperFX) superfx.unload();
  if(cartridge.has.ARMDSP) armdsp.unload();
  if(cartridge.has.HitachiDSP) hitachidsp.unload();
  if(cartridge.has.NECDSP) necdsp.unload();
  if(cartridge.has.EpsonRTC) epsonrtc.unload();
  if(cartridge.has.SharpRTC) sharprtc.unload();
  if(cartridge.has.SPC7110) spc7110.unload();
  if(cartridge.has.SDD1) sdd1.unload();
  if(cartridge.has.OBC1) obc1.unload();
  if(cartridge.has.MSU1) msu1.unload();

  if(cartridge.has.BSMemorySlot) bsmemory.unload();
  if(cartridge.has.SufamiTurboSlots) sufamiturboA.unload(), sufamiturboB.unload();

  cartridge.unload();
  information.loaded = false;
}

auto System::power() -> void {
  Emulator::video.reset();
  Emulator::video.setInterface(interface);
  configureVideoPalette();
  configureVideoEffects();

  Emulator::audio.reset();
  Emulator::audio.setInterface(interface);

  random.entropy(Random::Entropy::High);

  scheduler.reset();
  cpu.power();
  smp.power();
  dsp.power();
  ppu.power();

  if(cartridge.has.ICD2) icd2.power();
  if(cartridge.has.MCC) mcc.power();
  if(cartridge.has.NSSDIP) nss.power();
  if(cartridge.has.Event) event.power();
  if(cartridge.has.SA1) sa1.power();
  if(cartridge.has.SuperFX) superfx.power();
  if(cartridge.has.ARMDSP) armdsp.power();
  if(cartridge.has.HitachiDSP) hitachidsp.power();
  if(cartridge.has.NECDSP) necdsp.power();
  if(cartridge.has.EpsonRTC) epsonrtc.power();
  if(cartridge.has.SharpRTC) sharprtc.power();
  if(cartridge.has.SPC7110) spc7110.power();
  if(cartridge.has.SDD1) sdd1.power();
  if(cartridge.has.OBC1) obc1.power();
  if(cartridge.has.MSU1) msu1.power();

  if(cartridge.has.BSMemorySlot) bsmemory.power();

  if(cartridge.has.ICD2) cpu.coprocessors.append(&icd2);
  if(cartridge.has.Event) cpu.coprocessors.append(&event);
  if(cartridge.has.SA1) cpu.coprocessors.append(&sa1);
  if(cartridge.has.SuperFX) cpu.coprocessors.append(&superfx);
  if(cartridge.has.ARMDSP) cpu.coprocessors.append(&armdsp);
  if(cartridge.has.HitachiDSP) cpu.coprocessors.append(&hitachidsp);
  if(cartridge.has.NECDSP) cpu.coprocessors.append(&necdsp);
  if(cartridge.has.EpsonRTC) cpu.coprocessors.append(&epsonrtc);
  if(cartridge.has.SharpRTC) cpu.coprocessors.append(&sharprtc);
  if(cartridge.has.SPC7110) cpu.coprocessors.append(&spc7110);
  if(cartridge.has.MSU1) cpu.coprocessors.append(&msu1);

  scheduler.primary(cpu);

  controllerPort1.power(ID::Port::Controller1);
  controllerPort2.power(ID::Port::Controller2);
  expansionPort.power();

  controllerPort1.connect(settings.controllerPort1);
  controllerPort2.connect(settings.controllerPort2);
  expansionPort.connect(settings.expansionPort);
}

}
