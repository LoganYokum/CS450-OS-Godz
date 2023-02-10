void pcb_op(const char* param_str, const char* pcb_name, const char* pcb_class, const char* pcb_priority);
void pcb_create(const char* name, int class, int priority);
void pcb_delete(const char* name);
void pcb_block(const char* name);
void pcb_unblock(const char* name);
void pcb_suspend(const char* name);

// void pcb_resume(const char* name);
// void pcb_set_priority(const char* name, int priority);
// void pcb_show_pcb(const char* name);
// void pcb_show_ready();
// void pcb_show_blocked();
// void pcb_show_all();
