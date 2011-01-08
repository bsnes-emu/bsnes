uint8 r2181;
uint8 r2182;
MMIO *mmio[3];
uint8 mmio_read(unsigned addr);
void mmio_write(unsigned addr, uint8 data);

uint8 r6000;
uint8 r6003;
uint8 r6004;
uint8 r6005;
uint8 r6006;
uint8 r6007;
uint8 r7000[16];
unsigned r7800;
uint8 mlt_req;
uint8 read(unsigned addr);
void write(unsigned addr, uint8 data);

uint8 vram[320];
