#include <shutdown.h>
#include <stdlib.h>
#include <commhand.h>
#include <sys_req.h>
#include <string.h>
#include <ctype.h>
#include <mpx/pcb.h>
#include <pcb_user.h>

int shutdown(){

    char shutdown_buf[12] = {0};    // init shutdown buffer char array
    shutdown_buf[11] = '\0';        // null terminator at end of string
    char compare_str[10] = {0};     // init char compare array
    compare_str[9] = '\0';          // null terminator at end of string

    char prompt[] = "> ";
    println("You selected shutdown. Retype shutdown to confirm.");
    sys_req(WRITE, COM1, prompt, sizeof(prompt));           // add prompt to output    
    sys_req(READ,COM1,shutdown_buf,strlen(shutdown_buf));   // read in buffer for confirmation

    shutdown_buf[strlen(shutdown_buf) - 2] = ' ';                   //for strtok 

    int spaces = 0;
    while (isspace(shutdown_buf[spaces])) { // count leading spaces infront of command in the buffer
        spaces++;
    }

    strtok(shutdown_buf, " ");
    char *extra_arg = strtok(NULL, " ");
    if (strcmp(extra_arg, NULL) != 0 && strcmp(extra_arg, "\n") != 0) { // check for extra arguments in buffer
        error("You did not confirm shutdown. Too many arguments passed.");
        return 1;
    }

    for (int i = 0; !isspace(shutdown_buf[spaces + i]); i++) { // capture the command on buffer
        compare_str[i] = shutdown_buf[spaces + i];
    }

    if(strcmp(compare_str, "shutdown") == 0) {                  // compare string for shutdown
        //nothing to remove from queues
        if (ready_head == NULL && suspended_ready_head == NULL && suspended_blocked_head == NULL && blocked_head == NULL) return 0;
        
        //remove all processes from ready queue and suspend queue and blocked queues
        pcb* temp = NULL;
        while(ready_head != NULL) {
            temp = ready_head;
            ready_head = ready_head->next;
            pcb_remove(temp);
        }
        while(suspended_ready_head != NULL) {
            temp = suspended_ready_head;
            suspended_ready_head = suspended_ready_head->next;
            pcb_remove(temp);
        }
        while(suspended_blocked_head != NULL) {
            temp = suspended_blocked_head;
            suspended_blocked_head = suspended_blocked_head->next;
            pcb_remove(temp);
        }
        while(blocked_head != NULL) {
            temp = blocked_head;
            blocked_head = blocked_head->next;
            pcb_remove(temp);
        }
        //free all lists
        list_free(ready_head);
        list_free(suspended_ready_head);
        list_free(suspended_blocked_head);
        list_free(blocked_head);

        return 0;
    }
    else {                                                   // shutdown not confirmed
        error("Shutdown cancelled.");
        return 1;
    }
}