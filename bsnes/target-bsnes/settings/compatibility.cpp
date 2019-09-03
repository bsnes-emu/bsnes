auto CompatibilitySettings::create() -> void {
  setCollapsible();
  setVisible(false);

  entropyLabel.setText("Entropy (randomization)").setFont(Font().setBold());
  entropyNone.setText("None").setToolTip(
    "All memory and registers are initialized to constant values at startup.\n"
    "Use this for compatibility with very old demoscene homebrew games."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "None";
  });
  entropyLow.setText("Low").setToolTip(
    "All memory is randomized with repeating patterns, all registers are randomized at startup.\n"
    "Use this for the most accurate representation of a real SNES."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "Low";
  });
  entropyHigh.setText("High").setToolTip(
    "All memory and registers are randomized as much as possible.\n"
    "Use this when developing new SNES software to ensure maximum compatibility with real hardware."
  ).onActivate([&] {
    settings.emulator.hack.entropy = "High";
  });
  if(settings.emulator.hack.entropy == "None") entropyNone.setChecked();
  if(settings.emulator.hack.entropy == "Low" ) entropyLow.setChecked();
  if(settings.emulator.hack.entropy == "High") entropyHigh.setChecked();

  cpuLabel.setFont(Font().setBold()).setText("CPU (processor)");
  fastMath.setText("Fast math").setToolTip(
    "CPU multiplication and division take time to complete on a real SNES.\n"
    "Older emulators did not simulate these delays, and provided results immediately.\n"
    "Some older ROM hacks do not wait for math operations to complete and need this hack."
  ).setChecked(settings.emulator.hack.cpu.fastMath).onToggle([&] {
    settings.emulator.hack.cpu.fastMath = fastMath.checked();
    emulator->configure("Hacks/CPU/FastMath", settings.emulator.hack.cpu.fastMath);
  });

  ppuLabel.setFont(Font().setBold()).setText("PPU (video)");
  noVRAMBlocking.setText("No VRAM blocking").setToolTip(
    "This option emulates a bug in older releases of ZSNES and Snes9X where VRAM blocking was not emulated.\n"
    "A few older ROM hacks relied on this behavior, and will render graphics incorrectly if not enabled.\n"
    "Not only is this extremely inaccurate to real hardware, it also hurts the speed of the fast PPU.\n"
    "Do not enable this option unless you need to play a game that is incompatible with bsnes otherwise."
  ).setChecked(settings.emulator.hack.ppu.noVRAMBlocking).onToggle([&] {
    settings.emulator.hack.ppu.noVRAMBlocking = noVRAMBlocking.checked();
    emulator->configure("Hacks/PPU/NoVRAMBlocking", settings.emulator.hack.ppu.noVRAMBlocking);
  });

  dspLabel.setFont(Font().setBold()).setText("DSP (audio)");
  echoShadow.setText("Echo shadow RAM").setToolTip(
    "This option emulates a bug in ZSNES where echo RAM was treated as separate from APU RAM.\n"
    "Many older ROM hacks for Super Mario World relied on this behavior, and will crash without enabling this.\n"
    "It is, however, extremely inaccurate to real hardware and should not be enabled unless required."
  ).setChecked(settings.emulator.hack.dsp.echoShadow).onToggle([&] {
    settings.emulator.hack.dsp.echoShadow = echoShadow.checked();
    //not a run-time setting: do not call emulator->configure() here.
  });

  note.setText("Note: some settings do not take effect until after reloading games.");
}
