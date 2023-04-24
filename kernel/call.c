#include <mpx/call.h>
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>
#include <memory.h>

pcb *current_process;
context *first_context = NULL;

context *idle(context *c, pcb *next_process, context *next_context) {
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

context *sys_call(context *c) {
    pcb *next_process = NULL;
    context *next_context = NULL;
    op_code op = c->eax;
    
    device dev = c->ebx;
    char *buffer = (char *) c->ecx;
    size_t len = c->edx;

    for (int i = 0; i < 4; i++) {
        dcb *d = &dcb_table[i];
        if(d->event_flag == 1 && d->open_flag == 1 && !d->busy_flag){ //IF EVENT FLAG IS SET, CHECK FOR IOCB QUEUE FOR COMPLETION
            iocb *temp_iocb = d->iocb_queue; //GET IOCB FROM QUEUE
            next_process = temp_iocb->process; //get pcb of iocb 
            pcb_remove(next_process); //remove from blocked queue
            d->event_flag = 0; // reset event flag
            d->open_flag = 0; // reset open flag
            next_process->state = 0x01; // process back to ready state
            pcb_insert(next_process); //insert into ready queue
            context* iocb_context = next_process->stack_ptr; //get context of iocb process
            iocb_context->eax = temp_iocb->buf_len; //set return value of iocb buffer len for sys_call
            iocb_dequeue(&d->iocb_queue); //dequeue iocb
            d->busy_flag = 0; //reset busy flag
            idle((context *) current_process->stack_ptr, next_process, next_context);
        }
        else if(d->busy_flag) { //device is busy
            //dispatch new process with IDLE operation by pulling the next thing from the ready queue (icall)
            idle((context *) current_process->stack_ptr, next_process, next_context);
        }
    }
    if(op == READ){
        dcb *d = &dcb_table[0]; //0 is com1
        
        iocb* new_iocb = sys_alloc_mem(sizeof(iocb)); //allocate memory for new iocb
        new_iocb->cur_op = op; //set op
        new_iocb->buffer = buffer; //set buffer
        new_iocb->buf_len = len; //set length
        new_iocb->process = current_process; //set process
        new_iocb->next = NULL; //set next to null
        iocb_enqueue(&(d->iocb_queue), new_iocb); //enqueue iocb

        pcb *temp_pcb = ready_head; //get pcb of ready head process
        pcb_remove(ready_head);
        temp_pcb->state = 0x02; //move ready head process to blocked state
        pcb_insert(temp_pcb); //insert into blocked queue

        size_t transferred_bytes = serial_read(dev, buffer, len);
        //if it can be satisfied on the ring buffer return current context otherwise call idle
        if(transferred_bytes == len){
            c->eax = transferred_bytes;
            return c;
        }
        return idle(c, next_process, next_context);
    }
    else if(op == WRITE){
        dcb *d = &dcb_table[0]; // 0 is com1
        
        iocb* new_iocb = sys_alloc_mem(sizeof(iocb)); //allocate memory for new iocb
        new_iocb->cur_op = op; //set op
        new_iocb->buffer = buffer; //set buffer
        new_iocb->buf_len = len; //set length
        new_iocb->process = current_process; //set process
        new_iocb->next = NULL; //set next to null
        iocb_enqueue(&(d->iocb_queue), new_iocb); //enqueue iocb

        current_process->state = 0x02; //move current process to blocked state
        pcb_insert(current_process); //insert into blocked queue

        size_t transferred_bytes = serial_write(dev, buffer, len);
        c->eax = transferred_bytes;
        return idle(c, next_process, next_context);
    }
    else if (op == IDLE) {
        return idle(c, next_process, next_context);
    }
    else if (op == EXIT) {
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
    c->eax = -1;
    return c;
}

