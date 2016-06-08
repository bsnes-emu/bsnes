auto ARM::serialize(serializer& s) -> void {
  s.integer(processor.r0.data);
  s.integer(processor.r1.data);
  s.integer(processor.r2.data);
  s.integer(processor.r3.data);
  s.integer(processor.r4.data);
  s.integer(processor.r5.data);
  s.integer(processor.r6.data);
  s.integer(processor.r7.data);

  s.integer(processor.usr.r8.data);
  s.integer(processor.usr.r9.data);
  s.integer(processor.usr.r10.data);
  s.integer(processor.usr.r11.data);
  s.integer(processor.usr.r12.data);
  s.integer(processor.usr.sp.data);
  s.integer(processor.usr.lr.data);

  s.integer(processor.fiq.r8.data);
  s.integer(processor.fiq.r9.data);
  s.integer(processor.fiq.r10.data);
  s.integer(processor.fiq.r11.data);
  s.integer(processor.fiq.r12.data);
  s.integer(processor.fiq.sp.data);
  s.integer(processor.fiq.lr.data);
  s.integer(processor.fiq.spsr.data);

  s.integer(processor.irq.sp.data);
  s.integer(processor.irq.lr.data);
  s.integer(processor.irq.spsr.data);

  s.integer(processor.svc.sp.data);
  s.integer(processor.svc.lr.data);
  s.integer(processor.svc.spsr.data);

  s.integer(processor.abt.sp.data);
  s.integer(processor.abt.lr.data);
  s.integer(processor.abt.spsr.data);

  s.integer(processor.und.sp.data);
  s.integer(processor.und.lr.data);
  s.integer(processor.und.spsr.data);

  s.integer(processor.pc.data);
  s.integer(processor.cpsr.data);
  s.integer(processor.carryout);
  s.integer(processor.irqline);

  s.integer(pipeline.reload);
  s.integer(pipeline.nonsequential);
  s.integer(pipeline.execute.address);
  s.integer(pipeline.execute.instruction);
  s.integer(pipeline.decode.address);
  s.integer(pipeline.decode.instruction);
  s.integer(pipeline.fetch.address);
  s.integer(pipeline.fetch.instruction);

  s.integer(crash);

  processor.setMode((Processor::Mode)(uint)cpsr().m);
}
