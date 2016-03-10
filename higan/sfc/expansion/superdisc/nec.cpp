//NEC D75P308GF
//4-bit microcontroller
//CD-player interface

auto SuperDisc::necPollIRQ() -> uint8 {
  auto match = [&](const string& compare) -> bool {
    if(nec.command.size() != compare.size()) return false;
    for(auto n : range(nec.command)) {
      char c = compare[n];
      if(c == '?') continue;
      if(c >= '0' && c <= '9') c -= '0';
      if(c >= 'a' && c <= 'f') c -= 'a' - 10;
      if(nec.command[n] != c) return false;
    }
    return true;
  };

  //access ??/??/??
  if(match("b"       )) return 0x8f;
  if(match("b?"      )) return 0x8f;
  if(match("b??"     )) return 0x8f;
  if(match("b???"    )) return 0x8f;
  if(match("b????"   )) return 0x8f;
  if(match("b?????"  )) return 0x8f;
  if(match("b??????" )) return 0x8f;
  if(match("b??????f")) {
    nec.command.reset();
    return 0x8f;
  }

  //access t??/i??
  if(match("c"     )) return 0x8f;
  if(match("c?"    )) return 0x8f;
  if(match("c??"   )) return 0x8f;
  if(match("c???"  )) return 0x8f;
  if(match("c????" )) return 0x8f;
  if(match("c????f")) {
    nec.command.reset();
    return 0x8f;
  }

  //d-prefixes
  if(match("d" )) return 0x8f;
  if(match("d0")) return 0x8f;
  if(match("d1")) return 0x8f;
  if(match("d4")) return 0x8f;
  if(match("d5")) return 0x8f;

  //stop
  if(match("d01" )) return 0x8f;
  if(match("d01f")) {
    nec.command.reset();
    return 0x8f;
  }

  //play
  if(match("d02" )) return 0x8f;
  if(match("d02f")) {
    nec.command.reset();
    return 0x8f;
  }

  //pause
  if(match("d03" )) return 0x8f;
  if(match("d03f")) {
    nec.command.reset();
    return 0x8f;
  }

  //open / close
  if(match("d04" )) return 0x8f;
  if(match("d04f")) {
    nec.command.reset();
    return 0x8f;
  }

  //fast forward
  if(match("d10" )) return 0x8f;
  if(match("d10f")) {
    nec.command.reset();
    return 0x8f;
  }

  //fast reverse
  if(match("d11" )) return 0x8f;
  if(match("d11f")) {
    nec.command.reset();
    return 0x8f;
  }

  //forward
  if(match("d12" )) return 0x8f;
  if(match("d12f")) {
    nec.command.reset();
    return 0x8f;
  }

  //reverse
  if(match("d13" )) return 0x8f;
  if(match("d13f")) {
    nec.command.reset();
    return 0x8f;
  }

  //key direct
  if(match("d40" )) return 0x8f;
  if(match("d40f")) {
    nec.command.reset();
    return 0x8f;
  }

  //key ignore
  if(match("d41" )) return 0x8f;
  if(match("d41f")) {
    nec.command.reset();
    return 0x8f;
  }

  //continuous play
  if(match("d42" )) return 0x8f;
  if(match("d42f")) {
    nec.command.reset();
    return 0x8f;
  }

  //auto track pause
  if(match("d43" )) return 0x8f;
  if(match("d43f")) {
    nec.command.reset();
    return 0x8f;
  }

  //auto index pause
  if(match("d44" )) return 0x8f;
  if(match("d44f")) {
    nec.command.reset();
    return 0x8f;
  }

  //normal speed
  if(match("d45" )) return 0x8f;
  if(match("d45f")) {
    nec.command.reset();
    return 0x8f;
  }

  //double speed
  if(match("d46" )) return 0x8f;
  if(match("d46f")) {
    nec.command.reset();
    return 0x8f;
  }

  //q-data request
  if(match("d50"                  )) return 0x8f;
  if(match("d50f"                 )) return 0x8f;
  if(match("d50f?"                )) return 0x80;
  if(match("d50f??"               )) return 0x80;
  if(match("d50f???"              )) return 0x80;
  if(match("d50f????"             )) return 0x80;
  if(match("d50f?????"            )) return 0x80;
  if(match("d50f??????"           )) return 0x80;
  if(match("d50f???????"          )) return 0x80;
  if(match("d50f????????"         )) return 0x80;
  if(match("d50f?????????"        )) return 0x80;
  if(match("d50f??????????"       )) return 0x80;
  if(match("d50f???????????"      )) return 0x80;
  if(match("d50f????????????"     )) return 0x80;
  if(match("d50f?????????????"    )) return 0x80;
  if(match("d50f??????????????"   )) return 0x80;
  if(match("d50f???????????????"  )) return 0x80;
  if(match("d50f????????????????" )) return 0x80;
  if(match("d50f????????????????f")) {
    nec.command.reset();
    return 0x8f;
  }

  //status request
  if(match("d51"       )) return 0x8f;
  if(match("d51f"      )) return 0x8f;
  if(match("d51f0"     )) return 0x81;
  if(match("d51f01"    )) return 0x80;
  if(match("d51f012"   )) return 0x81;
  if(match("d51f0123"  )) return 0x80;
  if(match("d51f01234" )) return 0x80;
  if(match("d51f01234f")) {
    nec.command.reset();
    return 0x8f;
  }

  nec.command.reset();
  return 0x00;
}

auto SuperDisc::necReadData() -> uint8 {
  return nec.data;
}

auto SuperDisc::necWriteCommand(uint4 data) -> void {
  if(nec.command.size() >= 32) return;
  nec.command.append(data);
}
