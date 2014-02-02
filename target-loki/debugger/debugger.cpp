#include "../loki.hpp"
Debugger* debugger = nullptr;

Debugger::Debugger() {
  debugger = this;
  SFC::cpu.debugger.op_exec = {&Debugger::cpuExec, this};
  SFC::cpu.debugger.op_read = {&Debugger::cpuRead, this};
  SFC::cpu.debugger.op_write = {&Debugger::cpuWrite, this};
}

void Debugger::load() {
  usageCPU = new uint8_t[0x1000000]();
  file fp;
  if(fp.open({interface->pathname, "loki/usage.cpu.bin"}, file::mode::read)) {
    if(fp.size() == 0x1000000) {
      fp.read(usageCPU, 0x1000000);
    }
    fp.close();
  }
}

void Debugger::unload() {
  if(tracerFile.open()) tracerFile.close();

  file fp;
  if(fp.open({interface->pathname, "loki/usage.cpu.bin"}, file::mode::write)) {
    fp.write(usageCPU, 0x1000000);
    fp.close();
  }
  delete[] usageCPU;
  usageCPU = nullptr;
}

void Debugger::run() {
  switch(mode) {
  case Mode::Break:
    usleep(20 * 1000);
    break;

  case Mode::Run:
    emulator->run();
    break;

  case Mode::Step:
    emulator->run();
    break;
  }
}

void Debugger::stop() {
  mode = Mode::Break;
  runFor = false;
  runTo = false;
}

string Debugger::cpuDisassemble() {
  char text[4096];
  SFC::cpu.disassemble_opcode(text);
  return text;
}

string Debugger::cpuDisassemble(unsigned addr, bool e, bool m, bool x) {
  char text[4096];
  SFC::cpu.disassemble_opcode(text, addr, e, m, x);
  return text;
}

void Debugger::cpuExec(uint24 addr) {
  usageCPU[addr] |= Usage::Execute;
  if(SFC::cpu.regs.e   == 0) usageCPU[addr] &= ~Usage::FlagE;
  if(SFC::cpu.regs.p.m == 0) usageCPU[addr] &= ~Usage::FlagM;
  if(SFC::cpu.regs.p.x == 0) usageCPU[addr] &= ~Usage::FlagX;
  if(SFC::cpu.regs.e   == 1) usageCPU[addr] |=  Usage::FlagE;
  if(SFC::cpu.regs.p.m == 1) usageCPU[addr] |=  Usage::FlagM;
  if(SFC::cpu.regs.p.x == 1) usageCPU[addr] |=  Usage::FlagX;

  if(tracerFile.open()) {
    tracerFile.print(cpuDisassemble(), "\n");
  }

  for(unsigned n = 0; n < breakpoints.size(); n++) {
    auto& bp = breakpoints[n];
    if(bp.mode != Breakpoint::Mode::Execute) continue;
    if(bp.addr != addr) continue;
    echo("Breakpoint #", n, " triggered (", ++bp.triggered, " time(s))\n");
    echo(cpuDisassemble(), "\n");
    stop();
    SFC::scheduler.debug();
  }

  if(mode == Mode::Run) {
    if(runFor) {
      if(--runFor() == 0) {
        echo(cpuDisassemble(), "\n");
        stop();
        SFC::scheduler.debug();
        return;
      }
    }

    if(runTo) {
      if(addr == runTo()) {
        echo(cpuDisassemble(), "\n");
        stop();
        SFC::scheduler.debug();
        return;
      }
    }

    return;
  }

  if(mode == Mode::Step) {
    echo(cpuDisassemble(), "\n");
    if(--stepDuration == 0) {
      stop();
      SFC::scheduler.debug();
    }
    return;
  }
}

void Debugger::cpuRead(uint24 addr, uint8 data) {
  usageCPU[addr] |= Usage::Read;

  for(unsigned n = 0; n < breakpoints.size(); n++) {
    auto& bp = breakpoints[n];
    if(bp.mode != Breakpoint::Mode::Read) continue;
    if(bp.addr != addr) continue;
    if(bp.data && bp.data() != data) continue;
    echo("Breakpoint #", n, " triggered (", ++bp.triggered, " time(s))\n");
    echo("Read ", hex<2>(data), "\n");
    stop();
    SFC::scheduler.debug();
  }
}

void Debugger::cpuWrite(uint24 addr, uint8 data) {
  usageCPU[addr] |= Usage::Write;

  for(unsigned n = 0; n < breakpoints.size(); n++) {
    auto& bp = breakpoints[n];
    if(bp.mode != Breakpoint::Mode::Write) continue;
    if(bp.addr != addr) continue;
    if(bp.data && bp.data() != data) continue;
    echo("Breakpoint #", n, " triggered (", ++bp.triggered, " time(s))\n");
    echo("Wrote ", hex<2>(data), "\n");
    stop();
    SFC::scheduler.debug();
  }
}

void Debugger::echoBreakpoints() {
  if(breakpoints.size() == 0) return;
  echo("#    type      addr    data  triggered\n");
  echo("---  --------  ------  ----  ---------\n");
  for(unsigned n = 0; n < breakpoints.size(); n++) {
    auto& bp = breakpoints[n];
    string output = {format<-3>(n), "  "};
    if(bp.mode == Breakpoint::Mode::Disabled) output.append("disabled  ");
    if(bp.mode == Breakpoint::Mode::Read    ) output.append("read      ");
    if(bp.mode == Breakpoint::Mode::Write   ) output.append("write     ");
    if(bp.mode == Breakpoint::Mode::Execute ) output.append("execute   ");
    output.append(hex<6>(bp.addr), "  ");
    output.append(bp.data ? hex<2>(bp.data()) : "  ", "    ");
    output.append(format<-9>(bp.triggered));
    echo(output, "\n");
  }
}

void Debugger::echoDisassemble(unsigned addr, signed size) {
  if(!(usageCPU[addr] & Usage::Execute)) {
    echo("No usage data available for this address\n");
    return;
  }

  while(size > 0) {
    string text = cpuDisassemble(addr, usageCPU[addr] & Usage::FlagE, usageCPU[addr] & Usage::FlagM, usageCPU[addr] & Usage::FlagX);
    echo(text, "\n");
    if(--size <= 0) break;

    unsigned displacement = 1;
    while(displacement < 5) {  //maximum opcode length is four bytes
      if(usageCPU[addr + displacement] & Usage::Execute) break;
      displacement++;
    }
    if(displacement >= 5) {
      echo("...\n");
      return;
    }
    addr += displacement;
  }
}

void Debugger::echoHex(unsigned addr, signed size) {
  while(size > 0) {
    string hexdata, asciidata;
    for(unsigned n = 0; n < 16; n++) {
      unsigned offset = addr;
      if((offset & 0x40e000) == 0x002000 || (offset & 0x40e000) == 0x004000) {
        //$00-3f,80-bf:2000-5fff
        //reading MMIO registers can negatively impact emulation, so disallow these reads
        hexdata.append("?? ");
        asciidata.append("?");
      } else {
        uint8_t byte = SFC::bus.read(addr + n);
        hexdata.append(hex<2>(byte), " ");
        asciidata.append(byte >= 0x20 && byte <= 0x7e ? (char)byte : '.');
      }
    }
    echo(hex<6>(addr), " [ ", hexdata, "] ", asciidata, "\n");
    addr += 16, size -= 16;
  }
}

uint8_t Debugger::read(unsigned addr) {
  return SFC::bus.read(addr);
}

void Debugger::write(unsigned addr, uint8_t data) {
  return SFC::bus.write(addr, data);
}
