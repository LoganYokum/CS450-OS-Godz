/**
 * Function for determining which pcb command needs to be run and test if the command is valid.
 * @param pcb_str Unparsed string containing the pcb command and arguments to parse.
 */
void pcb_op(char *pcb_str);
/**
 * Creates a new process with the given name, class, and priority.
 * @param name The name of the process to create.
 * @param class The class of the process to create.
 * @param priority The priority of the process to create.
*/
// void pcb_create(const char* name, int class, int priority);
/**
 * Deletes the process with the given name.
 * @param name The name of the process to delete.
 */
void pcb_delete(const char* name);
/**
 * Blocks the process with the given name.
 * @param name The name of the process to block.
*/
void pcb_block(const char* name);
/**
 * Unblocks the process with the given name.
 * @param name The name of the process to unblock.
*/
void pcb_unblock(const char* name);
/**
 * Suspends the process with the given name.
 * @param name The name of the process to suspend.
*/
void pcb_suspend(const char* name);
/**
 * Resumes the process with the given name.
 * @param name The name of the process to resume.
*/
void pcb_resume(const char* name);
/**
 * Changes the priority of the process with the given name.
 * @param name The name of the process to change the priority of.
 * @param priority The new priority of the process.
*/
void pcb_set_priority(const char* name, int priority);
void pcb_show_one(const char* name);
void pcb_show_ready();
void pcb_show_blocked();
void pcb_show_all();
