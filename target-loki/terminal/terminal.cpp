#include "../loki.hpp"
Terminal* terminal = nullptr;

Terminal::Terminal() {
  terminal = this;
  if(settings->geometry.terminal.empty()) {
    unsigned y = 64 + presentation->geometry().height + presentation->frameMargin().height;
    settings->geometry.terminal = {"64,", y, ",800,480"};
  }

  setTitle({"loki v", Emulator::Version});
  setGeometry(settings->geometry.terminal);

  console.setFont(Font::monospace(8));
  console.setPrompt("$ ");
  setColors();

  layout.append(console, {~0, ~0});
  append(layout);

  onClose = &Application::quit;
  console.onActivate = {&Terminal::command, this};
}

void Terminal::load() {
  if(file::exists(program->path("aliases.cfg"))) {
    string filedata = file::read(program->path("aliases.cfg"));
    lstring lines = filedata.split("\n");
    for(auto& line : lines) {
      lstring part = line.split<1>(" => ");
      if(part.size() != 2) continue;
      aliases.append({part[0], part[1]});
    }
  }

  if(file::exists(program->path("hotkeys.cfg"))) {
    string filedata = file::read(program->path("hotkeys.cfg"));
    lstring lines = filedata.split("\n");
    for(auto& line : lines) {
      lstring part = line.split<1>(" => ");
      if(part.size() != 2) continue;
      hotkeys.append({part[0], part[1]});
    }
  }
}

void Terminal::unload() {
  file fp;
  if(fp.open(program->path("aliases.cfg"), file::mode::write)) {
    for(auto& alias : aliases) fp.print(alias.name, " => ", alias.mapping, "\n");
    fp.close();
  }
  if(fp.open(program->path("hotkeys.cfg"), file::mode::write)) {
    for(auto& hotkey : hotkeys) fp.print(hotkey.name, " => ", hotkey.mapping, "\n");
    fp.close();
  }
}

void Terminal::command(string t) {
  for(auto& alias : aliases) {
    lstring tokens;
    if(tokenize(tokens, t, alias.name) == false) continue;
    string output = alias.mapping;
    for(unsigned n = 0; n < tokens.size(); n++) {
      output.replace(string{"{", 1 + n, "}"}, tokens[n]);
    }
    t = output;
    break;
  }

  auto source = Debugger::Source::CPU;
  if(t.beginsWith("cpu/"  )) { source = Debugger::Source::CPU;   t.ltrim<1>("cpu/"  ); }
  if(t.beginsWith("smp/"  )) { source = Debugger::Source::SMP;   t.ltrim<1>("smp/"  ); }
  if(t.beginsWith("ppu/"  )) { source = Debugger::Source::PPU;   t.ltrim<1>("ppu/"  ); }
  if(t.beginsWith("dsp/"  )) { source = Debugger::Source::DSP;   t.ltrim<1>("dsp/"  ); }
  if(t.beginsWith("apu/"  )) { source = Debugger::Source::APU;   t.ltrim<1>("apu/"  ); }
  if(t.beginsWith("wram/" )) { source = Debugger::Source::WRAM;  t.ltrim<1>("wram/" ); }
  if(t.beginsWith("vram/" )) { source = Debugger::Source::VRAM;  t.ltrim<1>("vram/" ); }
  if(t.beginsWith("oam/"  )) { source = Debugger::Source::OAM;   t.ltrim<1>("oam/"  ); }
  if(t.beginsWith("cgram/")) { source = Debugger::Source::CGRAM; t.ltrim<1>("cgram/"); }

  if(source == Debugger::Source::CPU) t.replace("$", hex(SFC::cpu.regs.pc));
  if(source == Debugger::Source::SMP) t.replace("$", hex(SFC::smp.regs.pc));

  lstring part = t.strip().split<1>(" "), args;
  string s = part(0);
  string p = part(1);
  if(p) args = p.qsplit(" ").strip();
  unsigned argc = args.size();

  if(s.empty()) return;

  if(s.beginsWith("settings.")) return settings->command(s, args);

  if(s == "aliases") {
    echoAliases();
    return;
  }

  if(s == "aliases.append") {
    lstring part = p.qsplit<1>("=>").strip();
    if(part.size() == 2) aliases.append({part[0], part[1]});
    echoAliases();
    return;
  }

  if(s == "aliases.remove" && argc == 1) {
    unsigned n = decimal(args[0]);
    if(n < aliases.size()) aliases.remove(n);
    echoAliases();
    return;
  }

  if(s == "aliases.reset") {
    aliases.reset();
    echo("All aliases removed\n");
    return;
  }

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
    if(argc >= 3) bp.data = (uint8_t)hex(args[2]);
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
    echo("All breakpoints removed\n");
    return;
  }

  if(s == "clear") {
    reset();
    return;
  }

  if(s == "counter") {
    if(source == Debugger::Source::CPU) echo("CPU instructions executed: ", debugger->cpuInstructionCounter, "\n");
    if(source == Debugger::Source::SMP) echo("SMP instructions executed: ", debugger->smpInstructionCounter, "\n");
    return;
  }

  if(s == "counter.reset") {
    if(source == Debugger::Source::CPU) { echo("CPU instruction counter reset\n"); debugger->cpuInstructionCounter = 0; }
    if(source == Debugger::Source::SMP) { echo("SMP instruction counter reset\n"); debugger->smpInstructionCounter = 0; }
    return;
  }

  if(s == "disassemble" && argc >= 1 && argc <= 2) {
    debugger->echoDisassemble(source, hex(args[0]), argc == 2 ? decimal(args[1]) : 16);
    return;
  }

  if(s == "export" && argc <= 1) {
    string filename = {debugger->sourceName(source), "-", string::datetime().transform(" :", "--"), ".ram"};
    if(argc >= 1) filename = args[0];
    string pathname = {interface->pathname, "loki/", filename};
    debugger->memoryExport(source, pathname);
    return;
  }

  if(s == "hex" && argc >= 1 && argc <= 2) {
    debugger->echoHex(source, hex(args[0]), argc == 2 ? decimal(args[1]) : 256);
    return;
  }

  if(s == "hotkeys") {
    echoHotkeys();
    return;
  }

  if(s == "hotkeys.append") {
    lstring part = p.qsplit<1>("=>").strip();
    if(part.size() == 2) hotkeys.append({part[0], part[1]});
    echoHotkeys();
    return;
  }

  if(s == "hotkeys.remove" && argc == 1) {
    unsigned n = decimal(args[0]);
    if(n < hotkeys.size()) hotkeys.remove(n);
    echoHotkeys();
    return;
  }

  if(s == "hotkeys.reset") {
    hotkeys.reset();
    echo("All hotkeys removed\n");
    return;
  }

  if(s == "power") {
    emulator->power();
    echo("System has been power cycled\n");
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

  if(s == "reset") {
    emulator->reset();
    echo("System has been reset\n");
    return;
  }

  if(s == "run" && argc == 0) {
    debugger->run();
    return;
  }

  if(s == "run.for" && argc == 1) {
    debugger->run();
    if(source == Debugger::Source::CPU) debugger->cpuRunFor = (unsigned)decimal(args[0]);
    if(source == Debugger::Source::SMP) debugger->smpRunFor = (unsigned)decimal(args[0]);
    return;
  }

  if(s == "run.to" && argc == 1) {
    debugger->run();
    if(source == Debugger::Source::CPU) debugger->cpuRunTo = (unsigned)hex(args[0]);
    if(source == Debugger::Source::SMP) debugger->smpRunTo = (unsigned)hex(args[0]);
    return;
  }

  if(s == "state.load" && argc == 1) {
    string pathname = {interface->pathname, "loki/state-", args[0], ".bst"};
    debugger->stateLoad(pathname);
    return;
  }

  if(s == "state.save" && argc == 1) {
    string pathname = {interface->pathname, "loki/state-", args[0], ".bst"};
    debugger->stateSave(pathname);
    return;
  }

  if(s == "step" && argc == 0) {
    debugger->run();
    if(source == Debugger::Source::CPU) debugger->cpuStepFor = 1u;
    if(source == Debugger::Source::SMP) debugger->smpStepFor = 1u;
    return;
  }

  if(s == "step.for" && argc == 1) {
    debugger->run();
    if(source == Debugger::Source::CPU) debugger->cpuStepFor = (unsigned)decimal(args[0]);
    if(source == Debugger::Source::SMP) debugger->smpStepFor = (unsigned)decimal(args[0]);
    return;
  }

  if(s == "step.to" && argc == 1) {
    debugger->run();
    if(source == Debugger::Source::CPU) debugger->cpuStepTo = (unsigned)hex(args[0]);
    if(source == Debugger::Source::SMP) debugger->smpStepTo = (unsigned)hex(args[0]);
    return;
  }

  if(s == "tracer.enable" && argc <= 1) {
    string filename = {debugger->sourceName(source), "-trace-", string::datetime().transform(" :", "--"), ".log"};
    if(argc >= 1) filename = args[0];
    string pathname = {interface->pathname, "loki/", filename};
    debugger->tracerEnable(source, pathname);
    return;
  }

  if(s == "tracer.disable") {
    debugger->tracerDisable(source);
    return;
  }

  if(s == "tracer.mask.enable") {
    debugger->tracerMaskEnable(source);
    return;
  }

  if(s == "tracer.mask.disable") {
    debugger->tracerMaskDisable(source);
    return;
  }

  if(s == "usage.reset") {
    if(source == Debugger::Source::CPU) memset(debugger->cpuUsage, 0x00, 0x1000000);
    if(source == Debugger::Source::APU) memset(debugger->apuUsage, 0x00, 0x10000);
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

void Terminal::echoAliases() {
  if(aliases.size() == 0) return echo("No aliases defined\n");
  echo("#    alias\n");
  echo("---  -----\n");
  for(unsigned n = 0; n < aliases.size(); n++) {
    echo(format<-3>(n), "  ", aliases[n].name, " => ", aliases[n].mapping, "\n");
  }
}

void Terminal::echoHotkeys() {
  if(hotkeys.size() == 0) return echo("No hotkeys defined\n");
  echo("#    hotkey\n");
  echo("---  ------\n");
  for(unsigned n = 0; n < hotkeys.size(); n++) {
    echo(format<-3>(n), "  ", hotkeys[n].name, " => ", hotkeys[n].mapping, "\n");
  }
}

void Terminal::inputEvent(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) {
  if(focused() == false) return;
  if(device.isKeyboard() == false) return;    //only capture keyboard events
  if(oldValue != 0 || newValue != 1) return;  //only capture key down events
  string name = device.group[group].input[input].name;

  for(auto& hotkey : hotkeys) {
    if(name != hotkey.name) continue;
    command(hotkey.mapping);
  }
}

void Terminal::reset() {
  console.reset();
}

void Terminal::print(const string& text) {
  console.print(text);
}

void Terminal::setColors() {
  console.setBackgroundColor({
    (uint8)(settings->terminal.backgroundColor >> 16),
    (uint8)(settings->terminal.backgroundColor >>  8),
    (uint8)(settings->terminal.backgroundColor >>  0)
  });

  console.setForegroundColor({
    (uint8)(settings->terminal.foregroundColor >> 16),
    (uint8)(settings->terminal.foregroundColor >>  8),
    (uint8)(settings->terminal.foregroundColor >>  0)
  });
}
