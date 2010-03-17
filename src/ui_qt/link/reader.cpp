Reader reader;

const char* Reader::direct_supported() {
  return "";
}

bool Reader::direct_load(string &filename, uint8_t *&data, unsigned &size) {
  if(file::exists(filename) == false) return false;

  file fp;
  if(fp.open(filename, file::mode_read) == false) return false;

  data = new uint8_t[size = fp.size()];
  fp.read(data, size);
  fp.close();

  //remove copier header, if it exists
  if((size & 0x7fff) == 512) memmove(data, data + 512, size -= 512);

  return true;
}

bool Reader::direct_map(string &xml, const uint8_t *data, unsigned size) {
  xml = "";
  audio.clear();
  QMessageBox::warning(0, "bsnes Cartridge Mapping",
    "<p><b>Warning:</b><br>"
    "No XML cartridge mapping was found, and snesreader library is not "
    "installed to generate this mapping automatically. As a result of not "
    "being able to map the cartridge, loading has been aborted."
  );
  return false;
}

Reader::Reader() {
  if(open("snesreader")) {
    supported = sym("snesreader_supported");
    load = sym("snesreader_load");
    map = sym("snesreader_map");
  }

  if(!supported || !load) {
    supported = bind(&Reader::direct_supported, this);
    load = bind(&Reader::direct_load, this);
    map = bind(&Reader::direct_map, this);
  }

  compressionList = supported();
  if(compressionList.length() > 0) compressionList = string() << " " << compressionList;

  if(opened()) {
    extensionList = string()
    << " *.smc *.swc *.fig"
    << " *.ufo *.gd3 *.gd7 *.dx2 *.mgd *.mgh"
    << " *.048 *.058 *.068 *.078 *.bin"
    << " *.usa *.eur *.jap *.aus *.bsx";
  }
}
