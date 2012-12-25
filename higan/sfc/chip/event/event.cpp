#include <sfc/sfc.hpp>

#define EVENT_CPP
namespace SuperFamicom {

Event event;

void Event::Enter() { event.enter(); }

void Event::enter() {
  while(true) {
    if(scheduler.sync == Scheduler::SynchronizeMode::All) {
      scheduler.exit(Scheduler::ExitReason::SynchronizeEvent);
    }

    if(scoreActive && scoreSecondsRemaining) {
      if(--scoreSecondsRemaining == 0) {
        scoreActive = false;
        submitScore();
      }
    }

    if(timerActive && timerSecondsRemaining) {
      if(--timerSecondsRemaining == 0) {
        timerActive = false;
        status |= 0x02;  //time over
        scoreActive = true;
        scoreSecondsRemaining = 5;
      }
    }

    step(1);
    synchronize_cpu();
  }
}

void Event::submitScore() {
  if(usedSaveState) return;

  string data;
  data.append("timer=", timer, ";");
  if(board == Board::CampusChallenge92) {
    unsigned mw = 0, fz = 0, pw = 0;
    for(unsigned n = 0x0408; n <= 0x040e; n++) mw = mw * 10 + ram.read(n);
    for(unsigned n = 0x0413; n >= 0x0410; n--) fz = fz * 10 + ram.read(n);
    for(unsigned n = 0x0418; n >= 0x0415; n--) pw = pw * 10 + ram.read(n);
    data.append("mw=", mw, ";");
    data.append("fz=", fz, ";");
    data.append("pw=", pw);
  }
  if(board == Board::Powerfest94) {
    unsigned ml = 0, mk[2] = {0}, ba[2] = {0};
    for(unsigned n = 0x0408; n <= 0x040e; n++) ml = ml * 10 + ram.read(n);
    for(unsigned n = 0x0413; n >= 0x0412; n--) mk[0] = mk[0] * 10 + ram.read(n);
    for(unsigned n = 0x0411; n >= 0x0410; n--) mk[1] = mk[1] * 10 + ram.read(n);
    for(unsigned n = 0x0418; n >= 0x0415; n--) ba[0] = ba[0] * 10 + ram.read(n);
    for(unsigned n = 0x041a; n >= 0x0419; n--) ba[1] = ba[1] * 10 + ram.read(n);
    data.append("ml=", ml, ";");
    data.append("mk=", mk[0], ",", mk[1], ";");
    data.append("ba=", ba[0], ",", ba[1]);
  }

  lstring side = interface->server().split<1>("@");
  string username = side(0).split<1>(":")(0);
  string password = side(0).split<1>(":")(1);
  side(1).ltrim<1>("http://");
  string hostname = side(1).split<1>("/")(0);
  string hostpath = side(1).split<1>("/")(1);
  side = hostname.split<1>(":");
  hostname = side(0);
  string hostport = side(1);
  if(hostport.empty()) hostport = "80";

  http server;
  if(server.connect(hostname, decimal(hostport))) {
    string content = {
      "username:", username, "\n",
      "password:", password, "\n",
      "emulator:bsnes\n",
      "sha256:", interface->sha256(), "\n",
      "\n",
      data
    };
    string packet = {
      "POST /", hostpath, " HTTP/1.0\r\n",
      "Host: ", hostname, "\r\n",
      "Connection: close\r\n",
      "Content-Type: application/octet-stream\r\n",
      "Content-Length: ", content.length(), "\r\n",
      "\r\n",
      content
    };
    server.send(packet);
    server.disconnect();
  }
}

void Event::init() {
}

void Event::load() {
}

void Event::unload() {
  rom[0].reset();
  rom[1].reset();
  rom[2].reset();
  rom[3].reset();
  ram.reset();
}

void Event::power() {
  usedSaveState = false;
}

void Event::reset() {
  create(Event::Enter, 1);
  for(unsigned n = 0; n < ram.size(); n++) ram.write(n, 0x00);
  status = 0x00;
  select = 0x00;
  timerActive = false;
  scoreActive = false;
  timerSecondsRemaining = 0;
  scoreSecondsRemaining = 0;
  usedSaveState = false;
}

uint8 Event::sr(unsigned) {
  return status;
}

void Event::dr(unsigned, uint8 data) {
  select = data;
  if(timer && data == 0x09) {
    timerActive = true;
    timerSecondsRemaining = timer;
  }
}

uint8 Event::rom_read(unsigned addr) {
  if(board == Board::CampusChallenge92) {
    unsigned id = 0;
    if(select == 0x09) id = 1;
    if(select == 0x05) id = 2;
    if(select == 0x03) id = 3;
    if((addr & 0x808000) == 0x808000) id = 0;

    if(addr & 0x008000) {
      addr = ((addr & 0x7f0000) >> 1) | (addr & 0x7fff);
      return rom[id].read(bus.mirror(addr, rom[id].size()));
    }
  }

  if(board == Board::Powerfest94) {
    unsigned id = 0;
    if(select == 0x09) id = 1;
    if(select == 0x0c) id = 2;
    if(select == 0x0a) id = 3;
    if((addr & 0x208000) == 0x208000) id = 0;

    if(addr & 0x400000) {
      addr &= 0x3fffff;
      return rom[id].read(bus.mirror(addr, rom[id].size()));
    }

    if(addr & 0x008000) {
      addr &= 0x1fffff;
      if(id != 2) addr = ((addr & 0x1f0000) >> 1) | (addr & 0x7fff);
      return rom[id].read(bus.mirror(addr, rom[id].size()));
    }
  }

  return cpu.regs.mdr;
}

uint8 Event::ram_read(unsigned addr) {
  return ram.read(bus.mirror(addr, ram.size()));
}

void Event::ram_write(unsigned addr, uint8 data) {
  return ram.write(bus.mirror(addr, ram.size()), data);
}

void Event::serialize(serializer &s) {
  Thread::serialize(s);
  s.array(ram.data(), ram.size());
  s.integer(status);
  s.integer(select);
  s.integer(timerActive);
  s.integer(scoreActive);
  s.integer(timerSecondsRemaining);
  s.integer(scoreSecondsRemaining);

  usedSaveState = true;
}

}
