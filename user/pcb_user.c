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

void pcb(char[] pcb_str, int length){
    char* param_str = strtok(pcb_str, " ");
    if(strcmp(param_str, "set")){
        arg_str = strtok(NULL, " ");
        if(strcmp(arg_str, "priority") == 0){
            char* pcb_name = strtok(NULL, " ");
            char* pcb_priority = strtok(NULL, " ");
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_set_priority(pcb_name, atoi(pcb_priority));
        }
        else
            error("Incorrect parameter(s) for command: pcb. Try again.");    
    }
    else if(strcmp(param_str, "show")){
        if(strcmp(arg_str, "pcb") == 0){
            char* pcb_name = strtok(NULL, " ");
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_show_pcb(pcb_name);
        }
        else if(strcmp(arg_str, "ready") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_show_ready();
        }
        else if(strcmp(arg_str, "blocked") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_show_blocked();
        }
        else if(strcmp(arg_str, "all") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_show_all();
        }
        else
            error("Incorrect parameter(s) for command: pcb. Try again.");
    }
    else{
        char* pcb_name = strtok(NULL, " ");
        if(strcmp(param_str, "create") == 0){
            char* pcb_class = strtok(NULL, " ");
            char* pcb_priority = strtok(NULL, " ");
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_create(pcb_name, atoi(pcb_class), atoi(pcb_priority));
        }
        else if(strcmp(param_str, "delete") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_delete(pcb_name);
        }
        else if(strcmp(param_str, "block") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_block(pcb_name);
        }
        else if(strcmp(param_str, "unblock") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_unblock(pcb_name);
        }
        else if(strcmp(param_str, "suspend") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_suspend(pcb_name);
        }
        else if(strcmp(param_str, "resume") == 0){
            char* extra_arg_test = strtok(NULL, " ");
            if(strcmp(extra_arg_test, NULL) == 0)
                error("Incorrect parameter(s) for command: pcb. Try again.");
            else
                pcb_resume(pcb_name);
        }
        else
            error("Incorrect parameter(s) for command: pcb. Try again.");
    }
}

void pcb_create(const char* name, int class, int priority){
    if(strlen(name) > 8)
        error("Name too long. Must be 8 characters or less.");
    else if(class < 0 || class > 1)
        error("Invalid class. Must be 0 or 1.");
    else if(priority < 0 || priority > 9)
        error("Invalid priority. Must be 0-9.");
    else if(pcb_find(name) != NULL)
        error("Process already exists.");
    else{
        //process should be created.
        pcb p* = pcb_setup(name, class, priority);
        pcb_insert(p);
    }
}
void pcb_delete(const char* name){ 
    if(pcb_find(name) == NULL)
        error("Process does not exist.");
    else if(pcb_find(name)->class == 1) //system process not allowed to be deleted
        error("Cannot delete system process.");
    else{
        //process should be deleted.
        pcb* p = pcb_find(name);
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
    else if(pcb_find(name)->class == READY_NOT_SUSPENDED)
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
void pcb_resume(const char* name){

}
void pcb_set_priority(const char* name, int priority){

}
void pcb_show_pcb(const char* name){

}
void pcb_show_ready(){

}
void pcb_show_blocked(){

}
void pcb_show_all(){

}
