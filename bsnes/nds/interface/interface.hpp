#ifndef NDS_HPP
namespace NintendoDS {
#endif

struct ID {
  struct Port { enum{
    Buttons, Sensors, Touchpad,
    Slot1, Slot2,
  }; };
    
  struct Device { enum{
    Empty=0, BuiltIn=0,
    /* Slot1 */ GameCard=1,
    /* Slot2 */ GamePak=1, ExpansionPak, RumblePak, GuitarGrip, Piano, Paddle,
  };};
  
  // Internal inputs
  struct Sensors { enum{ Lid, Battery, Mains, Temperature, Headphones, FIQ, Reset }; };
  struct Buttons { enum{ A,B, Select,Start, Right,Left,Up,Down, R,L, X,Y,C,D,Z,W }; };
  struct Touchpad { enum{ X, Y, Pressure, PressureD }; };
  
  // Slot 2 device inputs
  struct GuitarGrip { enum{ Green, Red, Yellow, Blue }; };
  struct Piano { enum{ C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, Ch }; };
  struct Paddle { enum{ Rotation }; };
};

struct Interface : Emulator::Interface {
  double videoFrequency();
  double audioFrequency();

  enum {
    System,
    NintendoDS,
    
    ARM7BIOS, ARM9BIOS, Firmware, Clock,
    Slot1ROM, Slot1EEPROM, Slot1FRAM, Slot1Flash,
    Slot2ROM, Slot2RAM, Slot2SRAM, Slot2EEPROM, Slot2FRAM, Slot2Flash,
  };
  
  unsigned group(unsigned id);
  bool loaded();
  void load(unsigned id, const string &manifest = "");
  void load(unsigned id, const stream &memory, const string &markup = "");
  void save();
  void save(unsigned id, const stream &memory);
  void unload();

  void power();
  void run();
  void videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height);

  serializer serialize();
  bool unserialize(serializer&);

  void paletteUpdate();

  Interface();

private:
  string systemManifest;
  string gameManifest;
  
  vector<Device> device;
  
  // System inputs
  Device sensors;
  Device buttons;
  Device touchpad;
  
  Device emptySlot;
  
  // Slot 1
  Device gameCard;
  
  // Slot 2 peripherals
  Device gamePak;       // for linking Pokémon D/P/HG/SS and R/S/E
  Device expansionPak;  // Opera browser, homebrew
  Device rumblePak;     // Metroid Pinball, others
  Device guitarGrip;    // Guitar Hero: On Tour
  Device piano;         // Easy Piano
  Device paddle;        // Arkanoid
  
  unsigned palette[01000000];
};

extern Interface *interface;

#ifndef NDS_HPP
}
#endif
