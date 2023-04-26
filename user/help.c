#include <help.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <mpx/io.h>
#include <sys_req.h>

void help(char *args){
    char *help = sys_alloc_mem(1000);
    if  (args == 0 || strcmp(args, "\n") == 0) {    // only the argument string contains \n and no extra params
        strcpy(help, "The list of commands you can receive help on include:\n");
        strcat(help, "1. help\n");
        strcat(help, "2. shutdown\n");
        strcat(help, "3. time\n");
        strcat(help, "4. date\n");
        strcat(help, "5. version\n");
        strcat(help, "6. pcb\n");
        // strcat(help, "7. loadr3\n");
        strcat(help, "7. alarm\n");
        strcat(help, "8. allocate\n");
        strcat(help, "9. free\n");
        strcat(help, "10. show (allocated OR free)\n");
        sys_req(WRITE, COM1, help, strlen(help));
        sys_free_mem(help); //free memory
        return;
    } // the param string contains a argument and none extra
    if (strcmp(args, "help") == 0){ // param = help
        strcpy(help, "To use help, simply type help followed by the command you want help with.\n");
        strcat(help, "Example: help time\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }else if (strcmp(args, "shutdown") == 0){ // param = shutdown
        strcpy(help, "To use shutdown, simply type shutdown, follow the confirmation, and the system will terminate.\n");
        strcat(help, "Example: shutdown\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }else if (strcmp(args, "version") == 0){  // param = version
        strcpy(help, "To use version, simply type version and the current version and compilation will display.\n");
        strcat(help, "Example: version\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }else if (strcmp(args, "time") == 0){     // param = time
        strcpy(help, "To use time, simply type time followed by the hours, minutes, and seconds separated by colons) or\njust type time to display the current time\n");
        strcat(help, "Example: time OR time 08:45:59\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }else if (strcmp(args, "date") == 0){     // param = date
        strcpy(help, "To use date, simply type date followed by the month, day, and year separated by slashes or\njust type date to display the current date\n");
        strcat(help, "Example: date OR date 09/25/01\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }else if(strcmp(args, "pcb") == 0){       // param = pcb
        //2. *NO LONGER SUPPORTED* block name  (to block a pcb by name)\n
        //10. *NO LONGER SUPPORTED* create name class priority (to create a new pcb)\n
        //11. *NO LONGER SUPPORTED* unblock name  (to block a pcb by name)\n
        strcpy(help, "To use pcb, type pcb followed by an argument. The arguments for pcb are:\n1. delete name (to delete a pcb by name)\n2. suspend name(to suspend a pcb by name)\n3. resume name (to resume a pcb by name after suspended)\n4. set priority name priority number (to set the priority of a pcb)\n5. show pcb name (to show all details of a pcb)\n6. show ready   (to show all ready pcb's)\n7. show blocked     (to show all blocked pcb's)\n8. show all    (to show all pcb's)\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    // else if(strcmp(args,"yield") == 0){
    //     strcpy(help, "To use yield, simply type yield and the system will yield the current process.\n");
    //     sys_req(WRITE, COM1, help, strlen(help));
    // }
    else if(strcmp(args, "loadr3") == 0){     // param = loadr3
        strcpy(help, "*ONLY FOR R3* To use loadr3, simply type loadr3 and the system will load the r3 program.\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    else if(strcmp(args, "alarm") == 0){      // param = alarm
        strcpy(help, "To use alarm, type alarm followed by two arguments. Time (when to display the message) and Message (what to display).\n");
        strcat(help, "Example: alarm 10:00:00 Time to wake up!\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    else if(strcmp(args, "allocate") == 0){   // param = allocate"))
        strcpy(help, "To use allocate, type allocate followed by an argument. The argument for allocate is the size of the memory block you want to allocate.\n");
        strcat(help, "Example: allocate 100\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    else if(strcmp(args, "free") == 0){       // param = free
        strcpy(help, "To use free, type free followed by an argument. The argument for free is the address of the memory block you want to free.\n");
        strcat(help, "Example: free 0x0a123000\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    else if(strcmp(args, "show") == 0){       // param = show
        strcpy(help, "To use show, type show followed by an argument. The arguments for show are:\n1. allocated (to show all allocated memory blocks)\n2. free (to show all free memory blocks)\n");
        strcat(help, "Example: show allocated or show free\n");
        sys_req(WRITE, COM1, help, strlen(help));
    }
    else {                                //param doesnt have any matching string
        error("Incorrect parameter(s) for command: help. Try again.\n");
    }
    sys_free_mem(help); //free memory
}