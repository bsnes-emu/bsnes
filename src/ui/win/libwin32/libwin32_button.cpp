bool Button::Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text) {
  if(!parent_window)return false;

  parent = parent_window;
  id     = CONTROLID_INDEX + parent->control_count;
  type   = BUTTON;
  state.ws     = WS_CHILD;
  state.es     = 0;
  state.x      = x;
  state.y      = y;
  state.width  = width;
  state.height = height;

stringarray part;
  ParseStyleParam(style, part);
  for(int i = 0; i < count(part); i++) {
    if(!strcmp(part[i], "visible"))state.ws |= WS_VISIBLE;
    if(!strcmp(part[i], "disabled"))state.ws |= WS_DISABLED;

    if(!strcmp(part[i], "left"))state.ws |= BS_LEFT;
    if(!strcmp(part[i], "center"))state.ws |= BS_CENTER;
    if(!strcmp(part[i], "right"))state.ws |= BS_RIGHT;
  }

  hwnd = CreateWindowEx(state.es, "BUTTON", text, state.ws,
    state.x, state.y, state.width, state.height,
    parent->hwnd, (HMENU)id, GetModuleHandle(0), 0);
  if(!hwnd)return false;

  PostCreate();
  return true;
}
