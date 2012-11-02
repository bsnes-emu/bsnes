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
  packetlength = 0;
}

uint8 HSU1::read(unsigned addr) {
  return cpu.regs.mdr;
}

void HSU1::write(unsigned addr, uint8 data) {
  packet[packetlength] = data;
  if(packetlength < sizeof(packet)) packetlength++;
  if(data) return;

  lstring side = interface->scoreServer().split<1>("@");
  string username = side(0).split<1>(":")(0);
  string password = side(0).split<1>(":")(1);
  string servername = side(1).ltrim<1>("http://");
  side = servername.split<1>("/");
  string host = side(0).split<1>(":")(0);
  string port = side(0).split<1>(":")(1);
  string path = side(1);
  if(port.empty()) port = "80";

  http server;
  if(server.connect(host, decimal(port))) {
    string content = {
      "username:", username, "\r\n",
      "password:", password, "\r\n",
      "sha256:", interface->sha256(), "\r\n",
      "data:", (const char*)packet, "\r\n",
      "emulator:bsnes\r\n"
    };
    string packet = {
      "POST ", path, " HTTP/1.0\r\n",
      "Host: ", host, "\r\n",
      "Connection: close\r\n",
      "Content-Type: text/plain; charset=utf-8\r\n",
      "Content-Length: ", content.length(), "\r\n",
      "\r\n",
      content
    };
    server.send(packet);
  }
}

}
