#include <mpx/call.h>
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>

void temp_funt();

pcb *current_process;
context *first_context = NULL;
context *idle(context *c) {
    pcb *next_process;
    context *next_context = NULL;
    op_code op = c->eax;

    if (op == IDLE) {
        if (first_context == NULL) {
            first_context = c;
        }

        if (ready_head == NULL) {
            c->eax = 0;
            return c;
        }
        next_process = ready_head; // get head of ready queue
        pcb_remove(next_process);  // and remove it

        if (current_process == NULL) { // first instance of sys_call
            current_process = next_process;
        }else {
            current_process->stack_ptr = (char *) c; // update stack pointer
            pcb_insert(current_process); // insert current pcb back into queue
        }
        
        next_context = (context *) next_process->stack_ptr; // get context of next process
        next_context->eax = 0; // set okay return value for sys_call

        current_process = next_process; // update current process

        return next_context;
    }
}
context *sys_call(context *c) {
    pcb *next_process;
    context *next_context = NULL;
    op_code op = c->eax;

    if (op == IDLE) {
        if (first_context == NULL) {
            first_context = c;
        }

        if (ready_head == NULL) {
            c->eax = 0;
            return c;
        }
        next_process = ready_head; // get head of ready queue
        pcb_remove(next_process);  // and remove it

        if (current_process == NULL) { // first instance of sys_call
            current_process = next_process;
        }else {
            current_process->stack_ptr = (char *) c; // update stack pointer
            pcb_insert(current_process); // insert current pcb back into queue
        }
        
        next_context = (context *) next_process->stack_ptr; // get context of next process
        next_context->eax = 0; // set okay return value for sys_call

        current_process = next_process; // update current process

        return next_context;
    }else if (op == EXIT) {
        pcb_free(current_process); // delete current process

        if (ready_head == NULL) {
            first_context->eax = 0;
            return first_context;
        }
        next_process = ready_head; // get head of ready queue
        pcb_remove(next_process);  // and remove it

        current_process = next_process; // update current process
            
        next_context = (context *) next_process->stack_ptr; // get context of next process
        next_context->eax = 0; // set okay return value for sys_call
        return next_context;
    }
    device dev = c->ebx;
    char *buffer = (char *) c->ecx;
    size_t len = c->edx;

    int index = serial_devno(dev);
    dcb dev = devices[index];

    if (dev->open_flag == 0) { // device is not busy
        //call read or write
        if (op == READ) {
            serial_read(dev, buffer, len);
        }
        else if(op == WRITE) {
            serial_write(dev, buffer, len);
        }
    }else { //device is busy
        //schedule device via IO Scheduler
        //move ready head process to blocked state
        ready_head->state = 0x02;
        //dispatch new process with IDLE operation by pulling the next thing from the ready queue (icall)
        context* temp_context = (context *) ready_head->stack_ptr; // get context of ready head process
        context* return_context = idle(temp_context);
        //invoke device driver interrupt
        serial_interrupt();
        //check to see if its complete 
        if(return_context->eax = 0) {
            dev->event_flag = 1; //set event flag
        }
        //CHECK FOR EVENT FLAGS ACROSS ALL DEVICES
    }
    c->eax = -1;
    return c;
}
