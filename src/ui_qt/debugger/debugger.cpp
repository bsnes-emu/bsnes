#include "breakpoint.cpp"
#include "memory.cpp"

void Debugger::setup() {
  window = new QWidget;
  window->setObjectName("debugger");
  window->setWindowTitle("Debugger");

  layout = new QVBoxLayout;
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  window->setLayout(layout);

  menu = new QMenuBar;
  layout->setMenuBar(menu);

  tools = menu->addMenu("Tools");
  tools_breakpoint = tools->addAction("Breakpoint Editor");
  tools_memory = tools->addAction("Memory Editor");

  options = menu->addMenu("Options");
  options->setEnabled(false);

  console = new QTextEdit;
  console->setFont(QFont(Style::Monospace));
  console->setReadOnly(true);
  layout->addWidget(console);

  command = new QLineEdit;
  command->setFont(QFont(Style::Monospace));
  layout->addWidget(command);

  window->setMinimumSize(725, 425);
  window->resize(725, 425);
  connect(command, SIGNAL(returnPressed()), this, SLOT(execCommand()));
  connect(tools_breakpoint, SIGNAL(triggered()), this, SLOT(showBreakpointEditor()));
  connect(tools_memory, SIGNAL(triggered()), this, SLOT(showMemoryEditor()));

  breakpointEditor = new BreakpointEditor;
  breakpointEditor->setup();

  memoryEditor = new MemoryEditor;
  memoryEditor->setup();

  syncUi();
}

void Debugger::syncUi() {
  memoryEditor->syncUi();
}

void Debugger::show() {
  utility.showCentered(window);
  command->setFocus();
}

void Debugger::echo(const char *message) {
  console->moveCursor(QTextCursor::End);
  console->insertPlainText(message);
}

void Debugger::clear() {
  console->setHtml("");
}

void Debugger::execCommand() {
  string s = command->text().toUtf8().data();
  command->setText("");

  if(s == "clear") {
    clear();
    return;
  }

  if(s == "run") {
    SNES::debugger.break_event = SNES::Debugger::None;
    SNES::debugger.step_cpu = false;
    SNES::debugger.step_smp = false;
    return;
  }

  if(s == "step cpu") {
    SNES::debugger.step_cpu = true;
    return;
  }

  if(s == "step smp") {
    SNES::debugger.step_smp = true;
    return;
  }

  if(s == "trace on") {
    SNES::debugger.tracefile.open("trace.log", file::mode_write);
    echo("Tracing enabled.\n");
    return;
  }

  if(s == "trace off") {
    SNES::debugger.trace_cpu = false;
    SNES::debugger.trace_smp = false;
    SNES::debugger.tracefile.close();
    echo("Tracing disabled.\n");
    return;
  }

  if(s == "trace cpu on") {
    if(SNES::debugger.tracefile.open() == false) return;
    SNES::debugger.trace_cpu = true;
    echo("S-CPU tracing enabled.\n");
    return;
  }

  if(s == "trace cpu off") {
    SNES::debugger.trace_cpu = false;
    echo("S-CPU tracing disabled.\n");
    return;
  }

  if(s == "trace smp on") {
    if(SNES::debugger.tracefile.open() == false) return;
    SNES::debugger.trace_smp = true;
    echo("S-SMP tracing enabled.\n");
    return;
  }

  if(s == "trace smp off") {
    SNES::debugger.trace_smp = false;
    echo("S-SMP tracing disabled.\n");
    return;
  }

  if(SNES::cartridge.loaded() == false) return;

  if(s == "") {
    SNES::debugger.break_event = SNES::Debugger::None;
  }
}

void Debugger::showBreakpointEditor() {
  utility.showCentered(breakpointEditor->window);
}

void Debugger::showMemoryEditor() {
  utility.showCentered(memoryEditor->window);
}

void Debugger::event() {
  char t[256];

  switch(SNES::debugger.break_event) {
    case SNES::Debugger::BreakpointHit: {
      unsigned n = SNES::debugger.breakpoint_hit;
      echo(utf8() << "Breakpoint " << n << " hit (" << SNES::debugger.breakpoint[n].counter << ").\n");

      if(SNES::debugger.breakpoint[n].mode == SNES::Debugger::Breakpoint::Exec) {
        if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::CPUBus) {
          SNES::debugger.step_cpu = true;
          SNES::cpu.disassemble_opcode(t);
          echo(utf8() << t << "\n");
        }

        if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::APURAM) {
          SNES::debugger.step_smp = true;
          SNES::smp.disassemble_opcode(t);
          echo(utf8() << t << "\n");
        }
      }
    } break;

    case SNES::Debugger::CPUStep: {
      SNES::cpu.disassemble_opcode(t);
      echo(utf8() << t << "\n");
    } break;

    case SNES::Debugger::SMPStep: {
      SNES::smp.disassemble_opcode(t);
      echo(utf8() << t << "\n");
    } break;
  }
}
