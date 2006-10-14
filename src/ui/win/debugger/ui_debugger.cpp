bool DebugWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_CLOSE: {
    debugger.deactivate();
    return true;
  } break;

  case EVENT_CLICKED: {
    if(info.control == &Run) {
      if(bsnes->get_state() == bSNES::RUN) {
        bsnes->set_state(bSNES::STOP);
      } else if(bsnes->get_state() == bSNES::STOP) {
        bsnes->set_state(bSNES::RUN);
      }
    } else if(info.control == &Tracer) {
      wTracer.Show();
    } else if(info.control == &Memory) {
      wMemory.Show();
    }
  } break;

  }

  return false;
}

void DebugWindow::Update() {
  if(Visible() == false)return;

char t[250 * 132];
  sprintf(t, "");
  for(uint i = 0; i < buffer.count; i++) {
    strcat(t, buffer.line[i]);
    if(i != buffer.count - 1)strcat(t, "\r\n");
  }
  Console.SetText(t);
  Console.SetSelection(0, -1); //scroll cursor to bottom of window
}

void DebugWindow::Print(uint source, const char *str) {
  if(!hwnd)return;

  switch(source) {
  case source::none:
    break;
  case source::debug:
    if(output.debug == false)return;
    break;
  case source::cpu:
    if(output.cpu == false)return;
    break;
  case source::smp:
    if(output.smp == false)return;
    break;
  }

  for(uint i = 0; i < buffer.count - 1; i++) {
    strcpy(buffer.line[i], buffer.line[i + 1]);
  }

  strcpy(buffer.line[buffer.count - 1], str);
char *t = buffer.line[buffer.count - 1];
  if(strlen(t) > 100) {
    t[ 97] = '.';
    t[ 98] = '.';
    t[ 99] = '.';
    t[100] = 0;
  }
  Update();
}

void DebugWindow::Clear() {
  for(uint i = 0; i < buffer.count; i++) {
    strcpy(buffer.line[i], "");
  }
  Console.SetText("");
}

//called by bSNES::set_state() to update debug UI
void DebugWindow::SetState(uint state) {
  if(Visible() == false)return;

  switch(state) {
  case bSNES::RUN:  Run.SetText(" Stop"); break;
  case bSNES::STOP: Run.SetText(" Run");  break;
  }
}

void DebugWindow::Show() {
  Run.SetText((bsnes->get_state() == bSNES::RUN) ? " Stop" : " Run");
  Window::Show();
  Update();
}

void DebugWindow::Setup() {
int x = 5, y = 5;
static int bnwidth = 80;
  Run.Create(this, "visible|left", x, y, bnwidth, 20, " Run");
  x += bnwidth;
  Tracer.Create(this, "visible|left", x, y, bnwidth, 20, " Tracer");
  x += bnwidth;
  Memory.Create(this, "visible|left", x, y, bnwidth, 20, " Memory");
  x += bnwidth;

  x  = 5;
  y += 25;

  Console.Create(this, "visible|edge|multiline|vscroll|readonly", x, y, 725, 290);
  Console.SetFont(global::fwf);
  Console.SetBackgroundColor(32, 32, 32);
  Console.SetTextColor(255, 255, 255);
  y += 293;

  Command.Create(this, "visible|edge|disabled", x, y, 725, 22, "<disabled>");
  Command.SetFont(global::fwf);
  Command.SetBackgroundColor(32, 32, 32);
  Command.SetTextColor(255, 255, 255);
  y += 22;

  buffer.count = minmax<20, 250>((uint)config::debugger.console_lines);
  Clear();
}

DebugWindow::DebugWindow() {
  output.debug = true;
  output.cpu   = true;
  output.smp   = true;
}

DebugWindow::~DebugWindow() {}
