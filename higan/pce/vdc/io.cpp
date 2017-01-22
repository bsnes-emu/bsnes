auto VDC::read(uint11 addr) -> uint8 {
  bool a0 = addr.bit(0);
  if(!addr.bit(10)) {
    if(addr.bit(1) == 0) {
      //SR
      if(a0) return 0x00;
      uint8 data;
      data.bit(0) = irq.pendingCollision;
      data.bit(1) = irq.pendingOverflow;
      data.bit(2) = irq.pendingLineCoincidence;
      data.bit(3) = irq.pendingTransferSATB;
      data.bit(4) = irq.pendingTransferVRAM;
      data.bit(5) = irq.pendingVblank;
      irq.lower();
      return data;
    }

    if(addr.bit(1) == 1) {
      if(io.address == 0x02) {
        //VRR
        uint8 data = vram.dataRead.byte(a0);
        if(a0) {
          vram.addressRead += vram.addressIncrement;
          vram.dataRead = vram.read(vram.addressRead);
        }
        return data;
      }
    }
  } else {
    if(addr.bits(0,2) == 0x04) {
      //CTR
      uint8 data = cram.read(cram.address).bits(0,7);
      return data;
    }

    if(addr.bits(0,2) == 0x05) {
      //CTR
      uint8 data = 0xfe | cram.read(cram.address).bit(0);
      cram.address++;
      return data;
    }
  }

  return 0x00;
}

auto VDC::write(uint11 addr, uint8 data) -> void {
  bool a0 = addr.bit(0);
  if(!addr.bit(10)) {
    if(addr.bit(1) == 0) {
      //AR
      if(a0) return;
      io.address = data.bits(0,4);
      return;
    }

    if(addr.bit(1) == 1) {
      if(io.address == 0x00) {
        //MAWR
        vram.addressWrite.byte(a0) = data;
        return;
      }

      if(io.address == 0x01) {
        //MARR
        vram.addressRead.byte(a0) = data;
        vram.dataRead = vram.read(vram.addressRead);
        return;
      }

      if(io.address == 0x02) {
        //VWR
        vram.dataWrite.byte(a0) = data;
        if(a0) {
          vram.write(vram.addressWrite, vram.dataWrite);
          vram.addressWrite += vram.addressIncrement;
        }
        return;
      }

      if(io.address == 0x05) {
        //CR
        if(!a0) {
          irq.enableCollision = data.bit(0);
          irq.enableOverflow = data.bit(1);
          irq.enableLineCoincidence = data.bit(2);
          irq.enableVblank = data.bit(3);
          io.externalSync = data.bits(4,5);
          sprite.enable = data.bit(6);
          background.enable = data.bit(7);
        } else {
          io.displayOutput = data.bits(0,1);
          io.dramRefresh = data.bit(2);
          if(data.bits(3,4) == 0) vram.addressIncrement = 0x01;
          if(data.bits(3,4) == 1) vram.addressIncrement = 0x20;
          if(data.bits(3,4) == 2) vram.addressIncrement = 0x40;
          if(data.bits(3,4) == 3) vram.addressIncrement = 0x80;
        }
        return;
      }

      if(io.address == 0x06) {
        //RCR
        io.lineCoincidence.byte(a0) = data;
        return;
      }

      if(io.address == 0x07) {
        //BXR
        background.hscroll.byte(a0) = data;
        return;
      }

      if(io.address == 0x08) {
        //BYR
        background.vscroll.byte(a0) = data;
        background.vcounter = background.vscroll;
        return;
      }

      if(io.address == 0x09) {
        //MWR
        if(a0) return;
        io.vramAccess = data.bits(0,1);
        io.spriteAccess = data.bits(2,3);
        if(data.bits(4,5) == 0) background.width =  32;
        if(data.bits(4,5) == 1) background.width =  64;
        if(data.bits(4,5) == 2) background.width = 128;
        if(data.bits(4,5) == 3) background.width = 128;
        if(data.bit(6) == 0) background.height = 32;
        if(data.bit(6) == 1) background.height = 64;
        io.cgMode = data.bit(7);
        return;
      }

      if(io.address == 0x0a) {
        //HSR
        if(!a0) {
          vce.horizontalSyncWidth = data.bits(0,4);
        } else {
          vce.horizontalDisplayStart = data.bits(0,6);
        }
        return;
      }

      if(io.address == 0x0b) {
        //HDR
        if(!a0) {
          vce.horizontalDisplayLength = data.bits(0,6);
        } else {
          vce.horizontalDisplayEnd = data.bits(0,6);
        }
        return;
      }

      if(io.address == 0x0c) {
        //VPR
        if(!a0) {
          vce.verticalSyncWidth = data.bits(0,4);
        } else {
          vce.verticalDisplayStart = data.bits(0,7);
        }
        return;
      }

      if(io.address == 0x0d) {
        //VDR
        if(!a0) {
          vce.verticalDisplayLength.bits(0,7) = data.bits(0,7);
        } else {
          vce.verticalDisplayLength.bit(8) = data.bit(0);
        }
        return;
      }

      if(io.address == 0x0e) {
        //VCR
        if(a0) return;
        vce.verticalDisplayEnd = data.bits(0,7);
        return;
      }

      if(io.address == 0x0f) {
        //DCR
        if(a0) return;
        irq.enableTransferVRAM = data.bit(0);
        irq.enableTransferSATB = data.bit(1);
        dma.sourceIncrementMode = data.bit(2);
        dma.targetIncrementMode = data.bit(3);
        dma.satbRepeat = data.bit(4);
        return;
      }

      if(io.address == 0x10) {
        //SOUR
        dma.source.byte(a0) = data;
        return;
      }

      if(io.address == 0x11) {
        //DESR
        dma.target.byte(a0) = data;
        return;
      }

      if(io.address == 0x12) {
        //LENR
        dma.length.byte(a0) = data;
        if(a0) dma.vramStart();
        return;
      }

      if(io.address == 0x13) {
        //DVSSR
        dma.satbSource.byte(a0) = data;
        if(a0) dma.satbQueue();
        return;
      }
    }
  } else {
    if(addr.bits(0,2) == 0x00) {
      //CR
      if(data.bits(0,1) == 0) vce.clock = 4;
      if(data.bits(0,1) == 1) vce.clock = 3;
      if(data.bits(0,1) == 2) vce.clock = 2;
      if(data.bits(0,1) == 3) vce.clock = 2;
      io.colorBlur = data.bit(2);
      io.grayscale = data.bit(7);
      return;
    }

    if(addr.bits(0,2) == 0x02) {
      //CTA
      cram.address.bits(0,7) = data.bits(0,7);
      return;
    }

    if(addr.bits(0,2) == 0x03) {
      //CTA
      cram.address.bit(8) = data.bit(0);
      return;
    }

    if(addr.bits(0,2) == 0x04) {
      //CTW
      cram.write(cram.address, 0, data.bits(0,7));
      return;
    }

    if(addr.bits(0,2) == 0x05) {
      //CTW
      cram.write(cram.address, 1, data.bit(0));
      cram.address++;
      return;
    }
  }
}
