namespace libui {

/*****
 * Control
 *****/

void Control::resize(uint width, uint height) {
  SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void Control::focus() {
  if(hwnd) { SetFocus(hwnd); }
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
 * MenuBar
 *****/

void MenuBar::create(Window &r_owner) {
  type  = ControlType::MenuBar;
  group = CreateMenu();
  r_owner.attach(*this);
  owner->info.menu_check_list.reset();
}

void MenuBar::finish() {
  show();

//check all menu radio items that need to be ...
  for(uint i = 0; i < owner->info.menu_check_list.size(); i++) {
    CheckMenuItem(owner->info.menu_check_list[i]->parent, owner->info.menu_check_list[i]->id, MF_CHECKED);
  }
  owner->info.menu_check_list.reset();
}

void MenuBar::show() {
  SetMenu(owner->info.hwnd_resize, group);
  SetMenu(owner->info.hwnd,        group);
  owner->resize(owner->info.width, owner->info.height);
}

void MenuBar::hide() {
  SetMenu(owner->info.hwnd_resize, 0);
  SetMenu(owner->info.hwnd,        0);
  owner->resize(owner->info.width, owner->info.height);
}

void MenuBar::show(bool state) {
  (state == true) ? show() : hide();
}

bool MenuBar::visible() {
  return GetMenu(owner->info.hwnd);
}

/*****
 * MenuGroup
 *****/

void MenuGroup::create(MenuBar &r_owner, const char *_caption) {
  type    = ControlType::MenuGroup;
  owner   = r_owner.owner;
  parent  = r_owner.group;
  group   = CreatePopupMenu();
  caption = strdup(_caption);
  owner->attach(*this);
}

void MenuGroup::create(MenuGroup &r_owner, const char *_caption) {
  type    = ControlType::MenuGroup;
  owner   = r_owner.owner;
  parent  = r_owner.group;
  group   = CreatePopupMenu();
  caption = strdup(_caption);
  owner->attach(*this);
}

void MenuGroup::finish() {
  AppendMenu(parent, MF_STRING | MF_POPUP, (uint)group, caption);
}

/*****
 * MenuItem
 *****/

void MenuItem::create(MenuGroup &r_owner, const char *caption) {
  type  = ControlType::MenuItem;
  owner = r_owner.owner;
  owner->attach(*this);
  AppendMenu(r_owner.group, MF_STRING, (uint)id, caption);
}

/*****
 * MenuCheckItem
 *****/

void MenuCheckItem::create(MenuGroup &r_owner, const char *caption) {
  type   = ControlType::MenuCheckItem;
  owner  = r_owner.owner;
  parent = r_owner.group;
  owner->attach(*this);
  AppendMenu(r_owner.group, MF_STRING, (uint)id, caption);
}

void MenuCheckItem::check() {
  if(checked() == true)return;
  CheckMenuItem(parent, id, MF_CHECKED);
  owner->message(Message::Clicked, this);
}

void MenuCheckItem::uncheck() {
  if(checked() == false)return;
  CheckMenuItem(parent, id, MF_UNCHECKED);
  owner->message(Message::Clicked, this);
}

void MenuCheckItem::check(bool state) {
  (state == true) ? check() : uncheck();
}

bool MenuCheckItem::checked() {
MENUITEMINFO info;
  memset(&info, 0, sizeof(info));
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, id, false, &info);
  return (info.fState & MFS_CHECKED);
}

/*****
 * MenuRadioItem
 *****/

void MenuRadioItem::create(MenuGroup &r_owner, ControlGroup &list, const char *caption) {
  if(list.count() == 0)throw;
  type   = ControlType::MenuRadioItem;
  owner  = r_owner.owner;
  parent = r_owner.group;
  owner->attach(*this);
  AppendMenu(r_owner.group, MF_STRING, (uint)id, caption);
  group = list;
  if(&group[0] == this) { owner->info.menu_check_list[owner->info.menu_check_list.size()] = this; }
}

void MenuRadioItem::check() {
  if(checked() == true)return;
//uncheck all items in group except for current item ...
  for(uint i = 0; i < group.count(); i++) {
    CheckMenuItem(parent, group[i].id, (id == group[i].id) ? MF_CHECKED : MF_UNCHECKED);
  }
  owner->message(Message::Clicked, this);
}

bool MenuRadioItem::checked() {
MENUITEMINFO info;
  memset(&info, 0, sizeof(info));
  info.cbSize = sizeof(info);
  info.fMask = MIIM_STATE;
  GetMenuItemInfo(parent, id, false, &info);
  return (info.fState & MFS_CHECKED);
}

/*****
 * MenuSeparator
 *****/

void MenuSeparator::create(MenuGroup &r_owner) {
  AppendMenu(r_owner.group, MF_SEPARATOR, 0, "");
}

/*****
 * Panel
 *****/

void Panel::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type = ControlType::Panel;
  hwnd = CreateWindow("libui_class", "", WS_CHILD | WS_VISIBLE,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  r_owner.attach(*this);
}

void Panel::attach(Window &window) {
  if(attached) { detach(); }
  attached = &window;
  SetParent(attached->info.hwnd, hwnd);
  SetWindowLong(attached->info.hwnd, GWL_STYLE, WS_CHILD);
  SetWindowPos(attached->info.hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOSIZE);
  ShowWindow(attached->info.hwnd, SW_NORMAL);
}

void Panel::detach() {
  if(!attached) { return; }
  ShowWindow(attached->info.hwnd, SW_HIDE);
  SetWindowLong(attached->info.hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX);
  SetParent(attached->info.hwnd, NULL);
  attached = 0;
}

/*****
 * Container
 *****/

void Container::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type = ControlType::Container;

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
    0, 0, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
}

void Container::set_background_color(uint8 r, uint8 g, uint8 b) {
HBRUSH old_brush = background;
  background = (HBRUSH)CreateSolidBrush(RGB(r, g, b));
  SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)background);
  InvalidateRect(hwnd, 0, TRUE);
  if(old_brush) { DeleteObject((HGDIOBJ)old_brush); }
}

HWND Container::handle() {
  return hwnd;
}

Container::Container() {
  background = 0;
}

/*****
 * Canvas
 *****/

long __stdcall canvas_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {

  case WM_PAINT: {
  Canvas *canvas = (Canvas*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if(canvas) { canvas->blit(); }
  } break;

  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Canvas::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  _w = width;
  _h = height;

  type = ControlType::Canvas;
  hwnd = CreateWindow("canvas_class", "", WS_CHILD | WS_VISIBLE,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

  pitch = width * sizeof(uint32);
  buffer = (uint32*)malloc(pitch * height);
  memset(buffer, 0, pitch * height);

  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth       = width;
  bmi.bmiHeader.biHeight      = -height; //use negative height to tell GDI not to flip bitmap vertically
  bmi.bmiHeader.biPlanes      = 1;
  bmi.bmiHeader.biBitCount    = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biSizeImage   = pitch * height;

  r_owner.attach(*this);
}

void Canvas::blit() {
PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  SetDIBitsToDevice(ps.hdc, 0, 0, _w, _h, 0, 0, 0, _h, (void*)buffer, &bmi, DIB_RGB_COLORS);
  EndPaint(hwnd, &ps);
}

void Canvas::redraw() {
  InvalidateRect(hwnd, 0, FALSE);
}

Canvas::Canvas() {
  buffer = 0;
}

Canvas::~Canvas() {
  safe_free(buffer);
}

/*****
 * Frame
 *****/

void Frame::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type = ControlType::Frame;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

/*****
 * Label
 *****/

long __stdcall label_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch(msg) {

  case WM_PAINT: {
  PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
  RECT rc;
  char t[4096];
    GetWindowText(hwnd, t, 4095);
    GetClientRect(hwnd, &rc);
    SetTextColor(ps.hdc, RGB(0, 0, 0));
    SetBkMode(ps.hdc, TRANSPARENT);
    SelectObject(ps.hdc, (HGDIOBJ)libui::font.variable);
    DrawText(ps.hdc, t, strlen(t), &rc, DT_END_ELLIPSIS | DT_NOPREFIX);
    EndPaint(hwnd, &ps);
  } break;

  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Label::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
string t;
  strcpy(t, caption ? caption : "");
  replace(t, "\r", "");
  replace(t, "\n", "\r\n");

  type = ControlType::Label;
  hwnd = CreateWindow("label_class", strptr(t), WS_CHILD | WS_VISIBLE,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

void Label::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string temp;
  vsprintf(temp, str, args);
  va_end(args);
  SetWindowText(hwnd, strptr(temp));
  InvalidateRect(hwnd, 0, TRUE);
}

/*****
 * Button
 *****/

void Button::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type = ControlType::Button;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

/*****
 * Checkbox
 *****/

void Checkbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type = ControlType::Checkbox;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

void Checkbox::check() {
  if(checked() == true)return;
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)TRUE, 0);
  owner->message(Message::Clicked, this);
}

void Checkbox::uncheck() {
  if(checked() == false)return;
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)FALSE, 0);
  owner->message(Message::Clicked, this);
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

void Radiobox::create(Window &r_owner, ControlGroup &list, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  if(list.count() == 0)throw;
  type = ControlType::Radiobox;
  group = list;
  hwnd = CreateWindow("BUTTON", caption ? caption : "", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
  if(this == &group[0])check();
}

void Radiobox::check() {
  if(checked() == true)return;
  for(uint i = 0; i < group.count(); i++) {
    SendMessage(group[i].hwnd, BM_SETCHECK, (group[i].hwnd == hwnd) ? (WPARAM)TRUE : (WPARAM)FALSE, 0);
  }
  owner->message(Message::Clicked, this);
}

bool Radiobox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

/*****
 * Editbox
 *****/

void Editbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *caption) {
  type = ControlType::Editbox;
  multiline = (style & Multiline);
  readonly  = (style & Readonly);
  vscroll   = false;
  hscroll   = false;

uint hscroll = (style & HorizontalScrollAlways) ? WS_HSCROLL :
               (style & HorizontalScrollNever)  ? 0          :
               ES_AUTOHSCROLL;
uint vscroll = (style & VerticalScrollAlways) ? WS_VSCROLL :
               (style & VerticalScrollNever)  ? 0          :
               ES_AUTOVSCROLL;

string data = caption;
  replace(data, "\r", "");
  replace(data, "\n", "\r\n");
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", strptr(data),
    WS_CHILD | WS_VISIBLE | vscroll | hscroll |
    (multiline == true ? ES_MULTILINE : 0) |
    (readonly == true ? ES_READONLY : 0),
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

void Editbox::set_text(const char *str, ...) {
va_list args;
  va_start(args, str);
string data;
  vsprintf(data, str, args);
  va_end(args);

  replace(data, "\r", "");
  replace(data, "\n", "\r\n");
  SetWindowText(hwnd, strptr(data));
}

uint Editbox::get_text(char *str, uint length) {
  GetWindowText(hwnd, str, length);
  return strlen(str);
}

/*****
 * Listbox
 *****/

void Listbox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, const char *columns) {
stringarray part;
  header = (style & Header);

uint hscroll = (style & HorizontalScrollAlways) ? WS_HSCROLL :
               (style & HorizontalScrollNever)  ? 0          :
               0;
uint vscroll = (style & VerticalScrollAlways) ? WS_VSCROLL :
               (style & VerticalScrollNever)  ? 0          :
               0;

  type = ControlType::Listbox;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
    WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | vscroll | hscroll |
    (header == true ? 0 : LVS_NOCOLUMNHEADER),
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
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

  autosize_columns();
  r_owner.attach(*this);
}

void Listbox::autosize_columns() {
  for(uint i = 0; i < column_count; i++) {
    ListView_SetColumnWidth(hwnd, i, LVSCW_AUTOSIZE_USEHEADER);
  }
}

void Listbox::set_column_width(uint column, uint width) {
  ListView_SetColumnWidth(hwnd, column, width);
}

void Listbox::add_item(const char *data, ...) {
va_list args;
  va_start(args, data);
string temp;
  vsprintf(temp, data, args);
  va_end(args);

stringarray part;
  split(part, "|", temp);

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

void Listbox::set_item(uint index, const char *data, ...) {
va_list args;
  va_start(args, data);
string temp;
  vsprintf(temp, data, args);
  va_end(args);

stringarray part;
  split(part, "|", temp);

  for(uint i = 0; i < count(part); i++) {
    ListView_SetItemText(hwnd, index, i, strptr(part[i]));
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

void Combobox::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type = ControlType::Combobox;
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", "",
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
    x, y, width, 200, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)libui::font.variable, 0);
  r_owner.attach(*this);
}

void Combobox::add_item(const char *data) {
  SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)data);
  if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) { set_selection(0); }
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

void Progressbar::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height) {
  type = ControlType::Progressbar;
  hwnd = CreateWindow(PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
  r_owner.attach(*this);
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

void Slider::create(Window &r_owner, uint style, uint x, uint y, uint width, uint height, uint range) {
  type = ControlType::Slider;
  orientation = (style & Vertical) ? 1 : 0;
  if(range < 1)range = 1;

  hwnd = CreateWindow(TRACKBAR_CLASS, "",
    WS_CHILD | WS_VISIBLE | TBS_NOTICKS | TBS_BOTH | (orientation == 0 ? TBS_HORZ : TBS_VERT),
    x, y, width, height, r_owner.info.hwnd, (HMENU)0, GetModuleHandle(0), 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, range - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(range >> 3));
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)0);
  r_owner.attach(*this);
}

void Slider::set_position(uint position) {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

uint Slider::get_position() {
  return SendMessage(hwnd, TBM_GETPOS, 0, 0);
}

};
