auto APU::read(uint32 addr) -> uint8 {
  switch(addr) {

  //NR10
  case 0x0400'0060: return square1.read(0);
  case 0x0400'0061: return 0;

  //NR11, NR12
  case 0x0400'0062: return square1.read(1);
  case 0x0400'0063: return square1.read(2);

  //NR13, NR14
  case 0x0400'0064: return square1.read(3);
  case 0x0400'0065: return square1.read(4);

  //NR21, NR22
  case 0x0400'0068: return square2.read(1);
  case 0x0400'0069: return square2.read(2);

  //NR23, NR24
  case 0x0400'006c: return square2.read(3);
  case 0x0400'006d: return square2.read(4);

  //NR30
  case 0x0400'0070: return wave.read(0);
  case 0x0400'0071: return 0;

  //NR31, NR32
  case 0x0400'0072: return wave.read(1);
  case 0x0400'0073: return wave.read(2);

  //NR33, NR34
  case 0x0400'0074: return wave.read(3);
  case 0x0400'0075: return wave.read(4);

  //NR41, NR42
  case 0x0400'0078: return noise.read(1);
  case 0x0400'0079: return noise.read(2);

  //NR43, NR44
  case 0x0400'007c: return noise.read(3);
  case 0x0400'007d: return noise.read(4);

  //NR50, NR51
  case 0x0400'0080: return sequencer.read(0);
  case 0x0400'0081: return sequencer.read(1);

  //SOUND_CNT_H
  case 0x0400'0082: return (
    sequencer.volume << 0
  | fifo[0].volume   << 2
  | fifo[1].volume   << 3
  );

  case 0x0400'0083: return (
    fifo[0].renable << 0
  | fifo[0].lenable << 1
  | fifo[0].timer   << 2
  | fifo[1].renable << 4
  | fifo[1].lenable << 5
  | fifo[1].timer   << 6
  );

  //NR52
  case 0x0400'0084: return sequencer.read(2);
  case 0x0400'0085: return 0;

  //SOUNDBIAS
  case 0x0400'0088: return (
    regs.bias.level.bits(0,7)
  );
  case 0x0400'0089: return (
    regs.bias.level.bits(8,9) << 0
  | regs.bias.amplitude       << 6
  );

  //WAVE_RAM0_L
  case 0x0400'0090: return wave.readram( 0);
  case 0x0400'0091: return wave.readram( 1);

  //WAVE_RAM0_H
  case 0x0400'0092: return wave.readram( 2);
  case 0x0400'0093: return wave.readram( 3);

  //WAVE_RAM1_L
  case 0x0400'0094: return wave.readram( 4);
  case 0x0400'0095: return wave.readram( 5);

  //WAVE_RAM1_H
  case 0x0400'0096: return wave.readram( 6);
  case 0x0400'0097: return wave.readram( 7);

  //WAVE_RAM2_L
  case 0x0400'0098: return wave.readram( 8);
  case 0x0400'0099: return wave.readram( 9);

  //WAVE_RAM2_H
  case 0x0400'009a: return wave.readram(10);
  case 0x0400'009b: return wave.readram(11);

  //WAVE_RAM3_L
  case 0x0400'009c: return wave.readram(12);
  case 0x0400'009d: return wave.readram(13);

  //WAVE_RAM3_H
  case 0x0400'009e: return wave.readram(14);
  case 0x0400'009f: return wave.readram(15);

  }

  return 0;
}

auto APU::write(uint32 addr, uint8 data) -> void {
  switch(addr) {

  //NR10
  case 0x0400'0060: return square1.write(0, data);
  case 0x0400'0061: return;

  //NR11, NR12
  case 0x0400'0062: return square1.write(1, data);
  case 0x0400'0063: return square1.write(2, data);

  //NR13, NR14
  case 0x0400'0064: return square1.write(3, data);
  case 0x0400'0065: return square1.write(4, data);

  //NR21, NR22
  case 0x0400'0068: return square2.write(1, data);
  case 0x0400'0069: return square2.write(2, data);

  //NR23, NR24
  case 0x0400'006c: return square2.write(3, data);
  case 0x0400'006d: return square2.write(4, data);

  //NR30
  case 0x0400'0070: return wave.write(0, data);
  case 0x0400'0071: return;

  //NR31, NR32
  case 0x0400'0072: return wave.write(1, data);
  case 0x0400'0073: return wave.write(2, data);

  //NR33, NR34
  case 0x0400'0074: return wave.write(3, data);
  case 0x0400'0075: return wave.write(4, data);

  //NR41, NR42
  case 0x0400'0078: return noise.write(1, data);
  case 0x0400'0079: return noise.write(2, data);

  //NR43, NR44
  case 0x0400'007c: return noise.write(3, data);
  case 0x0400'007d: return noise.write(4, data);

  //NR50, NR51
  case 0x0400'0080: return sequencer.write(0, data);
  case 0x0400'0081: return sequencer.write(1, data);

  //SOUND_CNT_H
  case 0x0400'0082:
    sequencer.volume = data.bits(0,1);
    fifo[0].volume   = data.bit (2);
    fifo[1].volume   = data.bit (3);
    return;
  case 0x0400'0083:
    fifo[0].renable = data.bit(0);
    fifo[0].lenable = data.bit(1);
    fifo[0].timer   = data.bit(2);
    if(data.bit(3)) fifo[0].reset();
    fifo[1].renable = data.bit(4);
    fifo[1].lenable = data.bit(5);
    fifo[1].timer   = data.bit(6);
    if(data.bit(7)) fifo[1].reset();
    return;

  //NR52
  case 0x0400'0084: return sequencer.write(2, data);
  case 0x0400'0085: return;

  //SOUNDBIAS
  case 0x0400'0088:
    regs.bias.level.bits(0,7) = data;
    return;
  case 0x0400'0089:
    regs.bias.level.bits(8,9) = data.bits(0,1);
    regs.bias.amplitude       = data.bits(6,7);
    return;

  //WAVE_RAM0_L
  case 0x0400'0090: return wave.writeram( 0, data);
  case 0x0400'0091: return wave.writeram( 1, data);

  //WAVE_RAM0_H
  case 0x0400'0092: return wave.writeram( 2, data);
  case 0x0400'0093: return wave.writeram( 3, data);

  //WAVE_RAM1_L
  case 0x0400'0094: return wave.writeram( 4, data);
  case 0x0400'0095: return wave.writeram( 5, data);

  //WAVE_RAM1_H
  case 0x0400'0096: return wave.writeram( 6, data);
  case 0x0400'0097: return wave.writeram( 7, data);

  //WAVE_RAM2_L
  case 0x0400'0098: return wave.writeram( 8, data);
  case 0x0400'0099: return wave.writeram( 9, data);

  //WAVE_RAM2_H
  case 0x0400'009a: return wave.writeram(10, data);
  case 0x0400'009b: return wave.writeram(11, data);

  //WAVE_RAM3_L
  case 0x0400'009c: return wave.writeram(12, data);
  case 0x0400'009d: return wave.writeram(13, data);

  //WAVE_RAM3_H
  case 0x0400'009e: return wave.writeram(14, data);
  case 0x0400'009f: return wave.writeram(15, data);

  //FIFO_A_L
  //FIFO_A_H
  case 0x0400'00a0: case 0x0400'00a1:
  case 0x0400'00a2: case 0x0400'00a3:
    return fifo[0].write(data);

  //FIFO_B_L
  //FIFO_B_H
  case 0x0400'00a4: case 0x0400'00a5:
  case 0x0400'00a6: case 0x0400'00a7:
    return fifo[1].write(data);
  }
}
