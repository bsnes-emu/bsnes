bool8 old_keystate[4096];

void ui_poll_input(Window *focus, bool require_window_focus = true) {
  if(require_window_focus == true) {
    if(GetFocus() != focus->hwnd)return;
  }

  uiInput->poll_hw();

bool8 *keystate = uiInput->keystate;
EventInfo info;
  info.window    = focus;
  info.window_id = focus->id;
  info.control   = 0;

  for(int i = 0; i < 4096; i++) {
    if(!old_keystate[i] && keystate[i]) {
      info.event_id   = EVENT_INPUTKEYDOWN;
      info.control_id = i;
      focus->Event(info);
    } else if(old_keystate[i] && !keystate[i]) {
      info.event_id   = EVENT_INPUTKEYUP;
      info.control_id = i;
      focus->Event(info);
    }
  }

  memcpy(old_keystate, keystate, sizeof(keystate));
}
