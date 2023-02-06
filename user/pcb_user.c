#include <pcb_user.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <sys_req.h>
#include <mpx/pcb.h>
#include <memory.h>

void pcb(const char* param_str, const char* extra_arg){
    if(strcmp(param_str, "create") == 0){
        pcb_create(extra_arg, 0, 0);
    }
    else if(strcmp(param_str, "delete") == 0){
        pcb_delete(extra_arg);
    }
    else if(strcmp(param_str, "block") == 0){
        pcb_block(extra_arg);
    }
    else if(strcmp(param_str, "unblock") == 0){
        pcb_unblock(extra_arg);
    }
    else if(strcmp(param_str, "suspend") == 0){
        pcb_suspend(extra_arg);
    }
    else if(strcmp(param_str, "resume") == 0){
        pcb_resume(extra_arg);
    }
    else if(strcmp(param_str, "set_priority") == 0){
        pcb_set_priority(extra_arg, 0, 0, 0, 0);
    }
    else if(strcmp(param_str, "show_pcb") == 0){
        pcb_show_pcb(extra_arg, 0, 0, 0, 0);
    }
    else if(strcmp(param_str, "show_ready") == 0){
        pcb_show_ready(extra_arg, 0, 0, 0, 0);
    }
    else if(strcmp(param_str, "show_blocked") == 0){
        pcb_show_blocked(extra_arg, 0, 0, 0, 0);
    }
    else if(strcmp(param_str, "show_all") == 0){
        pcb_show_all(extra_arg, 0, 0, 0, 0);
    }
    else{
        println("Incorrect parameter(s) for command: pcb. Try again.");
    }
}

void pcb_create(const char* name, int priority, int class){

}
void pcb_delete(const char* name){

}
void pcb_block(const char* name){

}
void pcb_unblock(const char* name){

}
void pcb_suspend(const char* name){

}
void pcb_resume(const char* name){

}
void pcb_set_priority(const char* name, int class, int priority, int state, int suspended_status){

}
void pcb_show_pcb(const char* name, int class, int priority, int state, int suspended_status){

}
void pcb_show_ready(const char* name, int class, int priority, int state, int suspended_status){

}
void pcb_show_blocked(const char* name, int class, int priority, int state, int suspended_status){

}
void pcb_show_all(const char* name, int class, int priority, int state, int suspended_status){

}
