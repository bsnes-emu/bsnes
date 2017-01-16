auto VDC::IRQ::poll() -> void {
  bool pending = false;
  pending |= pendingCollision;
  pending |= pendingOverflow;
  pending |= pendingLineCoincidence;
  pending |= pendingVblank;
  pending |= pendingTransferVRAM;
  pending |= pendingTransferSATB;
  cpu.irq.level(CPU::IRQ::Line::VDC, pending);
}

auto VDC::IRQ::raise(Line line) -> void {
  if(line == Line::Collision && enableCollision) {
    pendingCollision = true;
  }

  if(line == Line::Overflow && enableOverflow) {
    pendingOverflow = true;
  }

  if(line == Line::LineCoincidence && enableLineCoincidence) {
    pendingLineCoincidence = true;
  }

  if(line == Line::Vblank && enableVblank) {
    pendingVblank = true;
  }

  if(line == Line::TransferVRAM && enableTransferVRAM) {
    pendingTransferVRAM = true;
  }

  if(line == Line::TransferSATB && enableTransferSATB) {
    pendingTransferSATB = true;
  }

  poll();
}

auto VDC::IRQ::lower() -> void {
  pendingCollision = false;
  pendingOverflow = false;
  pendingLineCoincidence = false;
  pendingVblank = false;
  pendingTransferVRAM = false;
  pendingTransferSATB = false;

  poll();
}
