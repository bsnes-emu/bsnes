void Input::ui_poll_input(Window *focus, bool require_window_focus) {
  if(require_window_focus == true) {
    if(GetFocus() != focus->hwnd)return;
  }

  poll();

EventInfo info;
  info.window    = focus;
  info.window_id = focus->id;
  info.control   = 0;

  for(int i = 0; i < 256; i++) {
    if(!old_keystate[i] && keystate[i]) {
      info.event_id   = EVENT_INPUTKEYDOWN;
      info.control_id = KEYFLAG | i;
      focus->Event(info);
    } else if(old_keystate[i] && !keystate[i]) {
      info.event_id   = EVENT_INPUTKEYUP;
      info.control_id = KEYFLAG | i;
      focus->Event(info);
    }
  }

  for(int j = 0; j < INPUT_JOYMAX; j++) {
    for(int i = 0; i < 256; i++) {
      if(!old_joystate[j * 256 + i] && joystate[j * 256 + i]) {
        info.event_id   = EVENT_INPUTKEYDOWN;
        info.control_id = JOYFLAG | (j << 8) | i;
        focus->Event(info);
      } else if(old_joystate[j * 256 + i] && !joystate[j * 256 + i]) {
        info.event_id   = EVENT_INPUTKEYUP;
        info.control_id = JOYFLAG | (j << 8) | i;
        focus->Event(info);
      }
    }
  }

  memcpy(old_keystate, keystate, 256);
  memcpy(old_joystate, joystate, INPUT_JOYMAX * 256);
}

void Input::ui_clear_input() {
  memset(keystate, 0, 256);
  memset(joystate, 0, INPUT_JOYMAX * 256);
  memset(old_keystate, 0, 256);
  memset(old_joystate, 0, INPUT_JOYMAX * 256);
}

//this does not poll actual key state, it relies on
//current keystate buffer to be up-to-date.
//it is intended to be used inside
//Input::EVENT_INPUTKEY* messages only.
//calling Input::ui_poll_input() first will allow
//this function to be used anywhere.
bool Input::keydown(uint id) {
  if(id < 256) {
    return (keystate[id] != 0);
  }

  return false;
}

//turn string into keymap value, eg
//"esc" returns keymap.esc
uint Input::stringtokeymap(const char *str) {
uint r = JOYKEY_NONE;
string t, part;
  strcpy(t, str);
  replace(t, ".", "|");
  split(part, "|", t);

#define cmp(__n) if(strmatch(#__n, part[0]) == true)r = (r & ~KEYMASK) | keymap.__n;
  cmp(esc)
  cmp(f1) cmp(f2) cmp(f3) cmp(f4) cmp(f5) cmp(f6)
  cmp(f7) cmp(f8) cmp(f9) cmp(f10) cmp(f11) cmp(f12)
  cmp(print_screen) cmp(scroll_lock) cmp(pause)

  cmp(tilde)
  cmp(num_0) cmp(num_1) cmp(num_2) cmp(num_3) cmp(num_4)
  cmp(num_5) cmp(num_6) cmp(num_7) cmp(num_8) cmp(num_9)
  cmp(minus) cmp(plus) cmp(backspace)
  cmp(ins) cmp(del) cmp(home) cmp(end) cmp(page_up) cmp(page_down)

  cmp(a) cmp(b) cmp(c) cmp(d) cmp(e) cmp(f) cmp(g) cmp(h) cmp(i)
  cmp(j) cmp(k) cmp(l) cmp(m) cmp(n) cmp(o) cmp(p) cmp(q) cmp(r)
  cmp(s) cmp(t) cmp(u) cmp(v) cmp(w) cmp(x) cmp(y) cmp(z)

  cmp(lbracket) cmp(rbracket) cmp(pipe)
  cmp(colon) cmp(quote)
  cmp(comma) cmp(period) cmp(question)

  cmp(numpad_0) cmp(numpad_1) cmp(numpad_2) cmp(numpad_3) cmp(numpad_4)
  cmp(numpad_5) cmp(numpad_6) cmp(numpad_7) cmp(numpad_8) cmp(numpad_9)
  cmp(numpad_plus) cmp(numpad_minus) cmp(numpad_mul) cmp(numpad_div)
  cmp(numpad_enter) cmp(numpad_point)

  cmp(numlock) cmp(capslock)

  cmp(up) cmp(down) cmp(left) cmp(right)

  cmp(tab) cmp(enter) cmp(space)
  cmp(lctrl) cmp(rctrl)
  cmp(lalt) cmp(ralt)
  cmp(lshift) cmp(rshift)

  cmp(lwin) cmp(rwin) cmp(menu)
#undef cmp

  if(count(part) < 3)return r;

  strcpy(t, part[1]);
  replace(t, "joy", "");
uint controller = strdec(t), button;

  strcpy(t, part[2]);
  if(strmatch(t, "up")) {
    button = 0x80;
  } else if(strmatch(t, "down")) {
    button = 0x81;
  } else if(strmatch(t, "left")) {
    button = 0x82;
  } else if(strmatch(t, "right")) {
    button = 0x83;
  } else {
    replace(t, "button", "");
    button = strdec(t);
  }

  r = (r & ~JOYMASK) | (controller << 16) | (button << 8);

  return r;
}

//turn keymap value into string,
//eg 0x01 returns "esc"
const char *Input::keymaptostring(uint key) {
static char result[128];
  strcpy(result, "keynone");

uint r = key & KEYMASK;

#define cmp(__n) if(r == keymap.__n)strcpy(result, #__n);
  cmp(esc)
  cmp(f1) cmp(f2) cmp(f3) cmp(f4) cmp(f5) cmp(f6)
  cmp(f7) cmp(f8) cmp(f9) cmp(f10) cmp(f11) cmp(f12)
  cmp(print_screen) cmp(scroll_lock) cmp(pause)

  cmp(tilde)
  cmp(num_0) cmp(num_1) cmp(num_2) cmp(num_3) cmp(num_4)
  cmp(num_5) cmp(num_6) cmp(num_7) cmp(num_8) cmp(num_9)
  cmp(minus) cmp(plus) cmp(backspace)
  cmp(ins) cmp(del) cmp(home) cmp(end) cmp(page_up) cmp(page_down)

  cmp(a) cmp(b) cmp(c) cmp(d) cmp(e) cmp(f) cmp(g) cmp(h) cmp(i)
  cmp(j) cmp(k) cmp(l) cmp(m) cmp(n) cmp(o) cmp(p) cmp(q) cmp(r)
  cmp(s) cmp(t) cmp(u) cmp(v) cmp(w) cmp(x) cmp(y) cmp(z)

  cmp(lbracket) cmp(rbracket) cmp(pipe)
  cmp(colon) cmp(quote)
  cmp(comma) cmp(period) cmp(question)

  cmp(numpad_0) cmp(numpad_1) cmp(numpad_2) cmp(numpad_3) cmp(numpad_4)
  cmp(numpad_5) cmp(numpad_6) cmp(numpad_7) cmp(numpad_8) cmp(numpad_9)
  cmp(numpad_plus) cmp(numpad_minus) cmp(numpad_mul) cmp(numpad_div)
  cmp(numpad_enter) cmp(numpad_point)

  cmp(numlock) cmp(capslock)

  cmp(up) cmp(down) cmp(left) cmp(right)

  cmp(tab) cmp(enter) cmp(space)
  cmp(lctrl) cmp(rctrl)
  cmp(lalt) cmp(ralt)
  cmp(lshift) cmp(rshift)

  cmp(lwin) cmp(rwin) cmp(menu)
#undef cmp

  if((key & JOYMASK) != JOY_NONE) {
    strcat(result, " | joy");
    r = (key >> 16) & 0xff;
  char tmp[16];
    sprintf(tmp, "%d", r);
    strcat(result, tmp);
    strcat(result, ".");
    r = (key >> 8) & 0xff;
    if(r == 0x80) {
      strcat(result, "up");
    } else if(r == 0x81) {
      strcat(result, "down");
    } else if(r == 0x82) {
      strcat(result, "left");
    } else if(r == 0x83) {
      strcat(result, "right");
    } else {
      sprintf(tmp, "button%d", r);
      strcat(result, tmp);
    }
  }

  return result;
}

void Input::init() {
string t, part;
  strcpy(t, config::input.joypad1.map.sget());
  replace(t, " ", "");
  replace(t, "\t", "");
  strlower(t);
  split(part, ";", t);
  config::input.joypad1.up     = stringtokeymap(strptr(part[ 0]));
  config::input.joypad1.down   = stringtokeymap(strptr(part[ 1]));
  config::input.joypad1.left   = stringtokeymap(strptr(part[ 2]));
  config::input.joypad1.right  = stringtokeymap(strptr(part[ 3]));
  config::input.joypad1.a      = stringtokeymap(strptr(part[ 4]));
  config::input.joypad1.b      = stringtokeymap(strptr(part[ 5]));
  config::input.joypad1.x      = stringtokeymap(strptr(part[ 6]));
  config::input.joypad1.y      = stringtokeymap(strptr(part[ 7]));
  config::input.joypad1.l      = stringtokeymap(strptr(part[ 8]));
  config::input.joypad1.r      = stringtokeymap(strptr(part[ 9]));
  config::input.joypad1.select = stringtokeymap(strptr(part[10]));
  config::input.joypad1.start  = stringtokeymap(strptr(part[11]));

  strcpy(t, config::input.joypad2.map.sget());
  replace(t, " ", "");
  replace(t, "\t", "");
  strlower(t);
  split(part, ";", t);
  config::input.joypad2.up     = stringtokeymap(strptr(part[ 0]));
  config::input.joypad2.down   = stringtokeymap(strptr(part[ 1]));
  config::input.joypad2.left   = stringtokeymap(strptr(part[ 2]));
  config::input.joypad2.right  = stringtokeymap(strptr(part[ 3]));
  config::input.joypad2.a      = stringtokeymap(strptr(part[ 4]));
  config::input.joypad2.b      = stringtokeymap(strptr(part[ 5]));
  config::input.joypad2.x      = stringtokeymap(strptr(part[ 6]));
  config::input.joypad2.y      = stringtokeymap(strptr(part[ 7]));
  config::input.joypad2.l      = stringtokeymap(strptr(part[ 8]));
  config::input.joypad2.r      = stringtokeymap(strptr(part[ 9]));
  config::input.joypad2.select = stringtokeymap(strptr(part[10]));
  config::input.joypad2.start  = stringtokeymap(strptr(part[11]));
}

void Input::term() {
string result;
char   t[256];
  strcpy(result, "");
  strcat(result, keymaptostring(config::input.joypad1.up));     strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.down));   strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.left));   strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.right));  strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.a));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.b));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.x));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.y));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.l));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.r));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.select)); strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad1.start));
  config::input.joypad1.map.sset(strptr(result));

  strcpy(result, "");
  strcat(result, keymaptostring(config::input.joypad2.up));     strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.down));   strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.left));   strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.right));  strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.a));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.b));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.x));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.y));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.l));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.r));      strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.select)); strcat(result, "; ");
  strcat(result, keymaptostring(config::input.joypad2.start));
  config::input.joypad2.map.sset(strptr(result));
}

Input::Input() {
  ui_clear_input();
}
