#ifdef CPU_CPP

bool CPUDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  //internal
  if(id == n++) { name = "S-CPU MDR"; value = string("0x", strhex<2>(mdr())); return true; }

  //$2181-2183
  if(id == n++) { name = "$2181-$2183"; value = ""; return true; }
  if(id == n++) { name = "WRAM Address"; value = string("0x", strhex<6>(wram_address())); return true; }

  //$4016
  if(id == n++) { name = "$4016"; value = ""; return true; }
  if(id == n++) { name = "Joypad Strobe Latch"; value = joypad_strobe_latch(); return true; }

  //$4200
  if(id == n++) { name = "$4200"; value = ""; return true; }
  if(id == n++) { name = "NMI Enable"; value = nmi_enable(); return true; }
  if(id == n++) { name = "H-IRQ Enable"; value = hirq_enable(); return true; }
  if(id == n++) { name = "V-IRQ Enable"; value = virq_enable(); return true; }
  if(id == n++) { name = "Auto Joypad Poll"; value = auto_joypad_poll(); return true; }

  //$4201
  if(id == n++) { name = "$4201"; value = ""; return true; }
  if(id == n++) { name = "PIO"; value = string("0x", strhex<2>(pio_bits())); return true; }

  //$4202
  if(id == n++) { name = "$4202"; value = ""; return true; }
  if(id == n++) { name = "Multiplicand"; value = string("0x", strhex<2>(multiplicand())); return true; }

  //$4203
  if(id == n++) { name = "$4203"; value = ""; return true; }
  if(id == n++) { name = "Multiplier"; value = string("0x", strhex<2>(multiplier())); return true; }

  //$4204-$4205
  if(id == n++) { name = "$4204-$4205"; value = ""; return true; }
  if(id == n++) { name = "Dividend"; value = string("0x", strhex<4>(dividend())); return true; }

  //$4206
  if(id == n++) { name = "$4206"; value = ""; return true; }
  if(id == n++) { name = "Divisor"; value = string("0x", strhex<2>(divisor())); return true; }

  //$4207-$4208
  if(id == n++) { name = "$4207-$4208"; value = ""; return true; }
  if(id == n++) { name = "H-Time"; value = string("0x", strhex<4>(htime())); return true; }

  //$4209-$420a
  if(id == n++) { name = "$4209-$420a"; value = ""; return true; }
  if(id == n++) { name = "V-Time"; value = string("0x", strhex<4>(vtime())); return true; }

  //$420b
  if(id == n++) { name = "$420b"; value = ""; return true; }
  if(id == n++) { name = "DMA Enable"; value = string("0x", strhex<2>(dma_enable())); return true; }

  //$420c
  if(id == n++) { name = "$420c"; value = ""; return true; }
  if(id == n++) { name = "HDMA Enable"; value = string("0x", strhex<2>(hdma_enable())); return true; }

  //$420d
  if(id == n++) { name = "$420d"; value = ""; return true; }
  if(id == n++) { name = "FastROM Enable"; value = fastrom_enable(); return true; }

  for(unsigned i = 0; i < 8; i++) {
    if(id == n++) { name = string() << "DMA Channel " << i; return true; }

    //$43x0
    if(id == n++) { name = "Direction"; value = dma_direction(i); return true; }
    if(id == n++) { name = "Indirect"; value = dma_indirect(i); return true; }
    if(id == n++) { name = "Reverse Transfer"; value = dma_reverse_transfer(i); return true; }
    if(id == n++) { name = "Fixed Transfer"; value = dma_fixed_transfer(i); return true; }
    if(id == n++) { name = "Transfer Mode"; value = dma_transfer_mode(i); return true; }

    //$43x1
    if(id == n++) { name = "B-Bus Address"; value = string("0x", strhex<4>(dma_bbus_address(i))); return true; }

    //$43x2-$43x3
    if(id == n++) { name = "A-Bus Address"; value = string("0x", strhex<4>(dma_abus_address(i))); return true; }

    //$43x4
    if(id == n++) { name = "A-Bus Bank"; value = string("0x", strhex<2>(dma_abus_bank(i))); return true; }

    //$43x5-$43x6
    if(id == n++) { name = "Transfer Size / Indirect Address"; value = string("0x", strhex<4>(dma_transfer_size(i))); return true; }

    //$43x7
    if(id == n++) { name = "Indirect Bank"; value = string("0x", strhex<2>(dma_indirect_bank(i))); return true; }

    //$43x8-$43x9
    if(id == n++) { name = "Table Address"; value = string("0x", strhex<4>(dma_table_address(i))); return true; }

    //$43xa
    if(id == n++) { name = "Line Counter"; value = string("0x", strhex<2>(dma_line_counter(i))); return true; }
  }

  return false;
}

#endif
