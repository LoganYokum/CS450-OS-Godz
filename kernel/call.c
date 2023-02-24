#include <stdint.h>
#include <sys_req.h>
#include <pcb.h>

typedef struct context {
    int esp; // stack pointer
    int ds, es, fs, gs, ss; // segment registers
    int edi, esi, edp, ebx, edx, ecx, eax; // general purpose registers
    int eip, cs, eflags; // control registers
}context;

pcb *executing_process;
context *first_context;

context *sys_call(context *c) {
    if (first_context == NULL) {
        first_context = c;
    }

    pcb *next_process;
    op_code op = c->eax;
    if (op == IDLE) {
        if (ready_head == NULL) {
            next_process = executing_process;
        }else {
            next_process = ready_head; // get head of ready queue
            pcb_remove(next_process);  // and remove it
            // still need save context of current pcb by updating stack pointer
            pcb_insert(executing_process); // insert current pcb back into queue
            // still need return context of the next process

        }
        c->eax = 0;
    }else if (op == EXIT) {
        
    }else {
       
    }
}