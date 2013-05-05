#include <sfc/sfc.hpp>

#define HSU1_CPP
namespace SuperFamicom {

#include "serialization.cpp"
HSU1 hsu1;

void HSU1::init() {
}

void HSU1::load() {
}

void HSU1::unload() {
}

void HSU1::power() {
}

void HSU1::reset() {
  txbusy = 0;
  rxbusy = 1;
  txlatch = 0;
  txbuffer.reset();
  rxbuffer.reset();
}

uint8 HSU1::read(unsigned addr) {
  addr &= 1;

  if(addr == 0) {
    return (txbusy << 7) | (rxbusy << 6) | (1 << 0);
  }

  if(addr == 1) {
    if(rxbusy) return 0x00;
    uint8 data = rxbuffer.take(0);
    if(rxbuffer.size() == 0) rxbusy = 1;
    return data;
  }

  //unreachable
  return cpu.regs.mdr;
}

void HSU1::write(unsigned addr, uint8 data) {
  addr &= 1;

  if(addr == 0) {
    if(txbusy) return;
    bool latch = data & 0x01;
    if(txlatch == 1 && latch == 0) {
      //username:password@http://server:port/path
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
        string header {
          "username:", username, "\n",
          "password:", password, "\n",
          "emulator:bsnes\n",
          "sha256:", interface->sha256(), "\n",
          "\n"
        };

        string packet {
          "POST /", hostpath, " HTTP/1.0\r\n",
          "Host: ", hostname, "\r\n",
          "Connection: close\r\n",
          "Content-Type: application/octet-stream\r\n",
          "Content-Length: ", header.length() + txbuffer.size(), "\r\n",
          "\r\n",
        };

        server.send(packet);
        server.send(header);
        server.send(txbuffer.data(), txbuffer.size());
        txbuffer.reset();

        server.header = server.downloadHeader();
        uint8_t* data = nullptr;
        unsigned size = 0;
        server.downloadContent(data, size);
        rxbuffer.resize(size);
        memcpy(rxbuffer.data(), data, size);
        rxbusy = rxbuffer.size() == 0;
        free(data);

        server.disconnect();
      }
    }
    txlatch = latch;
  }

  if(addr == 1) {
    if(txbusy) return;
    if(txlatch == 0) return;
    txbuffer.append(data);
  }
}

}
