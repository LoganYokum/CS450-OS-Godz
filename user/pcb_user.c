#include <pcb_user.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <sys_req.h>
#include <mpx/pcb.h>
#include <memory.h>

#define READY_NOT_SUSPENDED 1
#define BLOCKED_NOT_SUSPENDED 2
#define READY_AND_SUSPENDED 17
#define BLOCKED_AND_SUSPENDED 18

void pcb_op(const char* param_str, const char* pcb_name, const char* pcb_type, const char* pcb_priority){
    if(strcmp(param_str, "create") == 0)
        pcb_create(pcb_name, atoi(pcb_type), atoi(pcb_priority));
    else if(strcmp(param_str, "delete") == 0)
        pcb_delete(pcb_name);
    else if(strcmp(param_str, "block") == 0)
        pcb_block(pcb_name);
    else if(strcmp(param_str, "unblock") == 0)
        pcb_unblock(pcb_name);
    else if(strcmp(param_str, "suspend") == 0)
        pcb_suspend(pcb_name);
    // else if(strcmp(param_str, "resume") == 0)
    //     pcb_resume(pcb_name);
    // else if(strcmp(param_str, "set_priority") == 0)
    //     pcb_set_priority(pcb_name, atoi(pcb_priority));
    // else if(strcmp(param_str, "show_pcb") == 0)
    //     pcb_show_pcb(pcb_name);
    // else if(strcmp(param_str, "show_ready") == 0)
    //     pcb_show_ready();
    // else if(strcmp(param_str, "show_blocked") == 0)
    //     pcb_show_blocked();
    // else if(strcmp(param_str, "show_all") == 0)
    //     pcb_show_all();
    else
        error("Incorrect parameter(s) for command: pcb. Try again.");
}

void pcb_create(const char* name, int type, int priority){
    if(strlen(name) > 8)
        error("Name too long. Must be 8 characters or less.");
    else if(type < 0 || type > 1)
        error("Invalid type. Must be 0 or 1.");
    else if(priority < 0 || priority > 9)
        error("Invalid priority. Must be 0-9.");
    else if(pcb_find(name) != NULL)
        error("Process already exists.");
    else{
        //process should be created.
        pcb *p = pcb_setup(name, type, priority);
        pcb_insert(p);
    }
}
void pcb_delete(const char* name){ 
    if(pcb_find(name) == NULL)
        error("Process does not exist.");
    else if(pcb_find(name)->type == READY_NOT_SUSPENDED)
        error("Cannot delete system process.");
    else{
        pcb *p = pcb_find(name);
        pcb_remove(p);
        pcb_free(p);
    }
}
void pcb_block(const char* name){
    if(pcb_find(name) == NULL)
        error("Process does not exist.");
    else if(pcb_find(name)->state == BLOCKED_NOT_SUSPENDED || pcb_find(name)->state == BLOCKED_AND_SUSPENDED)
        error("Process is already blocked.");
    else{
        pcb* p = pcb_find(name); //find PCB
        pcb_remove(p); // remove PCB from ready queue to prevent duplicates
        if(p->state == READY_NOT_SUSPENDED) //ready and not suspended
            p->state = BLOCKED_NOT_SUSPENDED; // update PCB state to blocked and not suspended
        else //ready and suspended
            p->state = BLOCKED_AND_SUSPENDED; // update PCB state to blocked and suspended
        pcb_insert(p); // insert PCB into blocked queue
    }
}
void pcb_unblock(const char* name){
    if(pcb_find(name) == NULL)
        error("Process does not exist.");
    else if(pcb_find(name)->state != BLOCKED_NOT_SUSPENDED || pcb_find(name)->state != BLOCKED_AND_SUSPENDED)
        error("Process is not blocked.");
    else{
        pcb* p = pcb_find(name); //find PCB
        pcb_remove(p); // remove PCB from blocked queue to prevent duplicates
        if(p->state == BLOCKED_NOT_SUSPENDED) //blocked and not suspended
            p->state = READY_NOT_SUSPENDED; // update PCB state to unblocked and not suspended
        else               //blocked and suspended
            p->state = READY_AND_SUSPENDED; // update PCB state to unblocked and suspended
        pcb_insert(p); // insert PCB into blocked queue
    }
}
void pcb_suspend(const char* name){
    if(pcb_find(name) == NULL)
        error("Process does not exist.");
    else if(pcb_find(name)->state == READY_AND_SUSPENDED || pcb_find(name)->state == BLOCKED_AND_SUSPENDED)
        error("Process is already suspended.");
    else if(pcb_find(name)->type == READY_NOT_SUSPENDED)
        error("Cannot delete system process.");
    else{
        pcb* p = pcb_find(name); //find PCB
        pcb_remove(p); // remove PCB from suspended queue to prevent duplicates
        if(p->state == READY_NOT_SUSPENDED) //ready and not suspended
            p->state = READY_AND_SUSPENDED; // update PCB state to ready and suspended
        else               //blocked and unsuspended
            p->state = BLOCKED_AND_SUSPENDED; // update PCB state to blocked and suspended
        pcb_insert(p); // insert PCB into suspended queue
    }
}

// void pcb_resume(const char* name){

// }
// void pcb_set_priority(const char* name, int priority){

// }
// void pcb_show_pcb(const char* name){

// }
// void pcb_show_ready(){

// }
// void pcb_show_blocked(){

// }
// void pcb_show_all(){

// }
