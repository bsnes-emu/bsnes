struct AbstractInput {
  enum class Type : unsigned { Button, MouseButton, MouseAxis, HatUp, HatDown, HatLeft, HatRight, Axis, AxisLo, AxisHi } type;
  string name;
  string mapping;
  unsigned scancode;

  virtual void attach(const string &primaryName, const string &secondaryName, const string &tertiaryName);
  virtual void bind();
  virtual bool bind(int16_t scancode, int16_t value) = 0;
  virtual int16_t poll();
};

struct AnalogInput : AbstractInput {
  bool bind(int16_t scancode, int16_t value);
  int16_t poll();
};

struct DigitalInput : AbstractInput {
  bool bind(int16_t scancode, int16_t value);
  int16_t poll();
};

struct TurboInput : DigitalInput {
  unsigned phase;

  int16_t poll();
  TurboInput();
};

struct TertiaryInput : array<AbstractInput&> {
  string name;

  virtual void attach(const string &primaryName, const string &secondaryName);
  virtual void bind();
  virtual int16_t poll(unsigned n);
};

struct SecondaryInput : array<TertiaryInput&> {
  string name;

  virtual void attach(const string &primaryName);
  virtual void bind();
};

struct PrimaryInput : array<SecondaryInput&> {
  string name;

  virtual void attach();
  virtual void bind();
};

#include "nes.hpp"
#include "snes.hpp"
#include "gameboy.hpp"
#include "gba.hpp"
#include "user-interface.hpp"

struct InputManager {
  configuration config;
  int16_t scancode[2][Scancode::Limit];
  bool activeScancode;

  array<PrimaryInput&> inputList;
  NesInput nes;
  SnesInput snes;
  GameBoyInput gameBoy;
  GbaInput gba;
  UserInterfaceInput userInterface;

  void scan();
  InputManager();
  ~InputManager();
};

extern InputManager *inputManager;
