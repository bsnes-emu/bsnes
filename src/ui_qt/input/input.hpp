class InputManager {
public:
  void bind();
  void poll();
  void clear();
  void flush();

  int16_t state(uint16_t code);
  int16_t getStatus(unsigned deviceid, unsigned id);

  void refresh();
  function<void (uint16_t)> onInput;

  InputManager();

private:
  bool activeState;
  int16_t stateTable[2][nall::input_limit];
} inputManager;

struct InputCode {
  enum type_t {
    KeyboardButton,
    MouseAxis,
    MouseButton,
    JoypadAxis,
    JoypadButton,
    Unknown,
  };

  static type_t type(uint16_t code);
  static bool isButton(uint16_t code);
  static bool isAxis(uint16_t code);
};

struct InputGroup;

struct InputObject {
  InputGroup *parent;
  enum type_t { Button, Axis } type;
  const char *name;
  string &id;
  uint16_t code;
  int16_t state;

  void bind();
  InputObject(type_t t, const char *n, string &s);
};

struct InputGroup : public array<InputObject*> {
  const char *name;

  void attach(InputObject &object);
  void bind();
  void clear();
  void poll(const int16_t *table);
  virtual int16_t state(unsigned index) const;
  InputGroup(const char *n);

private:
  array<InputObject*> &list;
};

#include "device.hpp"
#include "userinterface.hpp"
