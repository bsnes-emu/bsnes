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

  dprintf("* Loading [%s]...", rom_fn);

Reader *rf = new FileReader();
FileReader *f_rf = static_cast<FileReader*>(rf);
  if(!f_rf->open(rom_fn)) {
    alert("Error loading image file [%s]!", rom_fn);
    return false;
  }
  mem_bus->rom->load_rom(rf);
  f_rf->close();
  delete rf;

uint8 *sram_data;
uint32 sram_size;
  sram_size = load_file(sram_fn, &sram_data);
  if(sram_size) {
    mem_bus->rom->load_sram(sram_data, sram_size);
    memfree(sram_data);
  }

  file_loaded = true;
  bsnes->debugger_activate();
  return true;
}

void ROMImage::unload() {
  if(file_loaded == false)return;

uint8 *sram_data;
uint32 sram_size;
  sram_size = mem_bus->rom->save_sram(&sram_data);
  if(sram_size) {
    save_file(sram_fn, sram_data, sram_size);
    memfree(sram_data);
  }
  file_loaded = false;
  bsnes->debugger_deactivate();

  mem_bus->rom->unload();
}

void ROMImage::select(char *fn) {
  if(file_loaded == true)return;

/* Remove quotes */
  if(fn[0] == '\"') {
    strcpy(rom_fn, fn + 1);
    rom_fn[strlen(rom_fn) - 1] = 0;
  } else {
    strcpy(rom_fn, fn);
  }

  for(int i=strlen(rom_fn)-1;i>=0;i--) {
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
