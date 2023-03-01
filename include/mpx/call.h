typedef struct context {
    uint32_t ds, es, fs, gs, ss; // segment registers
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp; // general registers
    uint32_t eip, cs, eflags; // control registers
}context;

/**
 * Loads the next process into memory and returns the context of the next process
 * @param c context of the current process
 * @return context of the next process 
 */
context *sys_call(context *c);