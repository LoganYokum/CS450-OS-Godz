#include <stddef.h>
#include <sys_req.h>
#include <mpx/call.h>
#include <mpx/pcb.h>
#include <pcb_user.h>

pcb *executing_process;
context *first_context;

context *sys_call(context *c) {
    if (first_context == NULL) {
        first_context = c;
    }

    pcb *next_process;
    context *next_context = NULL;
    op_code op = c->eax;
    if (op == IDLE) {
        if (ready_head == NULL) {
            c->eax = 0;
            return c;
        }
        next_process = ready_head; // get head of ready queue
        pcb_remove(next_process);  // and remove it

        executing_process->stack_ptr = (char *) c; // update stack pointer
        pcb_insert(executing_process); // insert current pcb back into queue
        
        next_context = (context *) next_process->stack_ptr; // get context of next process
        next_context->eax = 0;
        return next_context;
    }else if (op == EXIT) {
        pcb_free(executing_process); // delete current process
        if (ready_head == NULL) {
            first_context->eax = 0;
            return first_context;
        }
        next_process = ready_head; // get head of ready queue
        pcb_remove(next_process);  // and remove it

        executing_process->stack_ptr = (char *) c; // update stack pointer
        pcb_insert(executing_process); // insert current pcb back into queue
            
        next_context = (context *) next_process->stack_ptr; // get context of next process
        next_context->eax = 0;
        return next_context;
    }
    c->eax = -1; // invalid op code
    return c;
}