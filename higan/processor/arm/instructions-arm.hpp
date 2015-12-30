auto arm_opcode(uint32 rm);
auto arm_move_to_status(uint32 rm);

auto arm_op_multiply();
auto arm_op_multiply_long();
auto arm_op_memory_swap();
auto arm_op_move_half_register();
auto arm_op_move_half_immediate();
auto arm_op_load_register();
auto arm_op_load_immediate();

auto arm_op_move_to_register_from_status();
auto arm_op_move_to_status_from_register();
auto arm_op_branch_exchange_register();

auto arm_op_move_to_status_from_immediate();
auto arm_op_data_immediate_shift();
auto arm_op_data_register_shift();
auto arm_op_data_immediate();
auto arm_op_move_immediate_offset();
auto arm_op_move_register_offset();
auto arm_op_move_multiple();
auto arm_op_branch();
auto arm_op_software_interrupt();
