#ifdef DSP_CPP

//===========
//DSPDebugger
//===========

unsigned sDSPDebugger::main_volume_left() { return state.regs[0x0c]; }
unsigned sDSPDebugger::main_volume_right() { return state.regs[0x1c]; }
unsigned sDSPDebugger::echo_volume_left() { return state.regs[0x2c]; }
unsigned sDSPDebugger::echo_volume_right() { return state.regs[0x3c]; }
unsigned sDSPDebugger::key_on() { return state.regs[0x4c]; }
unsigned sDSPDebugger::key_off() { return state.regs[0x5c]; }
bool sDSPDebugger::flag_reset() { return state.regs[0x6c] & 0x80; }
bool sDSPDebugger::flag_mute() { return state.regs[0x6c] & 0x40; }
bool sDSPDebugger::flag_echo_disable() { return state.regs[0x6c] & 0x20; }
unsigned sDSPDebugger::flag_noise_clock() { return state.regs[0x6c] & 0x1f; }
unsigned sDSPDebugger::source_end_block() { return state.regs[0x7c]; }
unsigned sDSPDebugger::echo_feedback() { return state.regs[0x0d]; }
unsigned sDSPDebugger::pitch_modulation_enable() { return state.regs[0x2d]; }
unsigned sDSPDebugger::noise_enable() { return state.regs[0x3d]; }
unsigned sDSPDebugger::echo_enable() { return state.regs[0x4d]; }
unsigned sDSPDebugger::source_directory() { return state.regs[0x5d]; }
unsigned sDSPDebugger::echo_start_address() { return state.regs[0x6d]; }
unsigned sDSPDebugger::echo_directory() { return state.regs[0x7d]; }
unsigned sDSPDebugger::echo_filter_coefficient(unsigned n) { return state.regs[(n << 4) + 0x0f]; }
unsigned sDSPDebugger::voice_volume_left(unsigned n) { return state.regs[(n << 4) + 0x00]; }
unsigned sDSPDebugger::voice_volume_right(unsigned n) { return state.regs[(n << 4) + 0x01]; }
unsigned sDSPDebugger::voice_pitch_height(unsigned n) { return state.regs[(n << 4) + 0x02] + (state.regs[(n << 4) + 0x03] << 8); }
unsigned sDSPDebugger::voice_source_number(unsigned n) { return state.regs[(n << 4) + 0x04]; }
unsigned sDSPDebugger::voice_adsr1(unsigned n) { return state.regs[(n << 4) + 0x05]; }
unsigned sDSPDebugger::voice_adsr2(unsigned n) { return state.regs[(n << 4) + 0x06]; }
unsigned sDSPDebugger::voice_gain(unsigned n) { return state.regs[(n << 4) + 0x07]; }
unsigned sDSPDebugger::voice_envx(unsigned n) { return state.regs[(n << 4) + 0x08]; }
unsigned sDSPDebugger::voice_outx(unsigned n) { return state.regs[(n << 4) + 0x09]; }

#endif
