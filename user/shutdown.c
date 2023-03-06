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
        pcb *ready_cur = ready_head;
        pcb *suspended_ready_cur = suspended_ready_head;
        pcb* remove_cur = NULL;

        if (ready_cur == NULL && suspended_ready_cur == NULL) {
            //nothing in the ready or suspend queues
            return 0;
        }
        //remove all ready processes
        while (ready_cur != NULL) {
            remove_cur = ready_cur;
            pcb_remove(remove_cur);
            pcb_free(remove_cur);
            ready_cur = ready_cur->next;
        }
        //remove all suspended processes
        while (suspended_ready_cur != NULL) {
            remove_cur = suspended_ready_cur;
            pcb_remove(remove_cur);
            pcb_free(remove_cur);
            suspended_ready_cur = suspended_ready_cur->next;
        }
        pcb_show_all();
        return 0;
    }
    else {                                                   // shutdown not confirmed
        error("Shutdown cancelled.");
        return 1;
    }
}