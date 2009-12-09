#include "../ui-base.hpp"

#if defined(DEBUGGER)

#include "debugger.moc"
Debugger *debugger;

#include "hexeditor.cpp"
#include "disassembler.cpp"
#include "breakpoint.cpp"
#include "memory.cpp"
#include "vramviewer.cpp"
#include "tracer.cpp"

Debugger::Debugger() : QbWindow(config().geometry.debugger) {
  setObjectName("debugger");
  setWindowTitle("Debugger");

  layout = new QHBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  menu = new QMenuBar;
  layout->setMenuBar(menu);

  tools = menu->addMenu("Tools");
  tools_disassembler = tools->addAction("Disassembler ...");
  tools_breakpoint = tools->addAction("Breakpoint Editor ...");
  tools_memory = tools->addAction("Memory Editor ...");
  tools_vramViewer = tools->addAction("Video RAM Viewer ...");

  miscOptions = menu->addMenu("Misc");
  miscOptions_clear = miscOptions->addAction("Clear Console");

  console = new QTextEdit;
  console->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  console->setReadOnly(true);
  console->setFont(QFont(Style::Monospace));
  console->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  console->setMinimumWidth((92 + 4) * console->fontMetrics().width(' '));
  console->setMinimumHeight((25 + 1) * console->fontMetrics().height());
  layout->addWidget(console);

  controlLayout = new QVBoxLayout;
  controlLayout->setSpacing(0);
  layout->addLayout(controlLayout);

  commandLayout = new QHBoxLayout;
  controlLayout->addLayout(commandLayout);

  runBreak = new QPushButton("Break");
  commandLayout->addWidget(runBreak);
  commandLayout->addSpacing(Style::WidgetSpacing);

  stepInstruction = new QPushButton("Step");
  commandLayout->addWidget(stepInstruction);

  controlLayout->addSpacing(Style::WidgetSpacing);

  stepCPU = new QCheckBox("Step S-CPU");
  controlLayout->addWidget(stepCPU);

  stepSMP = new QCheckBox("Step S-SMP");
  controlLayout->addWidget(stepSMP);

  traceCPU = new QCheckBox("Trace S-CPU opcodes");
  controlLayout->addWidget(traceCPU);

  traceSMP = new QCheckBox("Trace S-SMP opcodes");
  controlLayout->addWidget(traceSMP);

  traceMask = new QCheckBox("Enable trace mask");
  controlLayout->addWidget(traceMask);

  spacer = new QWidget;
  spacer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  controlLayout->addWidget(spacer);

  disassembler = new Disassembler;
  breakpointEditor = new BreakpointEditor;
  memoryEditor = new MemoryEditor;
  vramViewer = new VramViewer;
  tracer = new Tracer;

  connect(tools_disassembler, SIGNAL(triggered()), this, SLOT(showDisassembler()));
  connect(tools_breakpoint, SIGNAL(triggered()), this, SLOT(showBreakpointEditor()));
  connect(tools_memory, SIGNAL(triggered()), this, SLOT(showMemoryEditor()));
  connect(tools_vramViewer, SIGNAL(triggered()), this, SLOT(showVramViewer()));
  connect(miscOptions_clear, SIGNAL(triggered()), this, SLOT(clear()));

  connect(runBreak, SIGNAL(released()), this, SLOT(toggleRunStatus()));
  connect(stepInstruction, SIGNAL(released()), this, SLOT(stepAction()));
  connect(stepCPU, SIGNAL(released()), this, SLOT(synchronize()));
  connect(stepSMP, SIGNAL(released()), this, SLOT(synchronize()));
  connect(traceCPU, SIGNAL(stateChanged(int)), tracer, SLOT(setCpuTraceState(int)));
  connect(traceSMP, SIGNAL(stateChanged(int)), tracer, SLOT(setSmpTraceState(int)));
  connect(traceMask, SIGNAL(stateChanged(int)), tracer, SLOT(setTraceMaskState(int)));

  frameCounter = 0;
  synchronize();
  resize(855, 425);
}

void Debugger::modifySystemState(unsigned state) {
  string usagefile = string() << dir(utility.cartridge.baseName) << "usage.bin";
  file fp;

  if(state == Utility::LoadCartridge) {
    if(fp.open(usagefile, file::mode_read)) {
      fp.read(SNES::cpu.usage, 1 << 24);
      fp.read(SNES::smp.usage, 1 << 16);
      fp.close();
    } else {
      memset(SNES::cpu.usage, 0x00, 1 << 24);
      memset(SNES::smp.usage, 0x00, 1 << 16);
    }
  }

  if(state == Utility::UnloadCartridge) {
    if(fp.open(usagefile, file::mode_write)) {
      fp.write(SNES::cpu.usage, 1 << 24);
      fp.write(SNES::smp.usage, 1 << 16);
      fp.close();
    }
  }
}

void Debugger::synchronize() {
  runBreak->setText(application.debug ? "Run" : "Break");
  stepInstruction->setEnabled(SNES::cartridge.loaded() && application.debug && (stepCPU->isChecked() || stepSMP->isChecked()));
  SNES::debugger.step_cpu = application.debug && stepCPU->isChecked();
  SNES::debugger.step_smp = application.debug && stepSMP->isChecked();

  memoryEditor->synchronize();
}

void Debugger::echo(const char *message) {
  console->moveCursor(QTextCursor::End);
  console->insertHtml(message);
}

void Debugger::clear() {
  console->setHtml("");
}

void Debugger::showDisassembler() {
  disassembler->show();
}

void Debugger::showBreakpointEditor() {
  breakpointEditor->show();
}

void Debugger::showMemoryEditor() {
  memoryEditor->show();
}

void Debugger::showVramViewer() {
  vramViewer->show();
  vramViewer->refresh();
}

void Debugger::toggleRunStatus() {
  application.debug = !application.debug;
  if(!application.debug) application.debugrun = false;
  synchronize();
}

void Debugger::stepAction() {
  application.debugrun = true;
}

void Debugger::event() {
  char t[256];

  switch(SNES::debugger.break_event) {
    case SNES::Debugger::BreakpointHit: {
      unsigned n = SNES::debugger.breakpoint_hit;
      echo(string() << "Breakpoint " << n << " hit (" << SNES::debugger.breakpoint[n].counter << ").<br>");

      if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::CPUBus) {
        SNES::debugger.step_cpu = true;
        SNES::cpu.disassemble_opcode(t, SNES::cpu.opcode_pc);
        string s = t;
        s.replace(" ", "&nbsp;");
        echo(string() << "<font color='#a00000'>" << s << "</font><br>");
        disassembler->refresh(Disassembler::CPU, SNES::cpu.opcode_pc);
      }

      if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::APURAM) {
        SNES::debugger.step_smp = true;
        SNES::smp.disassemble_opcode(t, SNES::smp.opcode_pc);
        string s = t;
        s.replace(" ", "&nbsp;");
        echo(string() << "<font color='#a00000'>" << t << "</font><br>");
        disassembler->refresh(Disassembler::SMP, SNES::smp.opcode_pc);
      }
    } break;

    case SNES::Debugger::CPUStep: {
      SNES::cpu.disassemble_opcode(t, SNES::cpu.regs.pc);
      string s = t;
      s.replace(" ", "&nbsp;");
      echo(string() << "<font color='#0000a0'>" << s << "</font><br>");
      disassembler->refresh(Disassembler::CPU, SNES::cpu.regs.pc);
    } break;

    case SNES::Debugger::SMPStep: {
      SNES::smp.disassemble_opcode(t, SNES::smp.regs.pc);
      string s = t;
      s.replace(" ", "&nbsp;");
      echo(string() << "<font color='#a00000'>" << s << "</font><br>");
      disassembler->refresh(Disassembler::SMP, SNES::smp.regs.pc);
    } break;
  }
}

//called once every time a video frame is rendered, used to update "auto refresh" tool windows
void Debugger::frameTick() {
  if(++frameCounter >= (SNES::system.region() == SNES::System::NTSC ? 60 : 50)) {
    frameCounter = 0;
    memoryEditor->autoUpdate();
    vramViewer->autoUpdate();
  }
}

#endif
