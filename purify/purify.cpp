#include <nall/config.hpp>
#include <nall/directory.hpp>
#include <nall/file.hpp>
#include <nall/invoke.hpp>
#include <nall/string.hpp>
#include <nall/zip.hpp>
#include <nall/emulation/famicom.hpp>
#include <nall/emulation/super-famicom.hpp>
#include <nall/emulation/satellaview.hpp>
#include <nall/emulation/sufami-turbo.hpp>
#include <nall/emulation/game-boy.hpp>
#include <nall/emulation/game-boy-advance.hpp>
using namespace nall;

#include <phoenix/phoenix.hpp>
using namespace phoenix;

#include "resource/resource.hpp"
#include "resource/resource.cpp"

struct Settings : configuration {
  bool ui;    //true if in user-interface mode (windows visible); false if in command-line mode
  bool play;  //true if emulator should be launched after game conversion
  lstring extensions;

  string emulator;
  string path;
  string recent;

  Settings() {
    ui = false;
    play = false;
    extensions = {".fc", ".nes", ".sfc", ".smc", ".swc", ".fig", ".bs", ".st", ".gb", ".gbc", ".sgb", ".gba"};

    directory::create({configpath(), "purify/"});
    append(emulator = "bsnes", "emulator");
    append(path = {configpath(), "Emulation/"}, "path");
    append(recent = {userpath(), "Desktop/"}, "recent");
    load({configpath(), "purify/settings.cfg"});
    save({configpath(), "purify/settings.cfg"});
  }

  ~Settings() {
    save({configpath(), "purify/settings.cfg"});
  }
} settings;

void play(const string &pathname) {
  settings.play = false;
  invoke(settings.emulator, pathname);
}

bool valid_extension(string name) {
  name.rtrim<1>("/");
  for(auto &extension : settings.extensions) {
    if(name.iendswith(extension)) return true;
  }
  return false;
}

void create_famicom(const string &filename, uint8_t *data, unsigned size) {
  FamicomCartridge information(data, size);
  if(information.markup.empty()) return;
  string name = {nall::basename(notdir(filename)), ".fc/"};

  print(name, "\n");
  string path = {settings.path, "Famicom/", name};
  directory::create(path, 0755);

  //skip iNES header
  data += 16, size -= 16;

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  if(information.prgrom > 0) file::write({path, "program.rom"}, data, information.prgrom);
  if(information.chrrom > 0) file::write({path, "character.rom"}, data + information.prgrom, information.chrrom);
  if(!file::exists({path, "save.rwm"})) file::copy({nall::basename(filename), ".sav"}, {path, "save.rwm"});
  if(settings.play) play(path);
}

void create_super_famicom(const string &filename, uint8_t *data, unsigned size) {
  SuperFamicomCartridge information(data, size);
  if(information.markup.empty()) return;
  string name = {nall::basename(notdir(filename)), ".sfc/"};

  print(name, "\n");
  string path = {settings.path, "Super Famicom/", name};
  directory::create(path, 0755);

  //skip copier header
  if((size & 0x7fff) == 512) data += 512, size -= 512;

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  if(information.markup.position("<spc7110>") && size >= 0x100000) {
    file::write({path, "program.rom"}, data, 0x100000);
    file::write({path, "data.rom"}, data + 0x100000, size - 0x100000);
  } else {
    file::write({path, "program.rom"}, data, size);
  }
  if(!file::exists({path, "save.rwm"})) file::copy({nall::basename(filename), ".srm"}, {path, "save.rwm"});

  //firmware
  if(auto position = information.markup.position("firmware=")) {
    string firmware = substr(information.markup, position() + 10);
    if(auto position = firmware.position("\"")) {
      firmware[position()] = 0;
      if(file::copy({dir(filename), firmware}, {path, firmware}) == false) {
        print("Warning: required firmware \"", firmware, "\" not found!\n");
      }
    }
  }

  if(settings.play) play(path);
}

void create_satellaview(const string &filename, uint8_t *data, unsigned size) {
  SatellaviewCartridge information(data, size);
  if(information.markup.empty()) return;
  string name = {nall::basename(notdir(filename)), ".bs/"};

  print(name, "\n");
  string path = {settings.path, "BS-X Satellaview/", name};
  directory::create(path, 0755);

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  file::write({path, "program.rom"}, data, size);
  if(settings.play) play(path);
}

void create_sufami_turbo(const string &filename, uint8_t *data, unsigned size) {
  SufamiTurboCartridge information(data, size);
  if(information.markup.empty()) return;
  string name = {nall::basename(notdir(filename)), ".st/"};

  print(name, "\n");
  string path = {settings.path, "Sufami Turbo/", name};
  directory::create(path, 0755);

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  file::write({path, "program.rom"}, data, size);
  if(!file::exists({path, "save.rwm"})) file::copy({nall::basename(filename), ".srm"}, {path, "save.rwm"});
}

void create_game_boy(const string &filename, uint8_t *data, unsigned size) {
  GameBoyCartridge information(data, size);
  if(information.markup.empty()) return;
  string system = information.info.cgb ? "Game Boy Color/" : "Game Boy/";
  string extension = information.info.cgb ? ".gbc/" : ".gb/";
  string name = {nall::basename(notdir(filename)), extension};

  print(name, "\n");
  string path = {settings.path, system, name};
  directory::create(path, 0755);

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  file::write({path, "program.rom"}, data, size);
  if(!file::exists({path, "save.rwm"})) file::copy({nall::basename(filename), ".sav"}, {path, "save.rwm"});
  if(settings.play) play(path);
}

void create_game_boy_advance(const string &filename, uint8_t *data, unsigned size) {
  GameBoyAdvanceCartridge information(data, size);
  if(information.markup.empty()) return;
  string name = {nall::basename(notdir(filename)), ".gba/"};

  print(name, "\n");
  string path = {settings.path, "Game Boy Advance/", name};
  directory::create(path, 0755);

  file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  file::write({path, "program.rom"}, data, size);
  if(!file::exists({path, "save.rwm"})) file::copy({nall::basename(filename), ".sav"}, {path, "save.rwm"});
  if(settings.play) play(path);
}

void create_data(const string &name, uint8_t *data, unsigned size) {
  if(name.iendswith(".fc")
  || name.iendswith(".nes")
  ) return create_famicom(name, data, size);

  if(name.iendswith(".sfc")
  || name.iendswith(".smc")
  || name.iendswith(".swc")
  || name.iendswith(".fig")
  ) return create_super_famicom(name, data, size);

  if(name.iendswith(".bs")
  ) return create_satellaview(name, data, size);

  if(name.iendswith(".st")
  ) return create_sufami_turbo(name, data, size);

  if(name.iendswith(".gb")
  || name.iendswith(".gbc")
  || name.iendswith(".sgb")
  ) return create_game_boy(name, data, size);

  if(name.iendswith(".gba")
  ) return create_game_boy_advance(name, data, size);
}

void create_file(const string &filename) {
  auto buffer = file::read(filename);
  if(buffer.size() == 0) return;

  return create_data(filename, buffer.data(), buffer.size());
}

void create_zip(const string &filename) {
  zip archive;
  if(archive.open(filename) == false) return;

  for(auto &file : archive.file) {
    if(file.data == nullptr || file.size == 0) return;
    if(valid_extension(file.name) == false) continue;

    auto buffer = archive.extract(file);
    if(buffer.size() == 0) continue;

    string name = {nall::basename(filename), ".", extension(file.name)};
    return create_data(name, buffer.data(), buffer.size());
  }
}

void create_manifest(const string &path) {
  string name = path;
  name.rtrim<1>("/");
  name = {notdir(name), "/"};

  //Famicom manifests cannot be generated from PRG+CHR ROMs alone
  //In the future, a games database may enable manifest generation

  if(path.iendswith(".sfc/") && file::exists({path, "program.rom"})) {
    print(name, "\n");
    auto buffer = file::read({path, "program.rom"});
    SuperFamicomCartridge information(buffer.data(), buffer.size());
    file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  }

  if(path.iendswith(".bs/") && file::exists({path, "program.rom"})) {
    print(name, "\n");
    auto buffer = file::read({path, "program.rom"});
    SatellaviewCartridge information(buffer.data(), buffer.size());
    file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  }

  if(path.iendswith(".st/") && file::exists({path, "program.rom"})) {
    print(name, "\n");
    auto buffer = file::read({path, "program.rom"});
    SufamiTurboCartridge information(buffer.data(), buffer.size());
    file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  }

  if((path.iendswith(".gb/") || path.iendswith(".gbc/")) && file::exists({path, "program.rom"})) {
    print(name, "\n");
    auto buffer = file::read({path, "program.rom"});
    GameBoyCartridge information(buffer.data(), buffer.size());
    file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  }

  if(path.iendswith(".gba/") && file::exists({path, "program.rom"})) {
    print(name, "\n");
    auto buffer = file::read({path, "program.rom"});
    GameBoyAdvanceCartridge information(buffer.data(), buffer.size());
    file::write({path, "manifest.xml"}, (const uint8_t*)information.markup(), information.markup.length());
  }
}

void create_folder(string pathname) {
  pathname.transform("\\", "/");
  if(pathname.endswith("/") == false) pathname.append("/");

  if(pathname == settings.path) {
    print(
      "You cannot use the same path for both the source and destination directories.\n"
      "Please choose a different output path in settings.cfg.\n"
    );
    return;
  }

  lstring files = directory::contents(pathname);
  for(auto &name : files) {
    if(directory::exists({pathname, name}) && valid_extension(name)) {
      create_manifest({pathname, name});
    } else if(name.iendswith(".zip")) {
      create_zip({pathname, name});
    } else {
      create_file({pathname, name});
    }
  }
}

struct Progress : Window {
  VerticalLayout layout;
  Label title;
  HorizontalLayout fileLayout;
    Label fileLabel;
    Label fileName;
  HorizontalLayout progressLayout;
    ProgressBar progressBar;
    Button stopButton;
  bool quit;

  void convert(const string &pathname) {
    fileName.setText("Initializing ...");
    progressBar.setPosition(0);
    stopButton.setEnabled(true);

    quit = false;
    setVisible(true);
    setModal(true);

    lstring files = directory::contents(pathname);
    for(unsigned n = 0; n < files.size() && quit == false; n++) {
      auto &filename = files(n);
      if(!filename.iendswith(".zip") && !valid_extension(filename)) continue;
      OS::processEvents();

      double position = (double)n / (double)files.size() * 100.0 + 0.5;
      progressBar.setPosition((unsigned)position);

      string name = filename;
      name.rtrim<1>("/");
      fileName.setText(notdir(name));

      if(directory::exists({pathname, filename}) && valid_extension(name)) {
        create_manifest({pathname, filename});
      } else if(filename.iendswith(".zip")) {
        create_zip({pathname, filename});
      } else {
        create_file({pathname, filename});
      }
    }

    if(quit == false) {
      fileName.setText("All games have been converted.");
      progressBar.setPosition(100);
    } else {
      fileName.setText("Process aborted. Not all games have been converted.");
    }
    stopButton.setEnabled(false);

    setModal(false);
  }

  Progress() {
    setTitle("purify");
    layout.setMargin(5);
    title.setFont("Sans, 16, Bold");
    title.setText("Conversion Progress");
    fileLabel.setFont("Sans, 8, Bold");
    fileLabel.setText("Filename:");
    stopButton.setText("Stop");

    append(layout);
    layout.append(title, {~0, 0}, 5);
    layout.append(fileLayout, {~0, 0}, 5);
      fileLayout.append(fileLabel, {0, 0}, 5);
      fileLayout.append(fileName, {~0, 0});
    layout.append(progressLayout, {~0, 0});
      progressLayout.append(progressBar, {~0, 0}, 5);
      progressLayout.append(stopButton, {80, 0});

    setGeometry({192, 192, 560, layout.minimumGeometry().height});

    stopButton.onActivate = [&] { quit = true; };
  }
} *progress = nullptr;

struct Application : Window {
  VerticalLayout layout;
  Label title;
  HorizontalLayout purifyLayout;
  Button playButton;
  Button convertButton;
  HorizontalLayout configLayout;
  Button emulatorButton;
  Button pathButton;
  HorizontalLayout emulatorLayout;
    Label emulatorName;
    Label emulatorValue;
  HorizontalLayout pathLayout;
    Label pathName;
    Label pathValue;

  Application() {
    setTitle("purify v00.06");
    setGeometry({128, 128, 440, 180});
    layout.setMargin(5);
    title.setFont("Sans, 16, Bold");
    title.setText("Choose Action");
    playButton.setImage({resource::play, sizeof resource::play});
    playButton.setText("Play Game");
    convertButton.setImage({resource::convert, sizeof resource::convert});
    convertButton.setText("Convert Games");
    emulatorButton.setImage({resource::emulator, sizeof resource::emulator});
    emulatorButton.setText("Choose Emulator");
    pathButton.setImage({resource::path, sizeof resource::path});
    pathButton.setText("Choose Output Path");
    emulatorName.setFont("Sans, 8, Bold");
    emulatorName.setText("Emulator:");
    emulatorValue.setText(settings.emulator);
    pathName.setFont("Sans, 8, Bold");
    pathName.setText("Output Path:");
    pathValue.setText(settings.path);

    Font font("Sans, 8, Bold");
    unsigned width = max(font.geometry("Emulator:").width, font.geometry("Output Path:").width);

    append(layout);
    layout.append(title, {~0, 0}, 5);
    layout.append(purifyLayout, {~0, ~0}, 5);
      purifyLayout.append(playButton, {~0, ~0}, 5);
      purifyLayout.append(convertButton, {~0, ~0});
    layout.append(configLayout, {~0, ~0}, 5);
      configLayout.append(emulatorButton, {~0, ~0}, 5);
      configLayout.append(pathButton, {~0, ~0});
    layout.append(emulatorLayout, {~0, 0}, 5);
      emulatorLayout.append(emulatorName, {width, 0}, 5);
      emulatorLayout.append(emulatorValue, {~0, 0});
    layout.append(pathLayout, {~0, 0});
      pathLayout.append(pathName, {width, 0}, 5);
      pathLayout.append(pathValue, {~0, 0});

    onClose = &OS::quit;
    playButton.onActivate = {&Application::playAction, this};
    convertButton.onActivate = {&Application::convertAction, this};
    emulatorButton.onActivate = {&Application::emulatorAction, this};
    pathButton.onActivate = {&Application::pathAction, this};

    setVisible();
  }

  void playAction() {
    string filters = {settings.extensions.concatenate(","), ",.zip"};
    filters.replace(".", "*.");
    string filename = DialogWindow::fileOpen(*this, settings.recent, string{"Game Images (", filters, ")"});
    if(!filename.empty()) {
      setVisible(false);
      settings.recent = dir(filename);

      settings.play = true;
      if(filename.iendswith(".zip")) {
        create_zip(filename);
      } else {
        create_file(filename);
      }
      exit(0);
    }
  }

  void convertAction() {
    string pathname = DialogWindow::folderSelect(*this, settings.recent);
    if(pathname.empty()) return;

    if(pathname == settings.path) {
      MessageWindow::critical(*this,
        "You cannot use the same path for both the source and destination directories.\n\n"
        "Please choose a different output path."
      );
      return;
    }

    settings.recent = pathname;
    progress->convert(pathname);
  }

  void emulatorAction() {
    string filter = {
      "Emulators ", Intrinsics::platform() == Intrinsics::Platform::Windows
      ? "(*.exe)"
      : "(*)"
    };
    string filename = DialogWindow::fileOpen(*this, settings.recent, filter);
    if(!filename.empty()) {
      settings.recent = dir(filename);
      emulatorValue.setText(settings.emulator = filename);
    }
  }

  void pathAction() {
    string pathname = DialogWindow::folderSelect(*this, settings.recent);
    if(!pathname.empty()) {
      settings.recent = pathname;
      pathValue.setText(settings.path = pathname);
    }
  }
} *application = nullptr;

int main(int argc, char **argv) {
  #if defined(PLATFORM_WINDOWS)
  utf8_args(argc, argv);
  #endif

  lstring args;
  for(unsigned n = 1; n < argc; n++) {
    string argument = argv[n];
    argument.replace("~/", userpath());
    args.append(argument);
  }

  if(args.size() == 1 && directory::exists(args[0])) {
    create_folder(args[0]);
    return 0;
  }

  if(args.size() == 1 && istrend(args[0], ".zip")) {
    settings.play = true;
    create_zip(args[0]);
    return 0;
  }

  if(args.size() == 1 && file::exists(args[0]) && valid_extension(args[0])) {
    settings.play = true;
    create_file(args[0]);
    return 0;
  }

  settings.ui = true;
  progress = new Progress;
  application = new Application;
  OS::main();
  return 0;
}
