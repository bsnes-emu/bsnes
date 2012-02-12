void Usage::allocate(unsigned size_) {
  if(data) delete[] data;
  size = size_;
  data = new uint8_t[size]();
}

Usage::Usage() {
  data = nullptr;
  size = 0;
}

Usage::~Usage() {
  if(data) delete[] data;
}

void Debugger::loadUsage() {
  file fp;

  //if cartridge image was modified after the usage files,
  //then it is possible that the memory map has changed.
  //will print invalidation message when files do not exist as well.

  if(file::timestamp(interface->fileName, file::time::modify) >=
     file::timestamp({ interface->pathName, "debug/usage.cpu" }, file::time::modify)
  ) {
    print("CPU usage invalidated\n");
  } else {
    if(fp.open({ interface->pathName, "debug/usage.cpu" }, file::mode::read)) {
      fp.read(cpuUsage.data, min(cpuUsage.size, fp.size()));
      fp.close();
    }
  }

  if(file::timestamp(interface->fileName, file::time::modify) >=
     file::timestamp({ interface->pathName, "debug/usage.apu" }, file::time::modify)
  ) {
    print("APU usage invalidated\n");
  } else {
    if(fp.open({ interface->pathName, "debug/usage.apu" }, file::mode::read)) {
      fp.read(apuUsage.data, min(apuUsage.size, fp.size()));
      fp.close();
    }
  }
}

void Debugger::saveUsage() {
  string filename;
  filename = { interface->pathName, "debug/usage.cpu" };
  file::write(filename, cpuUsage.data, cpuUsage.size);
  filename = { interface->pathName, "debug/usage.apu" };
  file::write(filename, apuUsage.data, apuUsage.size);
}

void Debugger::resetUsage() {
  if(cpuUsage.data) memset(cpuUsage.data, 0, cpuUsage.size);
  if(apuUsage.data) memset(apuUsage.data, 0, apuUsage.size);
}
