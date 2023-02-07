void pcb(const char* param_str, const char* extra_arg);
void pcb_create(const char* name, int priority, int class);
void pcb_delete(const char* name);
void pcb_block(const char* name);
void pcb_unblock(const char* name);
void pcb_suspend(const char* name);
void pcb_resume(const char* name);
void pcb_set_priority(const char* name, int class, int priority, int state, int suspended_status);
void pcb_show_pcb(const char* name, int class, int priority, int state, int suspended_status);
void pcb_show_ready(const char* name, int class, int priority, int state, int suspended_status);
void pcb_show_blocked(const char* name, int class, int priority, int state, int suspended_status);
void pcb_show_all(const char* name, int class, int priority, int state, int suspended_status);
