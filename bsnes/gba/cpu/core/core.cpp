void ARM7TDMI::power() {
  for(auto &gpr : r) gpr = 0;
}
