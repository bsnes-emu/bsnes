class STLoaderWindow : public Window {
public:
  Label   lbase;
  Editbox tbase;
  Button  bbase, cbase;

  Label   lslotA;
  Editbox tslotA;
  Button  bslotA, cslotA;

  Label   lslotB;
  Editbox tslotB;
  Button  bslotB, cslotB;

  Button  load, cancel;

  void setup();
  uintptr_t close(Event);
  uintptr_t bbase_tick(Event);
  uintptr_t cbase_tick(Event);
  uintptr_t bslotA_tick(Event);
  uintptr_t cslotA_tick(Event);
  uintptr_t bslotB_tick(Event);
  uintptr_t cslotB_tick(Event);
  uintptr_t load_tick(Event);
  uintptr_t cancel_tick(Event);
} window_stloader;
