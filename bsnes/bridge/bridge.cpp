#include "../base.h"
#include "bridge.h"
port_bridge *cpu_apu_bridge;

port_bridge::port_bridge() {
  cputoapu_port[0] = cputoapu_port[1] =
  cputoapu_port[2] = cputoapu_port[3] =
  aputocpu_port[0] = aputocpu_port[1] =
  aputocpu_port[2] = aputocpu_port[3] = 0;
}

byte port_bridge::cpu_read(byte port) {
  return aputocpu_port[port & 3];
}

byte port_bridge::apu_read(byte port) {
  return cputoapu_port[port & 3];
}

void port_bridge::cpu_write(byte port, byte value) {
  cputoapu_port[port & 3] = value;
}

void port_bridge::apu_write(byte port, byte value) {
  aputocpu_port[port & 3] = value;
}
