void Ananke::applyBeatPatch(vector<uint8_t> &buffer) {
  string name = {nall::basename(information.name), ".bps"};
  if(!file::exists(name)) return;

  bpspatch patch;
  if(patch.modify(name) == false) return;
  patch.source(buffer.data(), buffer.size());
  vector<uint8_t> output;
  output.resize(patch.size());
  patch.target(output.data(), output.size());
  if(patch.apply() == bpspatch::result::success) {
    buffer = output;
    information.manifest = patch.metadata();
  }
}
