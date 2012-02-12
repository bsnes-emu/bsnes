CPURegisterEditor *cpuRegisterEditor = nullptr;

CPURegisterEditor::CPURegisterEditor() {
  setTitle("CPU Register Editor");

  layout.setMargin(5);
  regALabel.setText("A:");
  regAValue.setFont(application->monospaceFont);
  regXLabel.setText("X:");
  regXValue.setFont(application->monospaceFont);
  regYLabel.setText("Y:");
  regYValue.setFont(application->monospaceFont);
  regSLabel.setText("S:");
  regSValue.setFont(application->monospaceFont);
  regDLabel.setText("D:");
  regDValue.setFont(application->monospaceFont);
  regDBLabel.setText("DB:");
  regDBValue.setFont(application->monospaceFont);
  flagN.setText("N");
  flagV.setText("V");
  flagM.setText("M");
  flagX.setText("X");
  flagD.setText("D");
  flagI.setText("I");
  flagZ.setText("Z");
  flagC.setText("C");
  flagE.setText("E");
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
    primaryLayout.append(regSValue, {0, 0}, 5);
    primaryLayout.append(regDLabel, {0, 0}, 5);
    primaryLayout.append(regDValue, {0, 0}, 5);
    primaryLayout.append(regDBLabel, {0, 0}, 5);
    primaryLayout.append(regDBValue, {0, 0});
  layout.append(secondaryLayout, {~0, 0}, 5);
    secondaryLayout.append(flagN, {0, 0}, 5);
    secondaryLayout.append(flagV, {0, 0}, 5);
    secondaryLayout.append(flagM, {0, 0}, 5);
    secondaryLayout.append(flagX, {0, 0}, 5);
    secondaryLayout.append(flagD, {0, 0}, 5);
    secondaryLayout.append(flagI, {0, 0}, 5);
    secondaryLayout.append(flagZ, {0, 0}, 5);
    secondaryLayout.append(flagC, {0, 0});
  layout.append(tertiaryLayout, {~0, 0});
    tertiaryLayout.append(flagE, {0, 0}, 5);
    tertiaryLayout.append(spacer, {~0, 0});
    tertiaryLayout.append(update, {80, 0});
  append(layout);

  update.onActivate = [&] {
    saveRegisters();
    cpuDebugger->updateDisassembly();
    setVisible(false);
  };

  setGeometry({{128, 128}, layout.minimumGeometry().size()});
  windowManager->append(this, "CPURegisterEditor");
}

void CPURegisterEditor::loadRegisters() {
  regAValue.setText(hex<4>(SNES::cpu.regs.a));
  regXValue.setText(hex<4>(SNES::cpu.regs.x));
  regYValue.setText(hex<4>(SNES::cpu.regs.y));
  regSValue.setText(hex<4>(SNES::cpu.regs.s));
  regDValue.setText(hex<4>(SNES::cpu.regs.d));
  regDBValue.setText(hex<2>(SNES::cpu.regs.db));
  flagN.setChecked(SNES::cpu.regs.p.n);
  flagV.setChecked(SNES::cpu.regs.p.v);
  flagM.setChecked(SNES::cpu.regs.p.m);
  flagX.setChecked(SNES::cpu.regs.p.x);
  flagD.setChecked(SNES::cpu.regs.p.d);
  flagI.setChecked(SNES::cpu.regs.p.i);
  flagZ.setChecked(SNES::cpu.regs.p.z);
  flagC.setChecked(SNES::cpu.regs.p.c);
  flagE.setChecked(SNES::cpu.regs.e);
}

void CPURegisterEditor::saveRegisters() {
  SNES::cpu.regs.a = hex(regAValue.text());
  SNES::cpu.regs.x = hex(regXValue.text());
  SNES::cpu.regs.y = hex(regYValue.text());
  SNES::cpu.regs.s = hex(regSValue.text());
  SNES::cpu.regs.d = hex(regDValue.text());
  SNES::cpu.regs.db = hex(regDBValue.text());
  SNES::cpu.regs.p.n = flagN.checked();
  SNES::cpu.regs.p.v = flagV.checked();
  SNES::cpu.regs.p.m = flagM.checked();
  SNES::cpu.regs.p.x = flagX.checked();
  SNES::cpu.regs.p.d = flagD.checked();
  SNES::cpu.regs.p.i = flagI.checked();
  SNES::cpu.regs.p.z = flagZ.checked();
  SNES::cpu.regs.p.c = flagC.checked();
  SNES::cpu.regs.e = flagE.checked();
  SNES::cpu.update_table();  //cache E/M/X flags
}
