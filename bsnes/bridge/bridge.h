class port_bridge {
public:
  byte cputoapu_port[4]; //holds values written to CPU at $2140-$2143
  byte aputocpu_port[4]; //holds values written to APU at $f4-$f7

  port_bridge();
  byte cpu_read(byte port);
  byte apu_read(byte port);
  void cpu_write(byte port, byte value);
  void apu_write(byte port, byte value);
};
