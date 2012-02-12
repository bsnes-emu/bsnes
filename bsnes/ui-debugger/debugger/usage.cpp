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
  if(fp.open({ interface->pathName, "debug/usage.cpu" }, file::mode::read)) {
    fp.read(cpuUsage.data, min(cpuUsage.size, fp.size()));
    fp.close();
  }
  if(fp.open({ interface->pathName, "debug/usage.apu" }, file::mode::read)) {
    fp.read(apuUsage.data, min(apuUsage.size, fp.size()));
    fp.close();
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
