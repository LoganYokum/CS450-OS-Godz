#include <help.h>
#include <stdlib.h>
#include <string.h>
#include <mpx/io.h>
#include <sys_req.h>

void help(char *args){
    if  (strcmp(args, "\n") == 0) {    // only the argument string contains \n and no extra params
        println("The list of commands you can receive help on include:");
        println("1. help");
        println("2. shutdown");
        println("3. time");
        println("4. date");
        println("5. version");
        println("6. pcb");
        println("7. loadr3");
        // println("8. yield");
        println("8. alarm");
        println("9. allocate");
        println("10. free");
        println("11. show (allocated OR free)");
        return;
    }                          // the param string contains a argument and none extra
    if (strcmp(args, "help") == 0){ // param = help
        println("To use help, simply type help followed by the command you want help with.");
        println("Example: help time");
    }else if (strcmp(args, "shutdown") == 0){ // param = shutdown
        println("To use shutdown, simply type shutdown, follow the confirmation, and the system will terminate.");
        println("Example: shutdown");
    }else if (strcmp(args, "version") == 0){  // param = version
        println("To use version, simply type version and the current version and compilation will display.");
        println("Example: version");
    }else if (strcmp(args, "time") == 0){     // param = time
        println("To use time, simply type time followed by the hours, minutes, and seconds separated by colons) or\njust type time to display the current time");
        println("Example: time OR time 08:45:59");
    }else if (strcmp(args, "date") == 0){     // param = date
        println("To use date, simply type date followed by the month, day, and year separated by slashes or\njust type date to display the current date");
        println("Example: date OR date 09/25/01");
    }else if(strcmp(args, "pcb") == 0){       // param = pcb
        println("To use pcb, type pcb followed by an argument. The arguments for pcb are:\n1. delete name (to delete a pcb by name)\n2. block name  (to block a pcb by name)\n3. suspend name(to suspend a pcb by name)\n4. resume name (to resume a pcb by name after suspended)\n5. set priority name priority number (to set the priority of a pcb)\n6. show pcb name (to show all details of a pcb)\n7. show ready   (to show all ready pcb's)\n8. show blocked     (to show all blocked pcb's)\n9. show all    (to show all pcb's)\n10. *NO LONGER SUPPORTED* create name class priority (to create a new pcb)");
    }
    else if(strcmp(args, "loadr3") == 0){     // param = loadr3
        println("*ONLY FOR R3* To use loadr3, simply type loadr3 and the system will load the r3 program.");
    }
    // else if(strcmp(args, "yield") == 0){      // param = yield
    //     println("*ONLY FOR R3* To use yield, simply type yield and the system will idle the current process.");
    // }
    else if(strcmp(args, "alarm") == 0){      // param = alarm
        println("To use alarm, type alarm followed by two arguments. Time (when to display the message) and Message (what to display).");
        println("Example: alarm 10:00:00 Time to wake up!");
    }
    else if(strcmp(args, "allocate") == 0){   // param = allocate"))
        println("To use allocate, type allocate followed by an argument. The argument for allocate is the size of the memory block you want to allocate.");
        println("Example: allocate 100");
    }
    else if(strcmp(args, "free") == 0){       // param = free
        println("To use free, type free followed by an argument. The argument for free is the address of the memory block you want to free.");
        println("Example: free 00000000");
    }
    else if(strcmp(args, "show") == 0){       // param = show
        println("To use show, type show followed by an argument. The arguments for show are:\n1. allocated (to show all allocated memory blocks)\n2. free (to show all free memory blocks)");
        println("Example: show allocated or show free");
    }
    else {                                //param doesnt have any matching string
        println("Incorrect parameter(s) for command: help. Try again.");
    }
}