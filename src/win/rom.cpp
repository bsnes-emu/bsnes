class ROMImage {
private:
char rom_fn[4096];
char sram_fn[4096];
bool file_loaded;

public:
  bool loaded();
  bool load();
  void unload();
  void select(char *fn);
  ROMImage();
  ~ROMImage();
};

bool ROMImage::loaded() {
  return file_loaded;
}

bool ROMImage::load() {
  if(file_loaded == true)return false;

  dprintf("* Loading \"%s\"...", rom_fn);

FileReader *rf = new FileReader();
  if(!rf->open(FileReader::TYPE_ROM, rom_fn)) {
    alert("Error loading image file [%s]!", rom_fn);
    return false;
  }
  mem_bus->load_cart(static_cast<Reader*>(rf));
  rf->close();

CartInfo ci;
  mem_bus->get_cartinfo(&ci);
  if(ci.sram_size != 0) {
    rf->open(FileReader::TYPE_SRAM, sram_fn);
    mem_bus->load_sram(static_cast<Reader*>(rf));
    rf->close();
  }

  delete(rf);

  file_loaded = true;
  bsnes->debugger_activate();
  return true;
}

void ROMImage::unload() {
  if(file_loaded == false)return;

FileWriter *wf;
CartInfo ci;
  mem_bus->get_cartinfo(&ci);
  if(ci.sram_size != 0) {
    wf = new FileWriter();
    wf->open(sram_fn);
    mem_bus->save_sram(static_cast<Writer*>(wf));
    wf->close();
    delete(wf);
  }

  file_loaded = false;
  bsnes->debugger_deactivate();

  mem_bus->unload_cart();
}

void ROMImage::select(char *fn) {
int i;
  if(file_loaded == true)return;

/* Remove quotes */
  if(fn[0] == '\"') {
    strcpy(rom_fn, fn + 1);
    rom_fn[strlen(rom_fn) - 1] = 0;
  } else {
    strcpy(rom_fn, fn);
  }

  for(i=strlen(rom_fn)-1;i>=0;i--) {
    if(rom_fn[i] == '.')break;
  }

  strcpy(sram_fn, rom_fn);
  if(rom_fn[i] == '.')sram_fn[i] = 0;
  strcat(sram_fn, ".srm");
}

ROMImage::ROMImage() {
  *rom_fn  = 0;
  *sram_fn = 0;
  file_loaded = false;
}

ROMImage::~ROMImage() {
  if(file_loaded == true) {
    unload();
  }
}

ROMImage *rom_image;
