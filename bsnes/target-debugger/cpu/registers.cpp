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
  regAValue.setText(hex<4>(SFC::cpu.regs.a));
  regXValue.setText(hex<4>(SFC::cpu.regs.x));
  regYValue.setText(hex<4>(SFC::cpu.regs.y));
  regSValue.setText(hex<4>(SFC::cpu.regs.s));
  regDValue.setText(hex<4>(SFC::cpu.regs.d));
  regDBValue.setText(hex<2>(SFC::cpu.regs.db));
  flagN.setChecked(SFC::cpu.regs.p.n);
  flagV.setChecked(SFC::cpu.regs.p.v);
  flagM.setChecked(SFC::cpu.regs.p.m);
  flagX.setChecked(SFC::cpu.regs.p.x);
  flagD.setChecked(SFC::cpu.regs.p.d);
  flagI.setChecked(SFC::cpu.regs.p.i);
  flagZ.setChecked(SFC::cpu.regs.p.z);
  flagC.setChecked(SFC::cpu.regs.p.c);
  flagE.setChecked(SFC::cpu.regs.e);
}

void CPURegisterEditor::saveRegisters() {
  SFC::cpu.regs.a = hex(regAValue.text());
  SFC::cpu.regs.x = hex(regXValue.text());
  SFC::cpu.regs.y = hex(regYValue.text());
  SFC::cpu.regs.s = hex(regSValue.text());
  SFC::cpu.regs.d = hex(regDValue.text());
  SFC::cpu.regs.db = hex(regDBValue.text());
  SFC::cpu.regs.p.n = flagN.checked();
  SFC::cpu.regs.p.v = flagV.checked();
  SFC::cpu.regs.p.m = flagM.checked();
  SFC::cpu.regs.p.x = flagX.checked();
  SFC::cpu.regs.p.d = flagD.checked();
  SFC::cpu.regs.p.i = flagI.checked();
  SFC::cpu.regs.p.z = flagZ.checked();
  SFC::cpu.regs.p.c = flagC.checked();
  SFC::cpu.regs.e = flagE.checked();
  SFC::cpu.update_table();  //cache E/M/X flags
}
