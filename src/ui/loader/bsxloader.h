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
  uintptr_t close(event_t);
  uintptr_t bbase_tick(event_t);
  uintptr_t cbase_tick(event_t);
  uintptr_t bslot_tick(event_t);
  uintptr_t cslot_tick(event_t);
  uintptr_t load_tick(event_t);
  uintptr_t cancel_tick(event_t);

  BSXLoaderWindow();
} window_bsxloader;
