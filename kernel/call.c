#include <mpx/call.h>
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>
#include <memory.h>

pcb *current_process;
context *first_context = NULL;
int device_index = -1;

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
        if(d->event_flag == 1 && d->open_flag == 1){ //IF EVENT FLAG IS SET, CHECK FOR IOCB QUEUE FOR COMPLETION
            iocb *temp_iocb = d->iocb_queue; //GET IOCB FROM QUEUE
            next_process = temp_iocb->process; //get pcb of iocb 
            pcb_remove(next_process); //remove from blocked queue
            d->event_flag = 0; // reset event flag
            next_process->state = 0x01; // process back to ready state
            pcb_insert(next_process); //insert into ready queue
            context* iocb_context = next_process->stack_ptr; //get context of iocb process
            iocb_context->eax = temp_iocb->buf_len; //set return value of iocb buffer len for sys_call
            iocb_dequeue(&d->iocb_queue); //dequeue iocb
            d->busy_flag = 0; //reset busy flag
        }
    }
    if(op == READ){
        if(dev==COM1){
            device_index = 0;
        }
        else if(dev==COM2){
            device_index = 1;
        }
        else if(dev==COM3){
            device_index = 2;
        }
        else if(dev==COM4){
            device_index = 3;
        }
        dcb *d = &dcb_table[device_index];

        iocb* new_io = sys_alloc_mem(sizeof(iocb)); //allocate memory for new iocb
        new_io->cur_op = op; //set op
        new_io->buffer = buffer; //set buffer
        new_io->buf_idx = 0; //set index
        new_io->buf_len = len; //set length
        new_io->process = current_process; //set process
        new_io->next = NULL; //set next to null
        iocb_enqueue(&(d->iocb_queue), new_io); //enqueue iocb

        current_process->state = 0x02; //move ready head process to blocked state
        pcb_insert(current_process); //insert into blocked queue

         if (d->busy_flag) { // if busy flag is set, call idle to get next process
            return idle(c, next_process, next_context);
        }

        size_t transferred_bytes = serial_read(dev, buffer, len);
        //if it can be satisfied on the ring buffer return current context otherwise call idle
        if(transferred_bytes == len){
            c->eax = transferred_bytes;
            return c;
        }
        return idle(c, next_process, next_context);
        // c->eax = -1; // for testing purposes
        // return c;
    }
    else if(op == WRITE){
        if(dev==COM1){
            device_index = 0;
        }
        else if(dev==COM2){
            device_index = 1;
        }
        else if(dev==COM3){
            device_index = 2;
        }
        else if(dev==COM4){
            device_index = 3;
        }
        dcb *d = &dcb_table[device_index];
        
        iocb* new_io = sys_alloc_mem(sizeof(iocb)); //allocate memory for new iocb
        new_io->cur_op = op; //set op
        new_io->buffer = buffer; //set buffer
        new_io->buf_idx = 0; //set index
        new_io->buf_len = len; //set length
        new_io->process = current_process; //set process
        new_io->next = NULL; //set next to null
        iocb_enqueue(&(d->iocb_queue), new_io); //enqueue iocb

        current_process->state = 0x02; //move current process to blocked state
        pcb_insert(current_process); //insert into blocked queue

        if (d->busy_flag) {
            return idle(c, next_process, next_context);
        }
        serial_write(dev, buffer, len);
       
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