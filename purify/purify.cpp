#include <nall/platform.hpp>
#include <nall/crc32.hpp>
#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/filemap.hpp>
#include <nall/string.hpp>
#include <nall/zip.hpp>
#include <nall/nes/cartridge.hpp>
#include <nall/snes/cartridge.hpp>
#include <nall/gb/cartridge.hpp>
#include <nall/gba/cartridge.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

struct Application : Window {
  file database;
  string source;
  string target;

  bool loadFile(const string &filename, string &suffix, uint8_t *&data, unsigned &size);

  void scanFamicom(const string &filename, uint8_t *data, unsigned size);
  void outputFamicom(const string &filename, uint8_t *data, unsigned size);

  void scanSuperFamicom(const string &filename, uint8_t *data, unsigned size);
  void outputSuperFamicom(const string &filename, uint8_t *data, unsigned size);

  void scanGameBoy(const string &filename, uint8_t *data, unsigned size);
  void outputGameBoy(const string &filename, uint8_t *data, unsigned size);

  void scanGameBoyColor(const string &filename, uint8_t *data, unsigned size);
  void outputGameBoyColor(const string &filename, uint8_t *data, unsigned size);

  void scanGameBoyAdvance(const string &filename, uint8_t *data, unsigned size);
  void outputGameBoyAdvance(const string &filename, uint8_t *data, unsigned size);

  void scanDirectory();
  void outputDirectory();
};

bool Application::loadFile(const string &filename, string &suffix, uint8_t *&data, unsigned &size) {
  print("-> ", notdir(filename), "\n");

  if(filename.endswith(".zip")) {
    zip archive;
    if(archive.open(filename) == false) return print("* failed to open archive\n"), false;
    if(archive.file.size() != 1) return print("* file count (", archive.file.size(), ") incorrect\n"), false;
    if(archive.extract(archive.file[0], data, size) == false) return print("* failed to extract ", filename, "\n"), false;
    uint32_t crc32 = crc32_calculate(data, size);
    if(crc32 != archive.file[0].crc32) return delete[] data, print("* CRC32 mismatch"), false;
    suffix = extension(archive.file[0].name);
    return true;
  }
  suffix = extension(filename);
  return file::read(filename, data, size);
}

//<famicom>

void Application::scanFamicom(const string &filename, uint8_t *data, unsigned size) {
  if(size & 255 != 16) return print("* ", filename, " missing iNES header\n");

  string sha256 = nall::sha256(data, size);
  database.print(sha256, "{}");
  database.print("0x", hex<8>(size - 16), "{}");
  for(unsigned n = 0; n < 16; n++) {
    database.print(hex<2>(data[n]), n != 15 ? " " : "{}");
  }
  database.print(notdir(nall::basename(filename)), "\n");
}

void Application::outputFamicom(const string &filename, uint8_t *data, unsigned size) {
  if(size & 255 != 16) return print("* ", filename, " missing iNES header\n");

  string markup = FamicomCartridge(data, size).markup;
  string path = {target, nall::basename(filename), ".fc/"};
  mkdir(path, 0777);
  if(file::exists({path, "program.rom"}) == false) {
    file::write({path, "program.rom"}, data + 16, size - 16);
  }
  file::write({path, "manifest.xml"}, (const uint8_t*)markup(), markup.length());
}

//</famicom>

//<superFamicom>

void Application::scanSuperFamicom(const string &filename, uint8_t *data, unsigned size) {
  if(size & 32767 == 512) size -= 512, data += 512;

  string sha256 = nall::sha256(data, size);
  database.print(sha256, "{}");
  database.print("0x", hex<8>(size), "{}");
  database.print(notdir(nall::basename(filename)), "\n");
}

void Application::outputSuperFamicom(const string &filename, uint8_t *data, unsigned size) {
  if(size & 32767 == 512) size -= 512, data += 512;

  string markup = SuperFamicomCartridge(data, size).markup;
  string path = {target, nall::basename(filename), ".sfc/"};
  mkdir(path, 0777);
  if(file::exists({path, "program.rom"}) == false) {
    file::write({path, "program.rom"}, data, size);
  }
  file::write({path, "manifest.xml"}, (const uint8_t*)markup(), markup.length());
}

//</superFamicom>

//<gameBoy>

void Application::scanGameBoy(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyCartridge(data, size).markup;
  XML::Document document(markup);

  string sha256 = nall::sha256(data, size);
  database.print(sha256, "{}");
  database.print("0x", hex<8>(size), "{}");
  database.print(document["cartridge"]["mapper"].data, "{}");
  database.print(notdir(nall::basename(filename)), "\n");
}

void Application::outputGameBoy(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyCartridge(data, size).markup;
  string path = {target, nall::basename(filename), ".gb/"};
  mkdir(path, 0777);
  if(file::exists({path, "program.rom"}) == false) {
    file::write({path, "program.rom"}, data, size);
  }
  file::write({path, "manifest.xml"}, (const uint8_t*)markup(), markup.length());
}

//</gameBoy>

//<gameBoyColor>

void Application::scanGameBoyColor(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyCartridge(data, size).markup;
  XML::Document document(markup);

  string sha256 = nall::sha256(data, size);
  database.print(sha256, "{}");
  database.print("0x", hex<8>(size), "{}");
  database.print(document["cartridge"]["mapper"].data, "{}");
  database.print(data[0x0143] == 0xc0 ? "N" : "Y", "{}");
  database.print(notdir(nall::basename(filename)), "\n");
}

void Application::outputGameBoyColor(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyCartridge(data, size).markup;
  string path = {target, nall::basename(filename), data[0x0143] == 0xc0 ? ".gbc/" : ".gbb/"};
  mkdir(path, 0777);
  if(file::exists({path, "program.rom"}) == false) {
    file::write({path, "program.rom"}, data, size);
  }
  file::write({path, "manifest.xml"}, (const uint8_t*)markup(), markup.length());
}

//</gameBoyColor>

//<gameBoyAdvance>

void Application::scanGameBoyAdvance(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyAdvanceCartridge(data, size).markup;
  string identifiers;
  if(auto start = markup.position("detected: ")) {
    if(auto finish = markup.position(" -->\n")) {
      identifiers = substr(markup, start() + 10, finish() - start() - 10);
    }
  }

  string sha256 = nall::sha256(data, size);
  database.print(sha256, "{}");
  database.print("0x", hex<8>(size), "{}");
  database.print(identifiers, "{}");
  database.print(notdir(nall::basename(filename)), "\n");
}

void Application::outputGameBoyAdvance(const string &filename, uint8_t *data, unsigned size) {
  string markup = GameBoyAdvanceCartridge(data, size).markup;
  string path = {target, nall::basename(filename), ".gba/"};
  mkdir(path, 0777);
  if(file::exists({path, "program.rom"}) == false) {
    file::write({path, "program.rom"}, data, size);
  }
  file::write({path, "manifest.xml"}, (const uint8_t*)markup(), markup.length());
}

//</gameBoyAdvance>

void Application::scanDirectory() {
  if(directory::exists(source) == false) return print("* source directory does not exist\n");
  if(database.open(target, file::mode::write) == false) return print("* cannot open target file\n");

  lstring files = directory::files(source);
  for(auto &filename : files) {
    string suffix;
    uint8_t *data;
    unsigned size;
    if(loadFile({source, filename}, suffix, data, size) == false) continue;

    if(suffix == "nes" || suffix == "fc") scanFamicom(filename, data, size);
    if(suffix == "sfc" || suffix == "smc") scanSuperFamicom(filename, data, size);
    if(suffix == "gb") scanGameBoy(filename, data, size);
    if(suffix == "gbc" || suffix == "gbb") scanGameBoyColor(filename, data, size);
    if(suffix == "gba") scanGameBoyAdvance(filename, data, size);

    delete[] data;
  }

  database.close();
}

void Application::outputDirectory() {
  if(directory::exists(source) == false) return print("* source directory does not exist\n");
  if(directory::exists(target) == false) return print("* target directory does not exist\n");

  lstring files = directory::files(source);
  for(auto &filename : files) {
    string suffix;
    uint8_t *data;
    unsigned size;
    if(loadFile({source, filename}, suffix, data, size) == false) continue;

    if(suffix == "nes" || suffix == "fc") outputFamicom(filename, data, size);
    if(suffix == "sfc" || suffix == "smc") outputSuperFamicom(filename, data, size);
    if(suffix == "gb") outputGameBoy(filename, data, size);
    if(suffix == "gbc" || suffix == "gbb") outputGameBoyColor(filename, data, size);
    if(suffix == "gba") outputGameBoyAdvance(filename, data, size);

    delete[] data;
  }
}

int main(int argc, char **argv) {
  Application *application = new Application;

  if(argc == 4 && cstring{argv[1]} == "scan") {
    application->source = argv[2];
    application->target = argv[3];
    application->scanDirectory();
  }

  else if(argc == 4 && cstring{argv[1]} == "output") {
    application->source = argv[2];
    application->target = argv[3];
    application->outputDirectory();
  }

  else {
    print("purify v01\n");
    print("usage: purify [mode] source target\n\n");
    print("modes:\n");
    print("  scan   - create database\n");
    print("  output - create folder images\n");
  }

  delete application;
  return 0;
}
