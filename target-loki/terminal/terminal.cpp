#include "../loki.hpp"
Terminal* terminal = nullptr;

Terminal::Terminal() {
  terminal = this;

  setTitle({"loki v", Emulator::Version});
  setWindowGeometry({0, 480 + frameMargin().height, 800, 480});

  console.setFont(Font::monospace(8));
  console.setPrompt("$ ");

  layout.append(console, {~0, ~0});
  append(layout);

  onClose = &Application::quit;
  console.onActivate = {&Terminal::command, this};
}

void Terminal::command(string t) {
  auto source = Debugger::Source::CPU;
  if(t.beginsWith("cpu/"  )) { source = Debugger::Source::CPU;   t.ltrim<1>("cpu/"  ); }
  if(t.beginsWith("smp/"  )) { source = Debugger::Source::SMP;   t.ltrim<1>("smp/"  ); }
  if(t.beginsWith("ppu/"  )) { source = Debugger::Source::PPU;   t.ltrim<1>("ppu/"  ); }
  if(t.beginsWith("dsp/"  )) { source = Debugger::Source::DSP;   t.ltrim<1>("dsp/"  ); }
  if(t.beginsWith("apu/"  )) { source = Debugger::Source::APU;   t.ltrim<1>("apu/"  ); }
  if(t.beginsWith("vram/" )) { source = Debugger::Source::VRAM;  t.ltrim<1>("vram/" ); }
  if(t.beginsWith("oam/"  )) { source = Debugger::Source::OAM;   t.ltrim<1>("oam/"  ); }
  if(t.beginsWith("cgram/")) { source = Debugger::Source::CGRAM; t.ltrim<1>("cgram/"); }

  if(source == Debugger::Source::CPU) t.replace("$", hex(SFC::cpu.regs.pc));
  if(source == Debugger::Source::SMP) t.replace("$", hex(SFC::smp.regs.pc));

  lstring args = t.strip().qsplit(" ").strip();
  string s = args.takeFirst();
  unsigned argc = args.size();

  if(s.empty()) return;

  if(s.beginsWith("settings.")) return settings->command(s, args);

  if(s == "break") {
    debugger->stop();
    return;
  }

  if(s == "breakpoints") {
    debugger->echoBreakpoints();
    return;
  }

  if(s == "breakpoints.append" && argc >= 2 && argc <= 3) {
    Debugger::Breakpoint bp;
    bp.source = source;
    if(args[0] == "read"   ) bp.mode = Debugger::Breakpoint::Mode::Read;
    if(args[0] == "write"  ) bp.mode = Debugger::Breakpoint::Mode::Write;
    if(args[0] == "execute") bp.mode = Debugger::Breakpoint::Mode::Execute;
    bp.addr = hex(args[1]);
    if(argc >= 3) bp.data = {true, (uint8_t)hex(args[2])};
    debugger->breakpoints.append(bp);
    debugger->echoBreakpoints();
    return;
  }

  if(s == "breakpoints.remove" && argc == 1) {
    unsigned n = decimal(args[0]);
    if(n < debugger->breakpoints.size()) {
      debugger->breakpoints.remove(n);
    }
    debugger->echoBreakpoints();
    return;
  }

  if(s == "breakpoints.reset") {
    debugger->breakpoints.reset();
    return;
  }

  if(s == "clear") {
    reset();
    return;
  }

  if(s == "disassemble" && argc >= 1 && argc <= 2) {
    debugger->echoDisassemble(hex(args[0]), argc == 2 ? decimal(args[1]) : 16);
    return;
  }

  if(s == "export" && argc <= 1) {
    string filename = {debugger->sourceName(source), "-", string::datetime().transform(" :", "--"), ".bin"};
    if(argc >= 1) filename = args[0];
    string pathname = {interface->pathname, "loki/", filename};
    debugger->memoryExport(source, pathname);
    return;
  }

  if(s == "hex" && argc >= 1 && argc <= 2) {
    debugger->echoHex(source, hex(args[0]), argc == 2 ? decimal(args[1]) : 256);
    return;
  }

  if(s == "quit") {
    Application::quit();
    return;
  }

  if(s == "read" && argc == 1) {
    unsigned addr = hex(args[0]);
    uint8_t data = debugger->memoryRead(source, addr);
    echo(debugger->sourceName(source), "/", hex<6>(addr), " = ", hex<2>(data), "\n");
    return;
  }

  if(s == "run" && argc == 0) {
    debugger->run();
    return;
  }

  if(s == "run.for" && argc == 1) {
    debugger->run();
    debugger->cpuRunFor = {true, (unsigned)decimal(args[0])};
    return;
  }

  if(s == "run.to" && argc == 1) {
    debugger->run();
    debugger->cpuRunTo = {true, (unsigned)hex(args[0])};
    return;
  }

  if(s == "step" && argc == 0) {
    debugger->run();
    debugger->cpuStepFor = {true, 1u};
    return;
  }

  if(s == "step.for" && argc == 1) {
    debugger->run();
    debugger->cpuStepFor = {true, (unsigned)decimal(args[0])};
    return;
  }

  if(s == "step.to" && argc == 1) {
    debugger->run();
    debugger->cpuStepTo = {true, (unsigned)hex(args[0])};
    return;
  }

  if(s == "tracer.enable" && argc <= 1) {
    if(debugger->tracerFile.open() == false) {
      string filename = {"trace-", string::datetime().transform(" :", "--"), ".log"};
      if(argc >= 1) filename = args[0];
      string pathname = {interface->pathname, "loki/", filename};
      if(debugger->tracerFile.open(pathname, file::mode::write)) {
        echo("Tracer enabled\n");
      }
    }
    return;
  }

  if(s == "tracer.disable") {
    if(debugger->tracerFile.open() == true) {
      debugger->tracerFile.close();
      echo("Tracer disabled\n");
    }
    return;
  }

  if(s == "tracer.mask" && argc == 1) {
    if(args[0] != "false") {
      debugger->tracerMask.resize(0x1000000);
      debugger->tracerMask.clear();
      echo("Tracer mask enabled\n");
    } else {
      debugger->tracerMask.reset();
      echo("Tracer mask disabled\n");
    }
    return;
  }

  if(s == "usage.reset") {
    memset(debugger->usageCPU, 0, 0x1000000);
    return;
  }

  if(s == "write" && argc == 2) {
    unsigned addr = hex(args[0]);
    uint8 data = hex(args[1]);
    debugger->memoryWrite(source, addr, data);
    echo(debugger->sourceName(source), "/", hex<6>(addr), " = ", hex<2>(data), "\n");
    return;
  }

  echo("Error: unrecognized command: ", s, "\n");
}

void Terminal::reset() {
  console.reset();
}

void Terminal::print(const string& text) {
  console.print(text);
}
