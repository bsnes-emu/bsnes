void thumb_step();

void thumb_mov(GPR &source, uint32 modify);
void thumb_cmp(GPR &source, uint32 modify);
void thumb_add(GPR &source, uint32 modify);
void thumb_sub(GPR &source, uint32 modify);

void thumb_op_immediate();
