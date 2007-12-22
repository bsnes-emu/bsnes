class BSXLoaderWindow : public Window {
public:
  enum Mode {
    ModeBSX, //BS-X cartridge
    ModeBSC, //BS-X slotted cartridge
  } mode;

  Label   lbase;
  Editbox tbase;
  Button  bbase, cbase;

  Label   lslot;
  Editbox tslot;
  Button  bslot, cslot;

  Button  load, cancel;

  void setup();
  uintptr_t close(Event);
  uintptr_t bbase_tick(Event);
  uintptr_t cbase_tick(Event);
  uintptr_t bslot_tick(Event);
  uintptr_t cslot_tick(Event);
  uintptr_t load_tick(Event);
  uintptr_t cancel_tick(Event);

  BSXLoaderWindow();
} window_bsxloader;
