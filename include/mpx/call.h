#ifndef MPX_CALL_H
#define MPX_CALL_H

#include <stdint.h>
#include <mpx/pcb.h>
typedef struct context {
    int ds, es, fs, gs, ss; // segment registers
    int eax, ebx, ecx, edx, esi, edi, ebp; // general registers
    int eip, cs, eflags; // control registers
}context;

context *idle(context *c, pcb *next_process, context *next_context);

/**
 * Loads the next process into memory and returns the context of the next process
 * @param c context of the current process
 * @return context of the next process 
 */
context *sys_call(context *c);

#endif