#include <help.h>
#include <stdlib.h>
#include <string.h>
#include <mpx/io.h>
#include <sys_req.h>

void help(char *args){
    if(strcmp(args, "\n") == 0){    // only the argument string contains \r and \n and no extra params
        println("The list of commands you can recieve help on include:");
        println("1. help");
        println("2. shutdown");
        println("3. time");
        println("4. date");
        println("5. version");
        return;
    }
    else{                           // the param string contains a argument and none extra
        if(strcmp(args,"help")==0){ // param = help
            println("To use help, simply type help followed by the command you want help with.");
            println("Example: help time");
        }
        else if(strcmp(args,"shutdown")==0){ // param = shutdown
            println("To use shutdown, simply type shutdown, follow the confirmation, and the system will terminate.");
            println("Example: shutdown");
        }
        else if(strcmp(args,"version")==0){  // param = version
            println("To use version, simply type version and the current version and compilation will display.");
            println("Example: version");
        }
        else if(strcmp(args,"time")==0){     // param = time
            println("To use time, simply type time followed by get or set (followed by the hours, minutes, and seconds separated by colons).");
            println("Example: time get or time set 08:45:59");
        }
        else if(strcmp(args,"date")==0){     // param = date
            println("To use date, simply type date followed by get or set (followed by the month, day, and year separated by colons).");
            println("Example: date get or date set 09:25:2001");
        }
        else{                                //param doesnt have any matching string
            println("Incorrect parameter(s) for command: help. Try again.");
        }
        return;
    }    
}