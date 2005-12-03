class Input {
public:
//used by GUI for controller configuration
  virtual uint32 poll() = 0;

  virtual void   poll(uint8 type) = 0;
  virtual bool   get_status(uint8 device, uint8 button) = 0;
  virtual void   set_status(uint8 device, uint8 button, bool status) = 0;
  virtual void   clear_input() = 0;
  virtual void   init() = 0;
  virtual void   term() = 0;
};

#include "dinput.h"
