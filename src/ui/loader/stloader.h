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
  uintptr_t close(event_t);
  uintptr_t bbase_tick(event_t);
  uintptr_t cbase_tick(event_t);
  uintptr_t bslotA_tick(event_t);
  uintptr_t cslotA_tick(event_t);
  uintptr_t bslotB_tick(event_t);
  uintptr_t cslotB_tick(event_t);
  uintptr_t load_tick(event_t);
  uintptr_t cancel_tick(event_t);
} window_stloader;
