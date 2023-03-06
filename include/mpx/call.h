#include <stdint.h>
typedef struct context {
    int ds, es, fs, gs, ss; // segment registers
    int eax, ebx, ecx, edx, esi, edi, ebp; // general registers
    int eip, cs, eflags; // control registers
}context;

/**
 * Loads the next process into memory and returns the context of the next process
 * @param c context of the current process
 * @return context of the next process 
 */
context *sys_call(context *c);