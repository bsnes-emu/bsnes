void Input::poll(uint8 device) {
  poll();

#define poll_key(__key) \
  __key = bool(keystate[(uint(config::input.__key) >>  0) & 4095]) | \
          bool(keystate[(uint(config::input.__key) >> 16) & 4095])

  switch(device) {
  case SNES::DEV_JOYPAD1:
    poll_key(joypad1.up);
    poll_key(joypad1.down);
    poll_key(joypad1.left);
    poll_key(joypad1.right);
    poll_key(joypad1.a);
    poll_key(joypad1.b);
    poll_key(joypad1.x);
    poll_key(joypad1.y);
    poll_key(joypad1.l);
    poll_key(joypad1.r);
    poll_key(joypad1.select);
    poll_key(joypad1.start);
    break;
  case SNES::DEV_JOYPAD2:
    poll_key(joypad2.up);
    poll_key(joypad2.down);
    poll_key(joypad2.left);
    poll_key(joypad2.right);
    poll_key(joypad2.a);
    poll_key(joypad2.b);
    poll_key(joypad2.x);
    poll_key(joypad2.y);
    poll_key(joypad2.l);
    poll_key(joypad2.r);
    poll_key(joypad2.select);
    poll_key(joypad2.start);
    break;
  }

#undef poll_key
}

bool Input::get_status(uint8 device, uint8 button) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     return joypad1.up;
    case SNES::JOYPAD_DOWN:   return joypad1.down;
    case SNES::JOYPAD_LEFT:   return joypad1.left;
    case SNES::JOYPAD_RIGHT:  return joypad1.right;
    case SNES::JOYPAD_A:      return joypad1.a;
    case SNES::JOYPAD_B:      return joypad1.b;
    case SNES::JOYPAD_X:      return joypad1.x;
    case SNES::JOYPAD_Y:      return joypad1.y;
    case SNES::JOYPAD_L:      return joypad1.l;
    case SNES::JOYPAD_R:      return joypad1.r;
    case SNES::JOYPAD_SELECT: return joypad1.select;
    case SNES::JOYPAD_START:  return joypad1.start;
    }
    break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     return joypad2.up;
    case SNES::JOYPAD_DOWN:   return joypad2.down;
    case SNES::JOYPAD_LEFT:   return joypad2.left;
    case SNES::JOYPAD_RIGHT:  return joypad2.right;
    case SNES::JOYPAD_A:      return joypad2.a;
    case SNES::JOYPAD_B:      return joypad2.b;
    case SNES::JOYPAD_X:      return joypad2.x;
    case SNES::JOYPAD_Y:      return joypad2.y;
    case SNES::JOYPAD_L:      return joypad2.l;
    case SNES::JOYPAD_R:      return joypad2.r;
    case SNES::JOYPAD_SELECT: return joypad2.select;
    case SNES::JOYPAD_START:  return joypad2.start;
    }
    break;
  }

  return false;
}

void Input::set_status(uint8 device, uint8 button, bool status) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     joypad1.up     = status; break;
    case SNES::JOYPAD_DOWN:   joypad1.down   = status; break;
    case SNES::JOYPAD_LEFT:   joypad1.left   = status; break;
    case SNES::JOYPAD_RIGHT:  joypad1.right  = status; break;
    case SNES::JOYPAD_A:      joypad1.a      = status; break;
    case SNES::JOYPAD_B:      joypad1.b      = status; break;
    case SNES::JOYPAD_X:      joypad1.x      = status; break;
    case SNES::JOYPAD_Y:      joypad1.y      = status; break;
    case SNES::JOYPAD_L:      joypad1.l      = status; break;
    case SNES::JOYPAD_R:      joypad1.r      = status; break;
    case SNES::JOYPAD_SELECT: joypad1.select = status; break;
    case SNES::JOYPAD_START:  joypad1.start  = status; break;
    } break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     joypad2.up     = status; break;
    case SNES::JOYPAD_DOWN:   joypad2.down   = status; break;
    case SNES::JOYPAD_LEFT:   joypad2.left   = status; break;
    case SNES::JOYPAD_RIGHT:  joypad2.right  = status; break;
    case SNES::JOYPAD_A:      joypad2.a      = status; break;
    case SNES::JOYPAD_B:      joypad2.b      = status; break;
    case SNES::JOYPAD_X:      joypad2.x      = status; break;
    case SNES::JOYPAD_Y:      joypad2.y      = status; break;
    case SNES::JOYPAD_L:      joypad2.l      = status; break;
    case SNES::JOYPAD_R:      joypad2.r      = status; break;
    case SNES::JOYPAD_SELECT: joypad2.select = status; break;
    case SNES::JOYPAD_START:  joypad2.start  = status; break;
    } break;
  }
}

void Input::clear_input() {
  joypad1.up     = joypad2.up     =
  joypad1.down   = joypad2.down   =
  joypad1.left   = joypad2.left   =
  joypad1.right  = joypad2.right  =
  joypad1.a      = joypad2.a      =
  joypad1.b      = joypad2.b      =
  joypad1.x      = joypad2.x      =
  joypad1.y      = joypad2.y      =
  joypad1.l      = joypad2.l      =
  joypad1.r      = joypad2.r      =
  joypad1.select = joypad2.select =
  joypad1.start  = joypad2.start  = false;
}

//this does not poll actual key state, it relies on
//current keystate buffer to be up-to-date.
bool Input::keydown(uint16 key) {
  return (keystate[key] != 0);
}

uint Input::get_key(uint device, uint button) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     return config::input.joypad1.up;
    case SNES::JOYPAD_DOWN:   return config::input.joypad1.down;
    case SNES::JOYPAD_LEFT:   return config::input.joypad1.left;
    case SNES::JOYPAD_RIGHT:  return config::input.joypad1.right;
    case SNES::JOYPAD_A:      return config::input.joypad1.a;
    case SNES::JOYPAD_B:      return config::input.joypad1.b;
    case SNES::JOYPAD_X:      return config::input.joypad1.x;
    case SNES::JOYPAD_Y:      return config::input.joypad1.y;
    case SNES::JOYPAD_L:      return config::input.joypad1.l;
    case SNES::JOYPAD_R:      return config::input.joypad1.r;
    case SNES::JOYPAD_SELECT: return config::input.joypad1.select;
    case SNES::JOYPAD_START:  return config::input.joypad1.start;
    } break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     return config::input.joypad2.up;
    case SNES::JOYPAD_DOWN:   return config::input.joypad2.down;
    case SNES::JOYPAD_LEFT:   return config::input.joypad2.left;
    case SNES::JOYPAD_RIGHT:  return config::input.joypad2.right;
    case SNES::JOYPAD_A:      return config::input.joypad2.a;
    case SNES::JOYPAD_B:      return config::input.joypad2.b;
    case SNES::JOYPAD_X:      return config::input.joypad2.x;
    case SNES::JOYPAD_Y:      return config::input.joypad2.y;
    case SNES::JOYPAD_L:      return config::input.joypad2.l;
    case SNES::JOYPAD_R:      return config::input.joypad2.r;
    case SNES::JOYPAD_SELECT: return config::input.joypad2.select;
    case SNES::JOYPAD_START:  return config::input.joypad2.start;
    } break;
  }

  return 0;
}

void Input::set_key(uint device, uint button, uint key) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     config::input.joypad1.up     = key; break;
    case SNES::JOYPAD_DOWN:   config::input.joypad1.down   = key; break;
    case SNES::JOYPAD_LEFT:   config::input.joypad1.left   = key; break;
    case SNES::JOYPAD_RIGHT:  config::input.joypad1.right  = key; break;
    case SNES::JOYPAD_A:      config::input.joypad1.a      = key; break;
    case SNES::JOYPAD_B:      config::input.joypad1.b      = key; break;
    case SNES::JOYPAD_X:      config::input.joypad1.x      = key; break;
    case SNES::JOYPAD_Y:      config::input.joypad1.y      = key; break;
    case SNES::JOYPAD_L:      config::input.joypad1.l      = key; break;
    case SNES::JOYPAD_R:      config::input.joypad1.r      = key; break;
    case SNES::JOYPAD_SELECT: config::input.joypad1.select = key; break;
    case SNES::JOYPAD_START:  config::input.joypad1.start  = key; break;
    } break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     config::input.joypad2.up     = key; break;
    case SNES::JOYPAD_DOWN:   config::input.joypad2.down   = key; break;
    case SNES::JOYPAD_LEFT:   config::input.joypad2.left   = key; break;
    case SNES::JOYPAD_RIGHT:  config::input.joypad2.right  = key; break;
    case SNES::JOYPAD_A:      config::input.joypad2.a      = key; break;
    case SNES::JOYPAD_B:      config::input.joypad2.b      = key; break;
    case SNES::JOYPAD_X:      config::input.joypad2.x      = key; break;
    case SNES::JOYPAD_Y:      config::input.joypad2.y      = key; break;
    case SNES::JOYPAD_L:      config::input.joypad2.l      = key; break;
    case SNES::JOYPAD_R:      config::input.joypad2.r      = key; break;
    case SNES::JOYPAD_SELECT: config::input.joypad2.select = key; break;
    case SNES::JOYPAD_START:  config::input.joypad2.start  = key; break;
    } break;
  }
}

uint Input::decode(substring &str) {
string p;
  split(p, "|", str);
uint r = key.find(strptr(p[0])) & 4095;
  if(count(p) >= 2) { r |= (key.find(strptr(p[1])) & 4095) << 16; }
  return r;
}

void Input::init() {
string t, part;
  strcpy(t, config::input.joypad1.map.sget());
  replace(t, " ", "");
  replace(t, "\t", "");
  strlower(t);
  split(part, ";", t);
  config::input.joypad1.up     = decode(part[ 0]);
  config::input.joypad1.down   = decode(part[ 1]);
  config::input.joypad1.left   = decode(part[ 2]);
  config::input.joypad1.right  = decode(part[ 3]);
  config::input.joypad1.a      = decode(part[ 4]);
  config::input.joypad1.b      = decode(part[ 5]);
  config::input.joypad1.x      = decode(part[ 6]);
  config::input.joypad1.y      = decode(part[ 7]);
  config::input.joypad1.l      = decode(part[ 8]);
  config::input.joypad1.r      = decode(part[ 9]);
  config::input.joypad1.select = decode(part[10]);
  config::input.joypad1.start  = decode(part[11]);

  strcpy(t, config::input.joypad2.map.sget());
  replace(t, " ", "");
  replace(t, "\t", "");
  strlower(t);
  split(part, ";", t);
  config::input.joypad2.up     = decode(part[ 0]);
  config::input.joypad2.down   = decode(part[ 1]);
  config::input.joypad2.left   = decode(part[ 2]);
  config::input.joypad2.right  = decode(part[ 3]);
  config::input.joypad2.a      = decode(part[ 4]);
  config::input.joypad2.b      = decode(part[ 5]);
  config::input.joypad2.x      = decode(part[ 6]);
  config::input.joypad2.y      = decode(part[ 7]);
  config::input.joypad2.l      = decode(part[ 8]);
  config::input.joypad2.r      = decode(part[ 9]);
  config::input.joypad2.select = decode(part[10]);
  config::input.joypad2.start  = decode(part[11]);
}

const char *Input::encode(uint k) {
  strcpy(tmp, key.find(k & 4095));
  if(k >> 16) {
    strcat(tmp, " | ");
    strcat(tmp, key.find((k >> 16) & 4095));
  }
  return tmp;
}

void Input::term() {
string result;
char   t[256];
  strcpy(result, "");
  strcat(result, encode((uint)config::input.joypad1.up));     strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.down));   strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.left));   strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.right));  strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.a));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.b));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.x));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.y));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.l));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.r));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.select)); strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad1.start));
  config::input.joypad1.map.sset(strptr(result));

  strcpy(result, "");
  strcat(result, encode((uint)config::input.joypad2.up));     strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.down));   strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.left));   strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.right));  strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.a));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.b));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.x));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.y));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.l));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.r));      strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.select)); strcat(result, "; ");
  strcat(result, encode((uint)config::input.joypad2.start));
  config::input.joypad2.map.sset(strptr(result));
}

Input::Input() {
  memset(keystate, 0, sizeof(keystate));
  clear_input();
}
