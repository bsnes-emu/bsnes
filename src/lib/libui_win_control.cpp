namespace libui {

/*****
 * FileLoad
 *****/

bool Window::file_load(char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

stringarray type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = strptr(f);
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|')pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = info.hwnd;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetOpenFileName(&ofn);
}

/*****
 * FileSave
 *****/

bool Window::file_save(char *filename, const char *filter, const char *path) {
string dir, f;
  strcpy(dir, path ? path : "");
  replace(dir, "/", "\\");

stringarray type, part;
  strcpy(f, "");
  split(type, "|", filter);
  for(int i = 0; i < count(type); i++) {
    split(part, ";", type[i]);
    if(count(part) != 2)continue;

    strcat(f, part[0]);
    strcat(f, " (");
    strcat(f, part[1]);
    strcat(f, ")|");
    replace(part[1], ",", ";");
    strcat(f, part[1]);
    strcat(f, "|");
  }

char *pf = strptr(f);
  for(int i = strlen(pf) - 1; i >= 0; i--) {
    if(pf[i] == '|')pf[i] = '\0';
  }

OPENFILENAME ofn;
  strcpy(filename, "");
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = info.hwnd;
  ofn.lpstrFilter     = pf;
  ofn.lpstrInitialDir = strptr(dir);
  ofn.lpstrFile       = filename;
  ofn.nMaxFile        = MAX_PATH;
  ofn.Flags           = OFN_EXPLORER | OFN_FILEMUSTEXIST;
  ofn.lpstrDefExt     = "";

  return GetSaveFileName(&ofn);
}

/*****
 * Control
 *****/

void Control::resize(uint width, uint height) {
  SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void Control::show() {
  ShowWindow(hwnd, SW_NORMAL);
}

void Control::hide() {
  ShowWindow(hwnd, SW_HIDE);
}

void Control::show(bool state) {
  (state == true) ? show() : hide();
}

bool Control::visible() {
  return (GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE) ? true : false;
}

void Control::enable() {
  EnableWindow(hwnd, true);
}

void Control::disable() {
  EnableWindow(hwnd, false);
}

void Control::enable(bool state) {
  (state == true) ? enable() : disable();
}

bool Control::enabled() {
  return !(GetWindowLong(hwnd, GWL_STYLE) & WS_DISABLED);
}

/*****
 * Menu
 *****/

void Window::menu_begin() {
  info.menu_check_list.reset();
}

void Window::menu_end() {
  SetMenu(info.hwnd_resize, info.menu_owner.group);
  SetMenu(info.hwnd, info.menu_owner.group);
  resize(info.width, info.height);
//check all menu radio items that need to be ...
  for(uint i = 0; i < info.menu_check_list.size(); i++) {
    info.menu_check_list[i]->check();
  }
  info.menu_check_list.reset();
}

void Window::menu_group_begin(MenuGroup &group) {
  info.control[info.control_index] = &group;
  group.id    = info.control_index++;
  group.owner = this;
  info.menu_group[++info.menu_group_index] = &group;
}

void Window::menu_group_end() {
MenuGroup *group = info.menu_group[info.menu_group_index--];
MenuGroup *owner = info.menu_group[info.menu_group_index];
  AppendMenu(owner->group, MF_STRING | MF_POPUP, (uint)group->group, group->caption);
}

void Window::menu_add_item(Control &item) {
  info.control[info.control_index] = &item;
MenuGroup *group = info.menu_group[info.menu_group_index];
  item.id    = info.control_index++;
  item.owner = this;

  switch(item.type) {

  case Control::MenuItem: {
    AppendMenu(group->group, MF_STRING, (uint)item.id, static_cast<MenuItem&>(item).caption);
  } break;

  case Control::MenuCheckItem: {
    AppendMenu(group->group, MF_STRING, (uint)item.id, static_cast<MenuCheckItem&>(item).caption);
  } break;

  case Control::MenuRadioItem: {
    AppendMenu(group->group, MF_STRING, (uint)item.id, static_cast<MenuRadioItem&>(item).caption);
  MenuRadioItem &radio = static_cast<MenuRadioItem&>(item);
    if(&radio == &radio.group[0]) { info.menu_check_list[info.menu_check_list.size()] = &radio; }
  } break;

  };
}

void Window::menu_add_separator() {
MenuGroup *group = info.menu_group[info.menu_group_index];
  AppendMenu(group->group, MF_SEPARATOR, 0, "");
}

/*****
 * MenuGroup
 *****/

MenuGroup& MenuGroup::create(const char *_caption) {
  type    = Control::MenuGroup;
  group   = CreatePopupMenu();
  caption = strdup(_caption);
  return *this;
}

/*****
 * MenuItem
 *****/

MenuItem& MenuItem::create(const char *_caption) {
  type = Control::MenuItem;
  caption = strdup(_caption);
  return *this;
}

/*****
 * MenuCheckItem
 *****/

MenuCheckItem& MenuCheckItem::create(const char *_caption) {
  type = Control::MenuCheckItem;
  caption = strdup(_caption);
  return *this;
}

void MenuCheckItem::check() {
  CheckMenuItem(owner->info.menu_owner.group, id, MF_CHECKED);
}

void MenuCheckItem::uncheck() {
  CheckMenuItem(owner->info.menu_owner.group, id, MF_UNCHECKED);
}

void MenuCheckItem::check(bool state) {
  (state == true) ? check() : uncheck();
}

bool MenuCheckItem::checked() {
MENUITEMINFO info;
  memset(&info, 0, sizeof(info));
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(owner->info.menu_owner.group, id, false, &info);
  return (info.fState & MFS_CHECKED);
}

/*****
 * MenuRadioItem
 *****/

MenuRadioItem& MenuRadioItem::create(ControlGroup &list, const char *_caption) {
  if(list.count() == 0)throw;
  type = Control::MenuRadioItem;
  caption = strdup(_caption);
  group = list;
  return *this;
}

void MenuRadioItem::check() {
  for(uint i = 0; i < group.count(); i++) {
    CheckMenuItem(group[i].owner->info.menu_owner.group, group[i].id, MF_UNCHECKED);
  }
  CheckMenuItem(owner->info.menu_owner.group, id, MF_CHECKED);
}

bool MenuRadioItem::checked() {
MENUITEMINFO info;
  memset(&info, 0, sizeof(info));
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(owner->info.menu_owner.group, id, false, &info);
  return (info.fState & MFS_CHECKED);
}

/*****
 * Container
 *****/

Container& Container::create(const char *style, uint width, uint height) {
  type = Control::Container;
  hwnd = CreateWindow("libui_class", "", WS_CHILD | WS_VISIBLE,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  return *this;
}

void Container::attach(Control &control, uint x, uint y) {
  owner->attach(control, x, y);
  SetParent(control.hwnd, hwnd);
}

void Container::move(Control &control, uint x, uint y) {
  SetWindowPos(control.hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

/*****
 * Canvas
 *****/

Canvas &Canvas::create(const char *style, uint width, uint height) {
  type = Control::Canvas;

char classname[4096];
  sprintf(classname, "libui_class_%d", window_count++);
WNDCLASS wc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
  wc.hInstance     = GetModuleHandle(0);
  wc.lpfnWndProc   = libui_wndproc;
  wc.lpszClassName = classname;
  wc.lpszMenuName  = 0;
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  RegisterClass(&wc);

  hwnd = CreateWindow(classname, "", WS_CHILD | WS_VISIBLE,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  return *this;
}

WindowHandle Canvas::handle() {
  return (WindowHandle)hwnd;
}

void Canvas::set_background_color(uint8 r, uint8 g, uint8 b) {
HBRUSH old_brush = background;
  background = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
  SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)background);
  InvalidateRect(hwnd, 0, TRUE);
  if(old_brush) { DeleteObject((HGDIOBJ)old_brush); }
}

/*****
 * Frame
 *****/

Frame& Frame::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Frame;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  return *this;
}

/*****
 * Label
 *****/

Label& Label::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Label;
  hwnd = CreateWindow("STATIC", caption ? caption : "", WS_CHILD | WS_VISIBLE | SS_NOPREFIX | SS_ENDELLIPSIS,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  return *this;
}

/*****
 * Button
 *****/

Button& Button::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Button;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  return *this;
}

/*****
 * Checkbox
 *****/

Checkbox& Checkbox::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Checkbox;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  return *this;
}

void Checkbox::check() {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)TRUE, 0);
}

void Checkbox::uncheck() {
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)FALSE, 0);
}

void Checkbox::check(bool state) {
  (state == true) ? check() : uncheck();
}

bool Checkbox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

/*****
 * Radiobox
 *****/

Radiobox& Radiobox::create(ControlGroup &list, const char *style, uint width, uint height, const char *caption) {
  if(list.count() == 0)throw;
  type = Control::Radiobox;
  group = list;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  if(this == &group[0])check();
  return *this;
}

void Radiobox::check() {
  for(uint i = 0; i < group.count(); i++) {
    SendMessage(group[i].hwnd, BM_SETCHECK, (WPARAM)FALSE, 0);
  }
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)TRUE, 0);
}

bool Radiobox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

/*****
 * Editbox
 *****/

Editbox& Editbox::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Editbox;
  multiline = readonly = vscroll = hscroll = false;
stringarray part;
  split(part, "|", style);
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "multiline")multiline = true;
    if(part[i] == "readonly")readonly = true;
    if(part[i] == "vscroll")vscroll = true;
    if(part[i] == "hscroll")hscroll = true;
  }
string data = caption;
  replace(data, "\r", "");
  replace(data, "\n", "\r\n");
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strptr(data),
    WS_CHILD | WS_VISIBLE |
    (multiline == true ? ES_MULTILINE : 0) |
    (readonly == true ? ES_READONLY : 0) |
    (vscroll == true ? WS_VSCROLL : ES_AUTOVSCROLL) |
    (hscroll == true ? WS_HSCROLL : ES_AUTOHSCROLL),
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  return *this;
}

/*****
 * Listbox
 *****/

Listbox& Listbox::create(const char *style, uint width, uint height, const char *columns, const char *data) {
stringarray part;
  header = false;
  split(part, "|", style);
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "header")header = true;
  }

  type = Control::Listbox;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
    WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS |
    (header == true ? 0 : LVS_NOCOLUMNHEADER),
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  ListView_SetExtendedListViewStyle(hwnd, LVS_EX_FULLROWSELECT);

  split(part, "|", columns);
  column_count = count(part);
  for(uint i = 0; i < column_count; i++) {
  LVCOLUMN column;
    column.mask     = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt      = LVCFMT_LEFT;
    column.iSubItem = count(part);
    column.pszText  = (LPSTR)strptr(part[i]);
    ListView_InsertColumn(hwnd, i, &column);
  }

  if(strcmp(data, "")) {
    split(part, "||", data);
    for(uint i = 0; i < count(part); i++) {
      add_item(strptr(part[i]));
    }
  }

  autosize_columns();
  return *this;
}

void Listbox::autosize_columns() {
  for(uint i = 0; i < column_count; i++) {
    ListView_SetColumnWidth(hwnd, i, LVSCW_AUTOSIZE_USEHEADER);
  }
}

void Listbox::set_column_width(uint column, uint width) {
  ListView_SetColumnWidth(hwnd, column, width);
}

void Listbox::add_item(const char *data) {
stringarray part;
  split(part, "|", data);

LVITEM item;
uint pos = ListView_GetItemCount(hwnd);
  item.mask     = LVIF_TEXT;
  item.iItem    = pos;
  item.iSubItem = 0;
  item.pszText  = (LPSTR)strptr(part[0]);
  ListView_InsertItem(hwnd, &item);

  for(uint i = 1; i < count(part); i++) {
    ListView_SetItemText(hwnd, pos, i, (LPSTR)strptr(part[i]));
  }
}

int Listbox::get_selection() {
uint count = ListView_GetItemCount(hwnd);
  for(uint i = 0; i < count; i++) {
    if(ListView_GetItemState(hwnd, i, LVIS_SELECTED))return i;
  }
  return -1;
}

void Listbox::set_selection(int index) {
uint count = ListView_GetItemCount(hwnd);
  for(uint i = 0; i < count; i++) {
  uint state = ListView_GetItemState(hwnd, i, LVIS_FOCUSED);
    ListView_SetItemState(hwnd, i, LVIS_FOCUSED,  (i == index) ? LVIS_FOCUSED  : 0);
    ListView_SetItemState(hwnd, i, LVIS_SELECTED, (i == index) ? LVIS_SELECTED : 0);
  }
}

void Listbox::reset() {
  ListView_DeleteAllItems(hwnd);
}

/*****
 * Combobox
 *****/

Combobox& Combobox::create(const char *style, uint width, uint height, const char *caption) {
  type = Control::Combobox;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", caption ? caption : "",
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    0, 0, width, 200, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  if(strcmp(caption, "")) {
  stringarray temp;
    split(temp, "|", caption);
    for(uint i = 0; i < count(temp); i++) { add_item(strptr(temp[i])); }
    set_selection(0);
  }
  return *this;
}

void Combobox::add_item(const char *data) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)data);
}

void Combobox::set_selection(int index) {
  SendMessage(hwnd, CB_SETCURSEL, index, 0);
}

int Combobox::get_selection() {
  return SendMessage(hwnd, CB_GETCURSEL, 0, 0);
}

void Combobox::reset() {
  SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
}

/*****
 * Progressbar
 *****/

Progressbar& Progressbar::create(const char *style, uint width, uint height) {
  type = Control::Progressbar;
  hwnd = CreateWindow(PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
  return *this;
}

void Progressbar::set_progress(uint progress) {
  progress = minmax<0, 100>(progress);
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)progress, 0);
}

uint Progressbar::get_progress() {
  return minmax<0, 100>(SendMessage(hwnd, PBM_GETPOS, 0, 0));
}

/*****
 * Slider
 *****/

Slider& Slider::create(const char *style, uint width, uint height, uint min, uint max) {
  type = Control::Slider;
  orientation = 0;
stringarray part;
  split(part, "|", style);
  for(uint i = 0; i < count(part); i++) {
    if(part[i] == "horizontal")orientation = 0;
    if(part[i] == "vertical")orientation = 1;
  }
  hwnd = CreateWindow(TRACKBAR_CLASS, "",
    WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_BOTH | (orientation == 0 ? TBS_HORZ : TBS_VERT),
    0, 0, width, height, libui::message_window, (HMENU)100, GetModuleHandle(0), 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(min, max));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)((max - min) >> 3));
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)min);
  return *this;
}

void Slider::set_position(uint position) {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

uint Slider::get_position() {
  return SendMessage(hwnd, TBM_GETPOS, 0, 0);
}

};
