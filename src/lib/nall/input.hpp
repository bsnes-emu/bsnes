#ifndef NALL_INPUT_HPP
#define NALL_INPUT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nall/stdint.hpp>

namespace nall {

struct keyboard {
  enum {
    none,
    escape, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
    print_screen, scroll_lock, pause, tilde,
    num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0,
    dash, equal, backspace,
    insert, delete_, home, end, page_up, page_down,
    a, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z,
    lbracket, rbracket, backslash, semicolon, apostrophe, comma, period, slash,
    pad_1, pad_2, pad_3, pad_4, pad_5, pad_6, pad_7, pad_8, pad_9, pad_0,
    point, enter, add, subtract, multiply, divide,
    num_lock, caps_lock,
    up, down, left, right,
    tab, return_, spacebar,
    lctrl, rctrl, lalt, ralt, lshift, rshift, lsuper, rsuper, menu,
    limit,
  };
};

struct mouse {
  enum { buttons = 8 };

  enum {
    none = keyboard::limit,
    x, y, z,
    button,
    limit = button + buttons,
  };
};

template<int number = -1> struct joypad {
  enum { axes = 8 };
  enum { buttons = 96 };

  enum {
    none = joypad<number - 1>::limit,
    up, down, left, right,
    axis,
    button = axis + axes,
    limit = button + buttons,
  };
};

template<> struct joypad<-1> {
  enum { count = 16 };
  enum { axes = 8 };
  enum { buttons = 96 };

  enum {
    none,
    up, down, left, right,
    axis,
    button = axis + axes,
    length = button + buttons - none,  //number of syms per joypad
    limit = mouse::limit,
  };

  static uint16_t index(unsigned joypad_number, unsigned joypad_enum) {
    if(joypad_number >= count) return keyboard::none;
    return limit + joypad_number * length + joypad_enum;
  }
};

enum { input_limit = joypad<joypad<>::count - 1>::limit };

static const char sym_table[][64] = {
  //keyboard
  "none",
  "escape", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12",
  "print_screen", "scroll_lock", "pause", "tilde",
  "num_1", "num_2", "num_3", "num_4", "num_5", "num_6", "num_7", "num_8", "num_9", "num_0",
  "dash", "equal", "backspace",
  "insert", "delete", "home", "end", "page_up", "page_down",
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  "lbracket", "rbracket", "backslash", "semicolon", "apostrophe", "comma", "period", "slash",
  "pad_1", "pad_2", "pad_3", "pad_4", "pad_5", "pad_6", "pad_7", "pad_8", "pad_9", "pad_0",
  "point", "enter", "add", "subtract", "multiply", "divide",
  "num_lock", "caps_lock",
  "up", "down", "left", "right",
  "tab", "return", "spacebar",
  "lctrl", "rctrl", "lalt", "ralt", "lshift", "rshift", "lsuper", "rsuper", "menu",
  "keyboard.limit",

  //mouse
  "mouse.x", "mouse.y", "mouse.z",
  "mouse.button00", "mouse.button01", "mouse.button02", "mouse.button03",
  "mouse.button04", "mouse.button05", "mouse.button06", "mouse.button07",
  "mouse.limit",
};

static const char* input_find(uint16_t key) {
  if(key < mouse::limit) return sym_table[key];

  static char buffer[64];
  for(unsigned j = 0; j < 16; j++) {
    if(key == joypad<>::index(j, joypad<>::up)) { sprintf(buffer, "joypad%.2d.up", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::down)) { sprintf(buffer, "joypad%.2d.down", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::left)) { sprintf(buffer, "joypad%.2d.left", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::right)) { sprintf(buffer, "joypad%.2d.right", j); return buffer; }

    if(key >= joypad<>::index(j, joypad<>::axis + 0)
    && key <  joypad<>::index(j, joypad<>::axis + joypad<>::axes)) {
      sprintf(buffer, "joypad%.2d.axis%.2d", j, key - joypad<>::index(j, joypad<>::axis));
      return buffer;
    }

    if(key >= joypad<>::index(j, joypad<>::button + 0)
    && key <  joypad<>::index(j, joypad<>::button + joypad<>::buttons)) {
      sprintf(buffer, "joypad%.2d.button%.2d", j, key - joypad<>::index(j, joypad<>::button));
      return buffer;
    }
  }

  return "none";
}

static char* input_find(char *out, uint16_t key) {
  strcpy(out, input_find(key));
  return out;
}

static uint16_t input_find(const char *key) {
  for(unsigned i = 0; i < mouse::limit; i++) {
    if(!strcmp(sym_table[i], key)) return i;
  }

  if(memcmp(key, "joypad", 6)) return keyboard::none;
  key += 6;
  if(!*key || !*(key + 1)) return keyboard::none;
  uint8_t j = (*key - '0') * 10 + (*(key + 1) - '0');
  if(j > 15) return keyboard::none;

  key += 2;
  if(!strcmp(key, ".up")) return joypad<>::index(j, joypad<>::up);
  if(!strcmp(key, ".down")) return joypad<>::index(j, joypad<>::down);
  if(!strcmp(key, ".left")) return joypad<>::index(j, joypad<>::left);
  if(!strcmp(key, ".right")) return joypad<>::index(j, joypad<>::right);

  if(!memcmp(key, ".axis", 5)) {
    key += 5;
    if(!*key || !*(key + 1)) return keyboard::none;
    uint8_t axis = (*key - '0') * 10 + (*(key + 1) - '0');
    if(axis >= joypad<>::axes) return keyboard::none;
    return joypad<>::index(j, joypad<>::axis + axis);
  }

  if(!memcmp(key, ".button", 7)) {
    key += 7;
    if(!*key || !*(key + 1)) return keyboard::none;
    uint8_t button = (*key - '0') * 10 + (*(key + 1) - '0');
    if(button >= joypad<>::buttons) return keyboard::none;
    return joypad<>::index(j, joypad<>::button + button);
  }

  return keyboard::none;
}

} //namespace nall

#endif //ifndef NALL_INPUT_HPP
