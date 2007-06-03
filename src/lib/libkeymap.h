/*
  libkeymap : version 0.02 ~byuu (2007-05-28)
*/

#ifndef LIBKEYMAP_H
#define LIBKEYMAP_H

namespace keymap {

enum {
  none = 0x0000,

  esc,

  f1,
  f2,
  f3,
  f4,
  f5,
  f6,
  f7,
  f8,
  f9,
  f10,
  f11,
  f12,

  print_screen, sys_req,
  scroll_lock,
  pause, brk,

  grave, tilde,

  num_1, exclamation,
  num_2, at,
  num_3, pound,
  num_4, dollar,
  num_5, percent,
  num_6, power,
  num_7, ampersand,
  num_8, asterisk,
  num_9, lparenthesis,
  num_0, rparenthesis,

  minus, underscore,
  equal, plus,
  backspace,

  ins,
  del,
  home,
  end,
  page_up,
  page_down,

  a, b, c, d, e, f, g, h, i, j, k, l, m,
  n, o, p, q, r, s, t, u, v, w, x, y, z,

  A, B, C, D, E, F, G, H, I, J, K, L, M,
  N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

  lbracket,   lbrace,
  rbracket,   rbrace,
  backslash,  pipe,
  semicolon,  colon,
  apostrophe, quote,
  comma,      lcaret,
  period,     rcaret,
  slash,      question,

  kp_1, kp_end,
  kp_2, kp_down,
  kp_3, kp_page_down,
  kp_4, kp_left,
  kp_5, kp_center,
  kp_6, kp_right,
  kp_7, kp_home,
  kp_8, kp_up,
  kp_9, kp_page_up,
  kp_0, kp_insert,
  kp_decimal, kp_delete,

  kp_plus,
  kp_minus,
  kp_mul,
  kp_div,
  kp_enter,

  num_lock,
  caps_lock,

  up,
  down,
  left,
  right,

  tab,
  enter,
  space,

  lctrl,
  rctrl,
  lalt,
  ralt,
  lshift,
  rshift,
  lsuper,
  rsuper,
  menu,

  joypad_flag    = 0x8000,
  joypad_nummask = 0x7f00,
  joypad_keymask = 0x00ff,

  joypad_up      = 0x0080,
  joypad_down    = 0x0081,
  joypad_left    = 0x0082,
  joypad_right   = 0x0083,
};

static uint16 find(const char *key) {
#define match(_n) if(!strcmp(#_n, key)) { return _n; }
  match(none)
  match(esc)
  match(f1) match(f2) match(f3) match(f4) match(f5) match(f6)
  match(f7) match(f8) match(f9) match(f10) match(f11) match(f12)
  match(print_screen) match(sys_req) match(scroll_lock) match(pause) match(brk)
  match(grave) match(tilde)
  match(num_1) match(exclamation)
  match(num_2) match(at)
  match(num_3) match(pound)
  match(num_4) match(dollar)
  match(num_5) match(percent)
  match(num_6) match(power)
  match(num_7) match(ampersand)
  match(num_8) match(asterisk)
  match(num_9) match(lparenthesis)
  match(num_0) match(rparenthesis)
  match(minus) match(underscore) match(equal) match(plus) match(backspace)
  match(ins) match(del) match(home) match(end) match(page_up) match(page_down)
  match(a) match(b) match(c) match(d) match(e) match(f)
  match(g) match(h) match(i) match(j) match(k) match(l)
  match(m) match(n) match(o) match(p) match(q) match(r)
  match(s) match(t) match(u) match(v) match(w) match(x)
  match(y) match(z)
  match(A) match(B) match(C) match(D) match(E) match(F)
  match(G) match(H) match(I) match(J) match(K) match(L)
  match(M) match(N) match(O) match(P) match(Q) match(R)
  match(S) match(T) match(U) match(V) match(W) match(X)
  match(Y) match(Z)
  match(lbracket) match(lbrace)
  match(rbracket) match(rbrace)
  match(backslash) match(pipe)
  match(semicolon) match(colon)
  match(apostrophe) match(quote)
  match(comma) match(lcaret)
  match(period) match(rcaret)
  match(slash) match(question)
  match(kp_1) match(kp_end)
  match(kp_2) match(kp_down)
  match(kp_3) match(kp_page_down)
  match(kp_4) match(kp_left)
  match(kp_5) match(kp_center)
  match(kp_6) match(kp_right)
  match(kp_7) match(kp_home)
  match(kp_8) match(kp_up)
  match(kp_9) match(kp_page_up)
  match(kp_0) match(kp_insert)
  match(kp_decimal) match(kp_delete)
  match(kp_plus) match(kp_minus) match(kp_mul) match(kp_div) match(kp_enter)
  match(num_lock) match(caps_lock)
  match(up) match(down) match(left) match(right)
  match(tab) match(enter) match(space)
  match(lctrl) match(rctrl)
  match(lalt) match(ralt)
  match(lshift) match(rshift)
  match(lsuper) match(rsuper)
  match(menu)
#undef match

  if(!memcmp(key, "joypad", 6)) {
  const char *p = key + 6;
  uint joypad, button, n;
    sscanf(p, "%u%n", &joypad, &n);
    joypad &= 127;
    p += n;
    if(!strcmp(p, ".up"))    { return joypad_flag | (joypad << 8) | joypad_up;    }
    if(!strcmp(p, ".down"))  { return joypad_flag | (joypad << 8) | joypad_down;  }
    if(!strcmp(p, ".left"))  { return joypad_flag | (joypad << 8) | joypad_left;  }
    if(!strcmp(p, ".right")) { return joypad_flag | (joypad << 8) | joypad_right; }
    if(!memcmp(p, ".button", 7)) {
      sscanf(p + 7, "%u", &button);
      button &= 127;
      return joypad_flag | (joypad << 8) | button;
    }
  }

  return none;
}

static char *find(char *out, uint16 key) {
#define match(_n) if(_n == key) { strcpy(out, #_n); return out; }
  match(none)
  match(esc)
  match(f1) match(f2) match(f3) match(f4) match(f5) match(f6)
  match(f7) match(f8) match(f9) match(f10) match(f11) match(f12)
  match(print_screen) match(sys_req) match(scroll_lock) match(pause) match(brk)
  match(grave) match(tilde)
  match(num_1) match(exclamation)
  match(num_2) match(at)
  match(num_3) match(pound)
  match(num_4) match(dollar)
  match(num_5) match(percent)
  match(num_6) match(power)
  match(num_7) match(ampersand)
  match(num_8) match(asterisk)
  match(num_9) match(lparenthesis)
  match(num_0) match(rparenthesis)
  match(minus) match(underscore) match(equal) match(plus) match(backspace)
  match(ins) match(del) match(home) match(end) match(page_up) match(page_down)
  match(a) match(b) match(c) match(d) match(e) match(f)
  match(g) match(h) match(i) match(j) match(k) match(l)
  match(m) match(n) match(o) match(p) match(q) match(r)
  match(s) match(t) match(u) match(v) match(w) match(x)
  match(y) match(z)
  match(A) match(B) match(C) match(D) match(E) match(F)
  match(G) match(H) match(I) match(J) match(K) match(L)
  match(M) match(N) match(O) match(P) match(Q) match(R)
  match(S) match(T) match(U) match(V) match(W) match(X)
  match(Y) match(Z)
  match(lbracket) match(lbrace)
  match(rbracket) match(rbrace)
  match(backslash) match(pipe)
  match(semicolon) match(colon)
  match(apostrophe) match(quote)
  match(comma) match(lcaret)
  match(period) match(rcaret)
  match(slash) match(question)
  match(kp_1) match(kp_end)
  match(kp_2) match(kp_down)
  match(kp_3) match(kp_page_down)
  match(kp_4) match(kp_left)
  match(kp_5) match(kp_center)
  match(kp_6) match(kp_right)
  match(kp_7) match(kp_home)
  match(kp_8) match(kp_up)
  match(kp_9) match(kp_page_up)
  match(kp_0) match(kp_insert)
  match(kp_decimal) match(kp_delete)
  match(kp_plus) match(kp_minus) match(kp_mul) match(kp_div) match(kp_enter)
  match(num_lock) match(caps_lock)
  match(up) match(down) match(left) match(right)
  match(tab) match(enter) match(space)
  match(lctrl) match(rctrl)
  match(lalt) match(ralt)
  match(lshift) match(rshift)
  match(lsuper) match(rsuper)
  match(menu)
#undef match

  if(key & joypad_flag) {
  uint joypad = (key & joypad_nummask) >> 8;
  uint button = (key & joypad_keymask);
    if(button == joypad_up)    { sprintf(out, "joypad%d.up",    joypad); return out; }
    if(button == joypad_down)  { sprintf(out, "joypad%d.down",  joypad); return out; }
    if(button == joypad_left)  { sprintf(out, "joypad%d.left",  joypad); return out; }
    if(button == joypad_right) { sprintf(out, "joypad%d.right", joypad); return out; }
    sprintf(out, "joypad%d.button%d", joypad, button & 127);
    return out;
  }

  strcpy(out, "none");
  return out;
}

static char find_t[256];

static const char *find(uint16 key) {
  find(find_t, key);
  return find_t;
}

};

#endif
