/*
  $21c2/$21c3

  These seem to be version information registers... I don't know
  what their purpose is, but I do know that the SNES demo rom
  expects these values to be returned in order to proceed through
  the character test.
*/
byte mmio_r21c2(void) {
  return 0x20;
}

byte mmio_r21c3(void) {
  return 0x00;
}
