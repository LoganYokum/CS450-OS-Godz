#include <mpx/call.h>
#include <mpx/serial.h>
#include <stddef.h>
#include <sys_req.h>
#include <memory.h>

pcb *current_process;
context *first_context = NULL;
int device_state[4] = {0,0,0,0};
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
        if (&dcb_table[i] == NULL) continue;
        dcb d = dcb_table[i];
        if(d.event_flag == 1 && d.open_flag == 1){ //IF EVENT FLAG IS SET, CHECK FOR IOCB QUEUE,  device is not busy
            if(dev==COM1){
                device_index = 0;
                device_state[device_index] = 1;
            }
            else if(dev==COM2){
                device_index = 1;
                device_state[device_index] = 1;
            }
            else if(dev==COM3){
                device_index = 2;
                device_state[device_index] = 1;
            }
            else if(dev==COM4){
                device_index = 3;
                device_state[device_index] = 1;
            }
            while(d.iocb_queue != NULL){ //IF IOCB QUEUE IS NOT EMPTY, PROCESS IOCB QUEUE
                iocb *temp_iocb = d.iocb_queue; //GET IOCB FROM QUEUE
                buffer = temp_iocb->buffer; //GET BUFFER FROM IOCB
                len = temp_iocb->buf_len; //GET LENGTH FROM IOCB
                next_process = temp_iocb->process; //get pcb of iocb 
                pcb *temp_pcb = next_process; //get pcb of ready head process
                pcb_remove(next_process); //remove from ready queue
                temp_pcb->state = 0x02; //blocked state
                pcb_insert(temp_pcb); //insert into blocked queue (state is set to block)
                if(temp_iocb->cur_op == READ){ // op is read
                    serial_read(dev, buffer, len); //read from device
                }
                else if (temp_iocb->cur_op == WRITE){ // op is write
                    serial_write(dev, buffer, len); //write to device
                }
                else{ // op is invalid
                    c->eax = -1;
                    return c;
                }
                // COMPLETION SEQUENCE
                pcb_remove(temp_pcb); //remove from blocked queue
                d.event_flag = 0; // reset event flag
                d.open_flag = 0; // reset open flag
                temp_pcb->state = 0x01; // process back to ready state
                pcb_insert(temp_pcb); //insert into ready queue
                context* iocb_context = temp_pcb->stack_ptr; //get context of iocb process
                iocb_context->eax = temp_iocb->buf_len; //set return value of iocb buffer len for sys_call
                iocb_dequeue(&d.iocb_queue); //dequeue iocb
                idle((context *) ready_head->stack_ptr, next_process, next_context);
            }
            if(dev==COM1)
                device_state[device_index] = 0;
            else if(dev==COM2)
                device_state[device_index] = 0;
            else if(dev==COM3)
                device_state[device_index] = 0;
            else if(dev==COM4)
                device_state[device_index] = 0;
        }
        else if(device_state[device_index]==1) { //device is busy
            iocb* new_iocb = sys_alloc_mem(sizeof(iocb)); //allocate memory for new iocb
            new_iocb->cur_op = op; //set op
            new_iocb->buffer = buffer; //set buffer
            new_iocb->buf_len = len; //set length
            new_iocb->process = ready_head; //set process
            new_iocb->next = NULL; //set next to null
            iocb_enqueue(&d.iocb_queue, new_iocb); //enqueue iocb
            pcb *temp_pcb = ready_head; //get pcb of ready head process
            pcb_remove(ready_head);
            temp_pcb->state = 0x02; //move ready head process to blocked state
            pcb_insert(temp_pcb); //insert into blocked queue
            //dispatch new process with IDLE operation by pulling the next thing from the ready queue (icall)
            idle((context *) ready_head->stack_ptr, next_process, next_context);
        }
    }
    if(op == READ){
            //check for invalid parameters
        if(op!=0&&op!=1&&op!=2&&op!=3){
            c->eax = -1;
            return c;
        }
        if(dev!=COM1&&dev!=COM2&&dev!=COM3&&dev!=COM4){
            c->eax = -1;
            return c;
        }
        if(buffer==NULL||len<=0){
            c->eax = -1;
            return c;
        }
        pcb* temp_pcb = ready_head; //get pcb of ready head process
        pcb_remove(ready_head);
        temp_pcb->state = 0x02; //move ready head process to blocked state
        pcb_insert(temp_pcb); //insert into blocked queue
        int return_int = serial_read(dev, buffer, len);
        //if it can be satsidied on the ring buffer return current context otherwise call idle
        if(return_int>=1){
            return c;
        }
        else{
            return idle(c, next_process, next_context);
        }
    }
    else if(op == WRITE){
            //check for invalid parameters
        if(op!=0&&op!=1&&op!=2&&op!=3){
            c->eax = -1;
            return c;
        }
        if(dev!=COM1&&dev!=COM2&&dev!=COM3&&dev!=COM4){
            c->eax = -1;
            return c;
        }
        if(buffer==NULL||len<=0){
            c->eax = -1;
            return c;
        }
        pcb* temp_pcb = ready_head; //get pcb of ready head process
        pcb_remove(ready_head);
        temp_pcb->state = 0x02; //move ready head process to blocked state
        pcb_insert(temp_pcb); //insert into blocked queue
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
