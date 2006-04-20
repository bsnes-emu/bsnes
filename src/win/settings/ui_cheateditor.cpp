bool CheatEditorWindow::Event(EventInfo &info) {
  switch(info.event_id) {

  case EVENT_DOUBLECLICKED: {
    if(info.control == &CheatList && CheatList.GetItemCount() != 0) {
    int sel = CheatList.GetSelection();
      if(sel == -1 || sel >= cheat.count())break;

    bool enabled = cheat.enabled(sel);
      (enabled == true) ? cheat.disable(sel) : cheat.enable(sel);
      UpdateItem(sel);
      LoadItem(sel);
    }
  } break;

  case EVENT_CHANGED: {
    if(info.control == &CheatList) {
      LoadItem(CheatList.GetSelection());
    }
  } break;

  case EVENT_CLICKED: {
    if(info.control == &AddCode)   CheatAdd();
    if(info.control == &EditCode)  CheatEdit();
    if(info.control == &DeleteCode)CheatDelete();
    if(info.control == &ClearCode) CheatClear();
  } break;

  }

  return false;
}

void CheatEditorWindow::CheatAdd() {
char code[16 + 1], desc[128 + 1];
  Code.GetText(code,  16);
  Desc.GetText(desc, 128);
  if(strmatch(code, "") == true)return;

  cheat.add(Enabled.Checked(), code, desc);
  Refresh();
}

void CheatEditorWindow::CheatEdit() {
int sel = CheatList.GetSelection();
  if(sel == -1 || sel >= cheat.count())return;

char code[16 + 1], desc[128 + 1];
  Code.GetText(code,  16);
  Desc.GetText(desc, 128);

  cheat.edit(sel, Enabled.Checked(), code, desc);
  UpdateItem(sel);
}

void CheatEditorWindow::CheatDelete() {
int sel = CheatList.GetSelection();
  if(sel == -1 || sel >= cheat.count())return;

  cheat.remove(sel);
  Refresh();
}

void CheatEditorWindow::CheatClear() {
  Enabled.Uncheck();
  Code.SetText("");
  Desc.SetText("");
}

void CheatEditorWindow::Refresh() {
  Clear();

char status[16 + 1], desc[128 + 1], code[16 + 1], result[16 + 1];
uint32 addr;
uint8  data;
bool   enabled;
  for(uint i = 0; i < cheat.count(); i++) {
    cheat.get(i, enabled, addr, data, code, desc);
    strcpy(status, enabled == true ? "On" : "Off");
    sprintf(result, "%0.6x = %0.2x", addr, data);
    AddItem(status, desc, code, result);
  }

  if(cheat.count()) {
    CheatList.SetSelection(0);
  }
}

void CheatEditorWindow::Clear() {
  CheatList.DeleteAllItems();
  CheatClear();
}

void CheatEditorWindow::LoadItem(int n) {
  if(n == -1 || n >= cheat.count()) {
    CheatClear();
    return;
  }

char status[16 + 1], desc[128 + 1], code[16 + 1], result[16 + 1];
uint32 addr;
uint8  data;
bool   enabled;
  cheat.get(n, enabled, addr, data, code, desc);

  Enabled.Check(enabled);
  Code.SetText(code);
  Desc.SetText(desc);
}

void CheatEditorWindow::UpdateItem(int n) {
  if(n == -1 || n >= cheat.count())return;

char status[16 + 1], desc[128 + 1], code[16 + 1], result[16 + 1];
uint32 addr;
uint8  data;
bool   enabled;
  cheat.get(n, enabled, addr, data, code, desc);

char t[256];
  sprintf(t, "%s|%s|%s|%0.6x = %0.2x", enabled == true ? "On" : "Off", desc, code, addr, data);
  CheatList.SetItemText(n, t);
}

void CheatEditorWindow::AddItem(const char *status, const char *desc, const char *code, const char *result) {
char t[256];
  sprintf(t, "%s|%s|%s|%s", status, desc, code, result);
  CheatList.AddItem(t);
}

void CheatEditorWindow::Show() {
  Refresh();
  Window::Show();
}

void CheatEditorWindow::Setup() {
  Header.Create(this, "visible", 0, 0, 475, 30, "Cheat Code Editor");
  Header.SetFont(global::font_header);

int x = 15, y = 30;
  CheatList.Create(this, "visible|edge", x, y, 460, 205);
  CheatList.AddColumn(Listview::LEFT,  50, "Status");
  CheatList.AddColumn(Listview::LEFT, 246, "Description");
  CheatList.AddColumn(Listview::LEFT,  80, "Code");
  CheatList.AddColumn(Listview::LEFT,  80, "Result");
  y += 210;

  Enabled.Create(this, "visible", x, y, 460, 15, "Enabled");
  y += 15;
  CodeLabel.Create(this, "visible", x, y + 4, 65, 15, "Code:");
  Code.Create(this, "visible|edge", x + 65, y, 75, 20);
  Code.SetFont(global::fwf);
  y += 20;
  DescLabel.Create(this, "visible", x, y + 4, 65, 15, "Description:");
  Desc.Create(this, "visible|edge", x + 65, y, 250, 20);
  y += 25;

  AddCode.Create   (this, "visible", x,       y, 65, 25, "Add");
  EditCode.Create  (this, "visible", x +  65, y, 65, 25, "Edit");
  DeleteCode.Create(this, "visible", x + 130, y, 65, 25, "Delete");
  ClearCode.Create (this, "visible", x + 195, y, 65, 25, "Clear");
  y += 30;

  GlobalEnable.Create(this, "visible|disabled", x, y, 460, 15, "Enable cheat system");
  GlobalEnable.Check();
  y += 15;

  Clear();
}
