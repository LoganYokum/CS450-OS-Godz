#include <mpx/call.h>
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>

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
    pcb *next_process = NULL;
    context *next_context = NULL;
    op_code op = c->eax;
    device dev = c->ebx;
    char *buffer = (char *) c->ecx;
    size_t len = c->edx;

    //check for invalid parameters
    if(op!=0||op!=1||op!=2||op!=3){
        c->eax = -1;
        return c;
    }
    if(dev!=COM1||dev!=COM2||dev!=COM3||dev!=COM4){
        c->eax = -1;
        return c;
    }
    if(buffer==NULL){
        c->eax = -1;
        return c;
    }
    if(len<=0){
        c->eax = -1;
        return c;
    }

    for(struct dcb *device_dcb_ptr = dev; device_dcb_ptr < dev + 4; device_dcb_ptr++){ //CHECK FOR EVENT FLAGS ACROSS ALL DEVICES
        if(device_dcb_ptr->event_flag == 1 && device_dcb_ptr->open_flag == 1){ //IF EVENT FLAG IS SET, CHECK FOR IOCB QUEUE,  device is not busy
            while(device_dcb_ptr->iocb_queue != NULL){ //IF IOCB QUEUE IS NOT EMPTY, PROCESS IOCB QUEUE
                int index = serial_devno(dev); //get index of device
                iocb *temp_iocb = device_dcb_ptr->iocb_queue; //GET IOCB FROM QUEUE
                buffer = temp_iocb->buffer; //GET BUFFER FROM IOCB
                len = temp_iocb->buf_len; //GET LENGTH FROM IOCB
                dcb dev_dcb = dcb_table[index]; //get device control block
                next_process = temp_iocb->process; //get pcb of iocb 
                pcb *temp_pcb = next_process; //get pcb of ready head process
                pcb_remove(next_process); //remove from ready queue
                temp_pcb->state = 0x02; //blocked state
                pcb_insert(temp_pcb); //insert into blocked queue (state is set to block)
                if(temp_iocb->cur_op == READ){ // op is read
                    while(dev_dcb.event_flag == 1){ //while event flag says there is data to read
                        serial_read(dev, buffer, len); //read from device
                    }
                }
                else if (temp_iocb->cur_op == WRITE){ // op is write
                    while(dev_dcb.event_flag == 1){ //while event flag says there is data to write
                        serial_write(dev, buffer, len); //write to device
                    }
                }
                else{ // op is invalid
                    c->eax = -1;
                    return c;
                }
                // COMPLETION SEQUENCE
                pcb_remove(temp_pcb); //remove from blocked queue
                dev_dcb.event_flag = 0; // reset event flag
                dev_dcb.open_flag = 0; // reset open flag
                temp_pcb->state = 0x01; // process back to ready state
                pcb_insert(temp_pcb); //insert into ready queue
                context* temp_context = (context *) ready_head->stack_ptr; // get context of ready head process
                context* return_context = idle(temp_context); //dispatch new process with IDLE operation by pulling the next thing from the ready queue (icall)
                temp_iocb = temp_iocb->next; //get next iocb
            }
        }
        else if(device_dcb_ptr->event_flag == 1 && device_dcb_ptr->open_flag == 0) { //device is busy
            ready_head->state = 0x02; //move ready head process to blocked state
            //dispatch new process with IDLE operation by pulling the next thing from the ready queue (icall)
            context* temp_context = (context *) ready_head->stack_ptr; // get context of ready head process
            return idle(temp_context);
        }
    }
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
