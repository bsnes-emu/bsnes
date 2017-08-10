auto V30MZ::instructionInString(Size size) -> void {
  wait(5);
  if(!repeat() || r.cx) {
    auto data = in(size, r.dx);
    write(size, r.es, r.di, data);
    r.di += r.f.d ? -size : size;

    if(!repeat() || !--r.cx) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionOutString(Size size) -> void {
  wait(6);
  if(!repeat() || r.cx) {
    auto data = read(size, segment(r.ds), r.si);
    out(size, r.dx, data);
    r.si += r.f.d ? -size : size;

    if(!repeat() || !--r.cx) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionMoveString(Size size) -> void {
  wait(4);
  if(!repeat() || r.cx) {
    auto data = read(size, segment(r.ds), r.si);
    write(size, r.es, r.di, data);
    r.si += r.f.d ? -size : size;
    r.di += r.f.d ? -size : size;

    if(!repeat() || !--r.cx) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionCompareString(Size size) -> void {
  wait(5);
  if(!repeat() || r.cx) {
    auto x = read(size, segment(r.ds), r.si);
    auto y = read(size, r.es, r.di);
    r.si += r.f.d ? -size : size;
    r.di += r.f.d ? -size : size;
    SUB(size, x, y);

    if(!repeat() || !--r.cx) return;
    if(repeat() == RepeatWhileZero && r.f.z == 0) return;
    if(repeat() == RepeatWhileNotZero && r.f.z == 1) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionStoreString(Size size) -> void {
  wait(2);
  if(!repeat() || r.cx) {
    write(size, r.es, r.di, getAcc(size));
    r.di += r.f.d ? -size : size;

    if(!repeat() || !--r.cx) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionLoadString(Size size) -> void {
  wait(2);
  if(!repeat() || r.cx) {
    setAcc(size, read(size, segment(r.ds), r.si));
    r.si += r.f.d ? -size : size;

    if(!repeat() || !--r.cx) return;

    state.prefix = true;
    r.ip--;
  }
}

auto V30MZ::instructionScanString(Size size) -> void {
  wait(3);
  if(!repeat() || r.cx) {
    auto x = getAcc(size);
    auto y = read(size, r.es, r.di);
    r.di += r.f.d ? -size : size;
    SUB(size, x, y);

    if(!repeat() || !--r.cx) return;
    if(repeat() == RepeatWhileZero && r.f.z == 0) return;
    if(repeat() == RepeatWhileNotZero && r.f.z == 1) return;

    state.prefix = true;
    r.ip--;
  }
}
