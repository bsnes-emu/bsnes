SMPRegisterEditor *smpRegisterEditor = nullptr;

SMPRegisterEditor::SMPRegisterEditor() {
  setTitle("SMP Register Editor");

  layout.setMargin(5);
  regALabel.setText("A:");
  regAValue.setFont(application->monospaceFont);
  regXLabel.setText("X:");
  regXValue.setFont(application->monospaceFont);
  regYLabel.setText("Y:");
  regYValue.setFont(application->monospaceFont);
  regSLabel.setText("S:01");
  regSValue.setFont(application->monospaceFont);
  flagN.setText("N");
  flagV.setText("V");
  flagP.setText("P");
  flagB.setText("B");
  flagH.setText("H");
  flagI.setText("I");
  flagZ.setText("Z");
  flagC.setText("C");
  update.setText("Update");

  loadRegisters();

  layout.append(primaryLayout, {~0, 0}, 5);
    primaryLayout.append(regALabel, {0, 0}, 5);
    primaryLayout.append(regAValue, {0, 0}, 5);
    primaryLayout.append(regXLabel, {0, 0}, 5);
    primaryLayout.append(regXValue, {0, 0}, 5);
    primaryLayout.append(regYLabel, {0, 0}, 5);
    primaryLayout.append(regYValue, {0, 0}, 5);
    primaryLayout.append(regSLabel, {0, 0}, 5);
    primaryLayout.append(regSValue, {0, 0});
  layout.append(secondaryLayout, {~0, 0}, 5);
    secondaryLayout.append(flagN, {0, 0}, 5);
    secondaryLayout.append(flagV, {0, 0}, 5);
    secondaryLayout.append(flagP, {0, 0}, 5);
    secondaryLayout.append(flagB, {0, 0}, 5);
    secondaryLayout.append(flagH, {0, 0}, 5);
    secondaryLayout.append(flagI, {0, 0}, 5);
    secondaryLayout.append(flagZ, {0, 0}, 5);
    secondaryLayout.append(flagC, {0, 0});
  layout.append(tertiaryLayout, {~0, 0});
    tertiaryLayout.append(spacer, {~0, 0});
    tertiaryLayout.append(update, {80, 0});
  append(layout);

  update.onActivate = [&] {
    saveRegisters();
    smpDebugger->updateDisassembly();
    setVisible(false);
  };

  setGeometry({{128, 128}, layout.minimumGeometry().size()});
  windowManager->append(this, "SMPRegisterEditor");
}

void SMPRegisterEditor::loadRegisters() {
  regAValue.setText(hex<2>(SFC::smp.regs.a));
  regXValue.setText(hex<2>(SFC::smp.regs.x));
  regYValue.setText(hex<2>(SFC::smp.regs.y));
  regSValue.setText(hex<2>(SFC::smp.regs.s));
  flagN.setChecked(SFC::smp.regs.p.n);
  flagV.setChecked(SFC::smp.regs.p.v);
  flagP.setChecked(SFC::smp.regs.p.p);
  flagB.setChecked(SFC::smp.regs.p.b);
  flagH.setChecked(SFC::smp.regs.p.h);
  flagI.setChecked(SFC::smp.regs.p.i);
  flagZ.setChecked(SFC::smp.regs.p.z);
  flagC.setChecked(SFC::smp.regs.p.c);
}

void SMPRegisterEditor::saveRegisters() {
  SFC::smp.regs.a = hex(regAValue.text());
  SFC::smp.regs.x = hex(regXValue.text());
  SFC::smp.regs.y = hex(regYValue.text());
  SFC::smp.regs.s = hex(regSValue.text());
  SFC::smp.regs.p.n = flagN.checked();
  SFC::smp.regs.p.v = flagV.checked();
  SFC::smp.regs.p.p = flagP.checked();
  SFC::smp.regs.p.b = flagB.checked();
  SFC::smp.regs.p.h = flagH.checked();
  SFC::smp.regs.p.i = flagI.checked();
  SFC::smp.regs.p.z = flagZ.checked();
  SFC::smp.regs.p.c = flagC.checked();
}
