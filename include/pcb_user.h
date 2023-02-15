/**
 * Executes the apporpriate pcb operation based on the passed operation string.
 * @param pcb_str PCB operation with potential arguments.
 */
void pcb_op(char *pcb_str);

/**
 * Creates a new PCB with the given name, class, and priority.
 * @param name Name of the new PCB.
 * @param class Class of the new PCB.
 * @param priority Priority of the new PCB.
 */
void pcb_create(const char* name, int class, int priority);

/**
 * Deletes the PCB with the given name.
 * @param name Name of the PCB to delete.
 */
void pcb_delete(const char* name);

/**
 * Blocks the PCB with the given name.
 * @param name Name of the PCB to block.
 */
void pcb_block(const char* name);

/**
 * Unblocks the PCB with the given name.
 * @param name Name of the PCB to unblock.
 */
void pcb_unblock(const char* name);

/**
 * Suspends the PCB with the given name.
 * @param name Name of the PCB to suspend.
 */
void pcb_suspend(const char* name);

/**
 * Resumes the PCB with the given name.
 * @param name Name of the PCB to resume.
 */
void pcb_resume(const char* name);

/**
 * Changes the priority of the PCB with the given name.
 * @param name Name of the PCB to change priority.
 * @param priority New priority of the PCB.
 */
void pcb_set_priority(const char* name, int priority);

/**
 * Displays the contents of the PCB with the given name.
 * @param name Name of the PCB to display.
 */
void pcb_show_one(const char* name);

/**
 * Displays the contents of all PCBs in that ready state.
 */
void pcb_show_ready();

/**
 * Displays the contents of all PCBs in that blocked state.
 */
void pcb_show_blocked();

/**
 * Displays the contents of all PCBs.
 */
void pcb_show_all();
