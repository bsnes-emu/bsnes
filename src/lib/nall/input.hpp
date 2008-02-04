#ifndef NALL_INPUT_HPP
#define NALL_INPUT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nall/static.hpp>
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

template<int number = -1> struct joypad {
  enum {
    none = joypad<number - 1>::limit,
    up, down, left, right,
    button_00, button_01, button_02, button_03,
    button_04, button_05, button_06, button_07,
    button_08, button_09, button_10, button_11,
    button_12, button_13, button_14, button_15,
    limit,
  };
};

template<> struct joypad<-1> {
  enum {
    none,
    up, down, left, right,
    button_00, button_01, button_02, button_03,
    button_04, button_05, button_06, button_07,
    button_08, button_09, button_10, button_11,
    button_12, button_13, button_14, button_15,
    length = button_15 - none + 1, //number of syms per joypad
    limit = keyboard::limit, //start joypad syms immediately after keyboard syms
  };

  static uint16_t index(int joypad_number, int joypad_enum) {
    if(joypad_number < 0 || joypad_number > 15) return keyboard::none;
    return limit + joypad_number * length + joypad_enum;
  }
};

enum { input_limit = joypad<15>::limit };

static static_assert<keyboard::limit < 256> keyboard_length_assert; //error if keyboard syms spill into joypad syms

static const char keyboard_table[][64] = {
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
  "limit",
};

static const char* input_find(uint16_t key) {
  if(key < keyboard::limit) return keyboard_table[key];
  static char buffer[64];
  for(uint16_t j = 0; j < 16; j++) {
    if(key == joypad<>::index(j, joypad<>::up)) { sprintf(buffer, "joypad%0.2d.up", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::down)) { sprintf(buffer, "joypad%0.2d.down", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::left)) { sprintf(buffer, "joypad%0.2d.left", j); return buffer; }
    if(key == joypad<>::index(j, joypad<>::right)) { sprintf(buffer, "joypad%0.2d.right", j); return buffer; }
    if(key >= joypad<>::index(j, joypad<>::button_00)
    && key <= joypad<>::index(j, joypad<>::button_15)) {
      sprintf(buffer, "joypad%0.2d.button_%0.2d", j, key - joypad<>::index(j, joypad<>::button_00));
      return buffer;
    }
  }
  return keyboard_table[0]; //"none"
}

static char* input_find(char *out, uint16_t key) {
  strcpy(out, input_find(key));
  return out;
}

static uint16_t input_find(const char *key) {
  for(uint16_t i = 0; i < keyboard::limit; i++) {
    if(!strcmp(keyboard_table[i], key)) return i;
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
  if(memcmp(key, ".button_", 8)) return keyboard::none;
  key += 8;
  if(!*key || !*(key + 1)) return keyboard::none;
  uint8_t button = (*key - '0') * 10 + (*(key + 1) - '0');
  if(button > 15) return keyboard::none;
  return joypad<>::index(j, joypad<>::button_00 + button);
}

} //namespace nall

#endif //ifndef NALL_INPUT_HPP
