auto VDC::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(vram.data);
  s.integer(vram.addressRead);
  s.integer(vram.addressWrite);
  s.integer(vram.addressIncrement);
  s.integer(vram.dataRead);
  s.integer(vram.dataWrite);

  s.array(satb.data);

  s.integer(timing.horizontalSyncWidth);
  s.integer(timing.horizontalDisplayStart);
  s.integer(timing.horizontalDisplayLength);
  s.integer(timing.horizontalDisplayEnd);
  s.integer(timing.verticalSyncWidth);
  s.integer(timing.verticalDisplayStart);
  s.integer(timing.verticalDisplayLength);
  s.integer(timing.verticalDisplayEnd);
  s.integer(timing.vpulse);
  s.integer(timing.hpulse);
  s.integer(timing.hclock);
  s.integer(timing.vclock);
  s.integer(timing.hoffset);
  s.integer(timing.voffset);
  s.integer(timing.hstart);
  s.integer(timing.vstart);
  s.integer(timing.hlength);
  s.integer(timing.vlength);

  s.integer(irq.enableCollision);
  s.integer(irq.enableOverflow);
  s.integer(irq.enableLineCoincidence);
  s.integer(irq.enableVblank);
  s.integer(irq.enableTransferVRAM);
  s.integer(irq.enableTransferSATB);
  s.integer(irq.pendingCollision);
  s.integer(irq.pendingOverflow);
  s.integer(irq.pendingLineCoincidence);
  s.integer(irq.pendingVblank);
  s.integer(irq.pendingTransferVRAM);
  s.integer(irq.pendingTransferSATB);
  s.integer(irq.line);

  s.integer(dma.sourceIncrementMode);
  s.integer(dma.targetIncrementMode);
  s.integer(dma.satbRepeat);
  s.integer(dma.source);
  s.integer(dma.target);
  s.integer(dma.length);
  s.integer(dma.satbSource);
  s.integer(dma.vramActive);
  s.integer(dma.satbActive);
  s.integer(dma.satbPending);
  s.integer(dma.satbOffset);

  s.integer(background.enable);
  s.integer(background.hscroll);
  s.integer(background.vscroll);
  s.integer(background.vcounter);
  s.integer(background.width);
  s.integer(background.height);
  s.integer(background.hoffset);
  s.integer(background.voffset);
  s.integer(background.color);
  s.integer(background.palette);

  s.integer(sprite.enable);
  s.integer(sprite.color);
  s.integer(sprite.palette);
  s.integer(sprite.priority);
  //todo: serialize array<sprite.objects>

  s.integer(io.address);
  s.integer(io.externalSync);
  s.integer(io.displayOutput);
  s.integer(io.dramRefresh);
  s.integer(io.lineCoincidence);
  s.integer(io.vramAccess);
  s.integer(io.spriteAccess);
  s.integer(io.cgMode);
}
