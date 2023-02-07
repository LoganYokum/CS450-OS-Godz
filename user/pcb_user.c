#include <pcb_user.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <sys_req.h>
#include <mpx/pcb.h>
#include <memory.h>

void pcb(const char* param_str, const char* pcb_name, const char* pcb_class,
            const char* pcb_priority, const char* pcb_state, const char* pcb_suspended_status){
    if(strcmp(param_str, "create") == 0)
        pcb_create(pcb_name, pcb_class, pcb_priority);
    else if(strcmp(param_str, "delete") == 0)
        pcb_delete(pcb_name);
    else if(strcmp(param_str, "block") == 0)
        pcb_block(pcb_name);
    else if(strcmp(param_str, "unblock") == 0)
        pcb_unblock(pcb_name);
    else if(strcmp(param_str, "suspend") == 0)
        pcb_suspend(pcb_name);
    else if(strcmp(param_str, "resume") == 0)
        pcb_resume(pcb_name);
    else if(strcmp(param_str, "set_priority") == 0)
        pcb_set_priority(pcb_name, pcb_class, pcb_priority, pcb_state, pcb_suspended_status);
    else if(strcmp(param_str, "show_pcb") == 0)
        pcb_show_pcb(pcb_name, pcb_class, pcb_priority, pcb_state, pcb_suspended_status);
    else if(strcmp(param_str, "show_ready") == 0)
        pcb_show_ready(pcb_name, pcb_class, pcb_priority, pcb_state, pcb_suspended_status);
    else if(strcmp(param_str, "show_blocked") == 0)
        pcb_show_blocked(pcb_name, pcb_class, pcb_priority, pcb_state, pcb_suspended_status);
    else if(strcmp(param_str, "show_all") == 0)
        pcb_show_all(pcb_name, pcb_class, pcb_priority, pcb_state, pcb_suspended_status);
    else
        error("Incorrect parameter(s) for command: pcb. Try again.");
}

void pcb_create(const char* name, int class, int priority,){

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
