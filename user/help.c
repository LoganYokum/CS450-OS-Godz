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
        println("To use pcb, type pcb followed by an argument. The arguments for pcb are:\n1. create 'name' (to create a pcb by name)\n2. delete 'name' (to delete a pcb by name)\n3. block 'name' (to block a pcb by name)\n4. suspend 'name' (to suspend a pcb by name)\n5. resume 'name' (to resume a pcb by name after suspended)\n6. set priority 'name' 'priority' (to set the priority of a pcb)\n7. show 'name' (to show all details of a pcb)\n8. show ready (to show all ready pcb's)\n9. show blocked (to show all blocked pcb's)\n10. show all (to show all pcb's)");
    }
    else {                                //param doesnt have any matching string
        println("Incorrect parameter(s) for command: help. Try again.");
    }
}