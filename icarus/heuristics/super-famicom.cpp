namespace Heuristics {

struct SuperFamicom : Heuristics {
  SuperFamicom(vector<uint8_t>& data, string location);
  explicit operator bool() const;

  auto manifest() const -> string;
  auto region() const -> string;
  auto revision() const -> string;
  auto board() const -> string;
  auto label() const -> string;
  auto serial() const -> string;
  auto romSize() const -> uint;
  auto ramSize() const -> uint;
  auto expansionRamSize() const -> uint;
  auto battery() const -> bool;

private:
  auto size() const -> uint { return data.size(); }
  auto scoreHeader(uint address) -> uint;
  auto firmwareARM() const -> string;
  auto firmwareHITACHI() const -> string;
  auto firmwareNEC() const -> string;
  auto firmwareNECEX() const -> string;
  auto firmwareSGB() const -> string;

  vector<uint8_t>& data;
  string location;
  uint headerAddress = 0;
};

SuperFamicom::SuperFamicom(vector<uint8_t>& data, string location) : data(data), location(location) {
  if((size() & 0x7fff) == 512) {
    //remove header if present
    memory::move(&data[0], &data[512], size() - 512);
    data.resize(size() - 512);
  }

  if(size() < 0x8000) return;  //ignore images too small to be valid

  uint scoreLo = scoreHeader(  0x7fb0);
  uint scoreHi = scoreHeader(  0xffb0);
  uint scoreEx = scoreHeader(0x40ffb0);
  if(scoreEx) scoreEx += 4;

  if(scoreLo >= scoreHi && scoreLo >= scoreEx) headerAddress = 0x7fb0;
  else if(scoreHi >= scoreEx) headerAddress = 0xffb0;
  else headerAddress = 0x40ffb0;
}

SuperFamicom::operator bool() const {
  return headerAddress;
}

auto SuperFamicom::manifest() const -> string {
  if(!operator bool()) return {};

  string output;
  output.append("game\n");
  output.append("  sha256:   ", Hash::SHA256(data).digest(), "\n");
  output.append("  region:   ", region(), "\n");
  output.append("  revision: ", revision(), "\n");
  output.append("  board:    ", board(), "\n");
  output.append("  name:     ", Location::prefix(location), "\n");
  output.append("  label:    ", label(), "\n");

  auto board = this->board().split("-");

         if(board.left() == "ARM") {
    output.append(memory("ROM", size() - 0x28000, "program.rom"));
    output.append(memory("ROM", 0x20000, {firmwareARM(), ".program.rom"}));
    output.append(memory("ROM",  0x8000, {firmwareARM(), ".data.rom"}));
  } else if(board.left() == "HITACHI") {
    output.append(memory("ROM", size() - 0xc00, "program.rom"));
    output.append(memory("ROM", 0xc00, {firmwareHITACHI(), ".data.rom"}));
  } else if(board.left() == "NEC") {
    output.append(memory("ROM", size() - 0x2000, "program.rom"));
    output.append(memory("ROM", 0x1800, {firmwareNEC(), ".program.rom"}));
    output.append(memory("ROM",  0x800, {firmwareNEC(), ".data.rom"}));
  } else if(board.left() == "NECEX") {
    output.append(memory("ROM", size() - 0xd000, "program.rom"));
    output.append(memory("ROM", 0xc000, {firmwareNECEX(), ".program.rom"}));
    output.append(memory("ROM", 0x1000, {firmwareNECEX(), ".data.rom"}));
  } else if(board.left() == "SGB") {
    output.append(memory("ROM", size() - 0x100, "program.rom"));
    output.append(memory("ROM", 0x100, {firmwareSGB(), ".boot.rom"}));
  } else if(board.left() == "SPC7110") {
    output.append(memory("ROM", 0x100000, "program.rom"));
    output.append(memory("ROM", size() - 0x100000, "data.rom"));
  } else {
    output.append(memory("ROM", size(), "program.rom"));
  }

  if(auto size = ramSize()) {
    auto type = battery() ? "NVRAM" : "RAM";
    output.append(memory(type, size, "save.ram"));
  }

  if(auto size = expansionRamSize()) {
    auto type = battery() ? "NVRAM" : "RAM";
    output.append(memory(type, size, "expansion.ram"));
  }

         if(board.left() == "ARM") {
    output.append(memory("NVRAM", 0x4000, {firmwareARM(), ".data.ram"}));
  } else if(board.left() == "BS" && board(1) == "MCC") {
    output.append(memory("PSRAM", 0x80000, "download.ram"));
  } else if(board.left() == "HITACHI") {
    output.append(memory("RAM", 0xc00, {firmwareHITACHI(), ".data.ram"}));
  } else if(board.left() == "NEC") {
    output.append(memory("RAM", 0x200, {firmwareNEC(), ".data.ram"}));
  } else if(board.left() == "NECEX") {
    output.append(memory("NVRAM", 0x1000, {firmwareNEC(), ".data.ram"}));
  } else if(board.left() == "RTC") {
    output.append(memory("NVRAM", 0x10, "sharp.rtc.ram"));
  } else if(board.left() == "SA1") {
    output.append(memory("RAM", 0x800, "internal.ram"));
  } else if(board.left() == "SPC7110" && board(1) == "RTC") {
    output.append(memory("NVRAM", 0x10, "epson.rtc.ram"));
  }

  return output;
}

auto SuperFamicom::region() const -> string {
  string region;

  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)
  auto E = data[headerAddress + 0x29];  //region code (old)

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    string code{A, B, C, D};
    if(D == 'B') region = {"SNS-",  code, "-BRA"};
    if(D == 'C') region = {"SNSN-", code, "-ROC"};
    if(D == 'D') region = {"SNSP-", code, "-NOE"};
    if(D == 'E') region = {"SNS-",  code, "-USA"};
    if(D == 'F') region = {"SNSP-", code, "-FRA"};
    if(D == 'H') region = {"SNSP-", code, "-HOL"};
    if(D == 'I') region = {"SNSP-", code, "-ITA"};
    if(D == 'J') region = {"SHVC-", code, "-JPN"};
    if(D == 'K') region = {"SNSN-", code, "-KOR"};
    if(D == 'N') region = {"SNS-",  code, "-CAN"};
    if(D == 'P') region = {"SNSP-", code, "-EUR"};
    if(D == 'S') region = {"SNSP-", code, "-ESP"};
    if(D == 'U') region = {"SNSP-", code, "-AUS"};
    if(D == 'W') region = {"SNSP-", code, "-SCN"};
  }

  if(!region) {
    if(E == 0x00) region = {"SHVC-JPN"};
    if(E == 0x01) region = { "SNS-USA"};
    if(E == 0x02) region = {"SNSP-EUR"};
    if(E == 0x03) region = {"SNSP-SCN"};
    if(E == 0x06) region = {"SNSP-FRA"};
    if(E == 0x07) region = {"SNSP-HOL"};
    if(E == 0x08) region = {"SNSP-ESP"};
    if(E == 0x09) region = {"SNSP-NOE"};
    if(E == 0x0a) region = {"SNSP-ITA"};
    if(E == 0x0b) region = {"SNSP-ROC"};
    if(E == 0x0d) region = {"SNSP-KOR"};
    if(E == 0x0f) region = { "SNS-CAN"};
    if(E == 0x10) region = { "SNS-BRA"};
    if(E == 0x11) region = {"SNSP-AUS"};
  }

  return region ? region : "NTSC";
}

auto SuperFamicom::revision() const -> string {
  string revision;

  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)
  auto E = data[headerAddress + 0x29];  //region code (old)
  uint F = data[headerAddress + 0x2b];  //revision code

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    string code{A, B, C, D};
    if(D == 'B') revision = {"SNS-",  code, "-", F};
    if(D == 'C') revision = {"SNSN-", code, "-", F};
    if(D == 'D') revision = {"SNSP-", code, "-", F};
    if(D == 'E') revision = {"SNS-",  code, "-", F};
    if(D == 'F') revision = {"SNSP-", code, "-", F};
    if(D == 'H') revision = {"SNSP-", code, "-", F};
    if(D == 'I') revision = {"SNSP-", code, "-", F};
    if(D == 'J') revision = {"SHVC-", code, "-", F};
    if(D == 'K') revision = {"SNSN-", code, "-", F};
    if(D == 'N') revision = {"SNS-",  code, "-", F};
    if(D == 'P') revision = {"SNSP-", code, "-", F};
    if(D == 'S') revision = {"SNSP-", code, "-", F};
    if(D == 'U') revision = {"SNSP-", code, "-", F};
    if(D == 'W') revision = {"SNSP-", code, "-", F};
  }

  if(!revision) {
    if(E == 0x00) revision = {"SHVC-", F};
    if(E == 0x01) revision = { "SNS-", F};
    if(E == 0x02) revision = {"SNSP-", F};
    if(E == 0x03) revision = {"SSWE-", F};
    if(E == 0x06) revision = {"SFRA-", F};
    if(E == 0x07) revision = {"SHOL-", F};
    if(E == 0x08) revision = {"SESP-", F};
    if(E == 0x09) revision = {"SFRG-", F};
    if(E == 0x0a) revision = {"SITA-", F};
    if(E == 0x0b) revision = {"SSCN-", F};
    if(E == 0x0d) revision = {"SKOR-", F};
    if(E == 0x0f) revision = { "SNS-", F};
    if(E == 0x10) revision = {"SBRA-", F};
    if(E == 0x11) revision = {"SNSP-", F};
  }

  return revision ? revision : string{"1.", F};
}

auto SuperFamicom::board() const -> string {
  auto mapMode = data[headerAddress + 0x25];
  auto cartridgeType = data[headerAddress + 0x26];
  auto cartridgeTypeLo = cartridgeType & 15;
  auto cartridgeTypeHi = cartridgeType >> 4;
  auto cartridgeSubType = data[headerAddress + 0x0f];

  string board;

  string mode;
  if(mapMode == 0x20 || mapMode == 0x30) mode = "LOROM-";
  if(mapMode == 0x21 || mapMode == 0x31) mode = "HIROM-";
  if(mapMode == 0x22 || mapMode == 0x32) mode = "SDD1-";
  if(mapMode == 0x23 || mapMode == 0x33) mode = "SA1-";
  if(mapMode == 0x25 || mapMode == 0x35) mode = "HIROMEX-";
  if(mapMode == 0x2a || mapMode == 0x3a) mode = "SPC7110-";

  //many games will store an extra title character, overwriting the map mode
  if(!mode) {
    if(headerAddress ==   0x7fb0) mode = "LOROM-";
    if(headerAddress ==   0xffb0) mode = "HIROM-";
    if(headerAddress == 0x40ffb0) mode = "HIROMEX-";
  }

         if(serial() == "A9PJ") {
  //Sufami Turbo (JPN)
    board.append("ST-", mode);
  } else if(serial() == "ZSBJ") {
  //BS-X: Sore wa Namae o Nusumareta Machi no Monogatari (JPN)
    board.append("BS-MCC-");
  } else if(serial() == "042J") {
  //Super Game Boy 2
    board.append("SGB-", mode);
  } else if(serial().match("Z\?\?J")) {
    board.append("BS-", mode);
  } else if(cartridgeTypeLo >= 0x3) {
    if(cartridgeTypeHi == 0x0) board.append("NEC-", mode);
    if(cartridgeTypeHi == 0x1) board.append("SUPERFX-");
    if(cartridgeTypeHi == 0x2) board.append("OBC1-", mode);
    if(cartridgeTypeHi == 0x3) board.append("SA1-");
    if(cartridgeTypeHi == 0x4) board.append("SDD1-");
    if(cartridgeTypeHi == 0x5) board.append("RTC-", mode);
    if(cartridgeTypeHi == 0xe && cartridgeTypeLo == 0x3) board.append("SGB-", mode);
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x00 && cartridgeTypeLo == 0x5) board.append("SPC7110-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x00 && cartridgeTypeLo == 0x9) board.append("SPC7110-RTC-");
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x01) board.append("NECEX-", mode);
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x02) board.append("ARM-", mode);
    if(cartridgeTypeHi == 0xf && cartridgeSubType == 0x10) board.append("HITACHI-", mode);
  }
  if(!board) board.append(mode);

  if(board.beginsWith("LOROM-") && romSize() > 0x200000 && ramSize()) board.replace("LOROM-", "LOROMEX-");
  if(board.beginsWith("NEC-LOROM-") && romSize() > 0x100000) board.replace("NEC-LOROM-", "NEC-LOROMEX-");

  if(cartridgeTypeLo == 0x1 || cartridgeTypeLo == 0x4) board.append("RAM-");  //without battery
  if(cartridgeTypeLo == 0x2 || cartridgeTypeLo == 0x5) board.append("RAM-");  //with battery
  if(cartridgeTypeLo == 0x6) board.append("BATTERY-");  //without RAM

  board.trimRight("-", 1L);

  //NEC uPD96050 frequency
  if(board.beginsWith("NECEX-") && firmwareNECEX() == "st010") board.append("#11");  //11MHz (22MHz / 2)
  if(board.beginsWith("NECEX-") && firmwareNECEX() == "st011") board.append("#15");  //15MHz

  return board;
}

auto SuperFamicom::label() const -> string {
  string label;

  for(uint n = 0; n < 0x15; n++) {
    auto x = data[headerAddress + 0x10 + n];
    auto y = n == 0x14 ? 0 : data[headerAddress + 0x11 + n];

    //null terminator (padding)
    if(x == 0x00 || x == 0xff);

    //ASCII
    else if(x >= 0x20 && x <= 0x7e) label.append((char)x);

    //Shift-JIS (half-width katakana)
    else if(x == 0xa1) label.append("。");
    else if(x == 0xa2) label.append("「");
    else if(x == 0xa3) label.append("」");
    else if(x == 0xa4) label.append("、");
    else if(x == 0xa5) label.append("・");
    else if(x == 0xa6) label.append("ヲ");
    else if(x == 0xa7) label.append("ァ");
    else if(x == 0xa8) label.append("ィ");
    else if(x == 0xa9) label.append("ゥ");
    else if(x == 0xaa) label.append("ェ");
    else if(x == 0xab) label.append("ォ");
    else if(x == 0xac) label.append("ャ");
    else if(x == 0xad) label.append("ュ");
    else if(x == 0xae) label.append("ョ");
    else if(x == 0xaf) label.append("ッ");
    else if(x == 0xb0) label.append("ー");

    else if(x == 0xb1) label.append(                 "ア");
    else if(x == 0xb2) label.append(                 "イ");
    else if(x == 0xb3) label.append(y == 0xde ? "ヴ" : "ウ");
    else if(x == 0xb4) label.append(                 "エ");
    else if(x == 0xb5) label.append(                 "オ");

    else if(x == 0xb6) label.append(y == 0xde ? "ガ" : "カ");
    else if(x == 0xb7) label.append(y == 0xde ? "ギ" : "キ");
    else if(x == 0xb8) label.append(y == 0xde ? "グ" : "ク");
    else if(x == 0xb9) label.append(y == 0xde ? "ゲ" : "ケ");
    else if(x == 0xba) label.append(y == 0xde ? "ゴ" : "コ");

    else if(x == 0xbb) label.append(y == 0xde ? "ザ" : "サ");
    else if(x == 0xbc) label.append(y == 0xde ? "ジ" : "シ");
    else if(x == 0xbd) label.append(y == 0xde ? "ズ" : "ス");
    else if(x == 0xbe) label.append(y == 0xde ? "ゼ" : "セ");
    else if(x == 0xbf) label.append(y == 0xde ? "ゾ" : "ソ");

    else if(x == 0xc0) label.append(y == 0xde ? "ダ" : "タ");
    else if(x == 0xc1) label.append(y == 0xde ? "ヂ" : "チ");
    else if(x == 0xc2) label.append(y == 0xde ? "ヅ" : "ツ");
    else if(x == 0xc3) label.append(y == 0xde ? "デ" : "テ");
    else if(x == 0xc4) label.append(y == 0xde ? "ド" : "ト");

    else if(x == 0xc5) label.append("ナ");
    else if(x == 0xc6) label.append("ニ");
    else if(x == 0xc7) label.append("ヌ");
    else if(x == 0xc8) label.append("ネ");
    else if(x == 0xc9) label.append("ノ");

    else if(x == 0xca) label.append(y == 0xdf ? "パ" : y == 0xde ? "バ" : "ハ");
    else if(x == 0xcb) label.append(y == 0xdf ? "ピ" : y == 0xde ? "ビ" : "ヒ");
    else if(x == 0xcc) label.append(y == 0xdf ? "プ" : y == 0xde ? "ブ" : "フ");
    else if(x == 0xcd) label.append(y == 0xdf ? "ペ" : y == 0xde ? "ベ" : "ヘ");
    else if(x == 0xce) label.append(y == 0xdf ? "ポ" : y == 0xde ? "ボ" : "ホ");

    else if(x == 0xcf) label.append("マ");
    else if(x == 0xd0) label.append("ミ");
    else if(x == 0xd1) label.append("ム");
    else if(x == 0xd2) label.append("メ");
    else if(x == 0xd3) label.append("モ");

    else if(x == 0xd4) label.append("ヤ");
    else if(x == 0xd5) label.append("ユ");
    else if(x == 0xd6) label.append("ヨ");

    else if(x == 0xd7) label.append("ラ");
    else if(x == 0xd8) label.append("リ");
    else if(x == 0xd9) label.append("ル");
    else if(x == 0xda) label.append("レ");
    else if(x == 0xdb) label.append("ロ");

    else if(x == 0xdc) label.append("ワ");
    else if(x == 0xdd) label.append("ン");

    else if(x == 0xde) label.append("\xef\xbe\x9e");  //dakuten
    else if(x == 0xdf) label.append("\xef\xbe\x9f");  //handakuten

    //unknown
    else label.append("?");

    //(han)dakuten skip
    if(y == 0xde && x == 0xb3) n++;
    if(y == 0xde && x >= 0xb6 && x <= 0xc4) n++;
    if(y == 0xde && x >= 0xca && x <= 0xce) n++;
    if(y == 0xdf && x >= 0xca && y <= 0xce) n++;
  }

  return label.strip();
}

auto SuperFamicom::serial() const -> string {
  char A = data[headerAddress + 0x02];  //game type
  char B = data[headerAddress + 0x03];  //game code
  char C = data[headerAddress + 0x04];  //game code
  char D = data[headerAddress + 0x05];  //region code (new; sometimes ambiguous)

  auto valid = [](char n) { return (n >= '0' && n <= '9') || (n >= 'A' && n <= 'Z'); };
  if(data[headerAddress + 0x2a] == 0x33 && valid(A) && valid(B) & valid(C) & valid(D)) {
    return {A, B, C, D};
  }

  return "";
}

auto SuperFamicom::romSize() const -> uint {
  //subtract appended firmware size, if firmware is present
  if((size() &  0x7fff) ==   0x100) return size() -   0x100;
  if((size() &  0x7fff) ==   0xc00) return size() -   0xc00;
  if((size() &  0x7fff) ==  0x2000) return size() -  0x2000;
  if((size() &  0xffff) ==  0xd000) return size() -  0xd000;
  if((size() & 0x3ffff) == 0x28000) return size() - 0x28000;
  return size();

//auto romSize = data[headerAddress + 0x27] & 15;
//return 1024 << romSize;
}

auto SuperFamicom::ramSize() const -> uint {
  auto ramSize = data[headerAddress + 0x28] & 7;
  if(ramSize) return 1024 << ramSize;
  return 0;
}

auto SuperFamicom::expansionRamSize() const -> uint {
  if(data[headerAddress + 0x2a] != 0x33) return 0;
  auto ramSize = data[headerAddress + 0x0d] & 7;
  if(ramSize) return 1024 << ramSize;
  return 0;
}

auto SuperFamicom::battery() const -> bool {
  auto cartridgeTypeLo = data[headerAddress + 0x26] & 15;
  return cartridgeTypeLo == 0x2 || cartridgeTypeLo == 0x5 || cartridgeTypeLo == 0x6;
}

auto SuperFamicom::scoreHeader(uint address) -> uint {
  int score = 0;
  if(size() < address + 0x50) return score;

  uint8_t  mapMode     = data[address + 0x25] & ~0x10;  //ignore FastROM bit
  uint16_t complement  = data[address + 0x2c] << 0 | data[address + 0x2d] << 8;
  uint16_t checksum    = data[address + 0x2e] << 0 | data[address + 0x2f] << 8;
  uint16_t resetVector = data[address + 0x4c] << 0 | data[address + 0x4d] << 8;
  if(resetVector < 0x8000) return score;  //$00:0000-7fff is never ROM data

  uint8_t opcode = data[(address & ~0x7fff) | (resetVector & 0x7fff)];  //first instruction executed

  //most likely opcodes
  if(opcode == 0x78  //sei
  || opcode == 0x18  //clc (clc; xce)
  || opcode == 0x38  //sec (sec; xce)
  || opcode == 0x9c  //stz $nnnn (stz $4200)
  || opcode == 0x4c  //jmp $nnnn
  || opcode == 0x5c  //jml $nnnnnn
  ) score += 8;

  //plausible opcodes
  if(opcode == 0xc2  //rep #$nn
  || opcode == 0xe2  //sep #$nn
  || opcode == 0xad  //lda $nnnn
  || opcode == 0xae  //ldx $nnnn
  || opcode == 0xac  //ldy $nnnn
  || opcode == 0xaf  //lda $nnnnnn
  || opcode == 0xa9  //lda #$nn
  || opcode == 0xa2  //ldx #$nn
  || opcode == 0xa0  //ldy #$nn
  || opcode == 0x20  //jsr $nnnn
  || opcode == 0x22  //jsl $nnnnnn
  ) score += 4;

  //implausible opcodes
  if(opcode == 0x40  //rti
  || opcode == 0x60  //rts
  || opcode == 0x6b  //rtl
  || opcode == 0xcd  //cmp $nnnn
  || opcode == 0xec  //cpx $nnnn
  || opcode == 0xcc  //cpy $nnnn
  ) score -= 4;

  //least likely opcodes
  if(opcode == 0x00  //brk #$nn
  || opcode == 0x02  //cop #$nn
  || opcode == 0xdb  //stp
  || opcode == 0x42  //wdm
  || opcode == 0xff  //sbc $nnnnnn,x
  ) score -= 8;

  if(checksum + complement == 0xffff) score += 4;

  if(address == 0x7fb0 && mapMode == 0x20) score += 2;
  if(address == 0xffb0 && mapMode == 0x21) score += 2;

  return max(0, score);
}

auto SuperFamicom::firmwareARM() const -> string {
  return "st018";
}

auto SuperFamicom::firmwareHITACHI() const -> string {
  return "cx4";
}

auto SuperFamicom::firmwareNEC() const -> string {
  if(label() == "PILOTWINGS") return "dsp1";
  if(label() == "DUNGEON MASTER") return "dsp2";
  if(label() == "SDガンダムGX") return "dsp3";
  if(label() == "PLANETS CHAMP TG3000") return "dsp4";
  if(label() == "TOP GEAR 3000") return "dsp4";
  return "dsp1b";
}

auto SuperFamicom::firmwareNECEX() const -> string {
  if(label() == "EXHAUST HEAT2") return "st010";
  if(label() == "F1 ROC II") return "st010";
  if(label() == "2DAN MORITA SHOUGI") return "st011";
  return "st010";
}

auto SuperFamicom::firmwareSGB() const -> string {
  if(label() == "Super GAMEBOY") return "sgb1";
  if(label() == "Super GAMEBOY2") return "sgb2";
  return "sgb1";
}

}
