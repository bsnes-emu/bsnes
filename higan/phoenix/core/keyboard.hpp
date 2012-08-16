//each code refers to a physical key
//names are taken assuming: NumLock on, CapsLock off, Shift off
//layout uses US-104 keyboard
enum class Scancode : unsigned {
  None,

  Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  PrintScreen, ScrollLock, Pause,
  Insert, Delete, Home, End, PageUp, PageDown,
  Up, Down, Left, Right,

  Grave, Number1, Number2, Number3, Number4, Number5, Number6, Number7, Number8, Number9, Number0, Minus, Equal, Backspace,
  BracketLeft, BracketRight, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
  Tab, CapsLock, Return, ShiftLeft, ShiftRight, ControlLeft, ControlRight, SuperLeft, SuperRight, AltLeft, AltRight, Space, Menu,
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

  NumLock, Divide, Multiply, Subtract, Add, Enter, Point,
  Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, Keypad0,

  Limit,
};

//each enum refers to a translated scancode (eg Shift+1 = !)
enum class Keycode : unsigned {
  None,

  Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  PrintScreen, SysRq, ScrollLock, Pause, Break,
  Insert, Delete, Home, End, PageUp, PageDown,
  Up, Down, Left, Right,

  Grave, Number1, Number2, Number3, Number4, Number5, Number6, Number7, Number8, Number9, Number0, Minus, Equal, Backspace,
  Tilde, Exclamation, At, Pound, Dollar, Percent, Power, Ampersand, Asterisk, ParenthesisLeft, ParenthesisRight, Underscore, Plus,
  BracketLeft, BracketRight, Backslash, Semicolon, Apostrophe, Comma, Period, Slash,
  BraceLeft, BraceRight, Pipe, Colon, Quote, CaretLeft, CaretRight, Question,
  Tab, CapsLock, Return, ShiftLeft, ShiftRight, ControlLeft, ControlRight, SuperLeft, SuperRight, AltLeft, AltRight, Space, Menu,
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,

  NumLock, Divide, Multiply, Subtract, Add, Enter, Point,
  Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, Keypad0,
  KeypadInsert, KeypadDelete, KeypadHome, KeypadEnd, KeypadPageUp, KeypadPageDown, KeypadUp, KeypadDown, KeypadLeft, KeypadRight, KeypadCenter,

  Limit,
};
