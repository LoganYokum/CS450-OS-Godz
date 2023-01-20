#include <help.h>
#include <stdlib.h>
#include <string.h>

void help(char str[]){
    if(strlen(str)==2){//only the argument string contains \r and \n
        println("The list of commands you can recieve help on include:");
        println("1. help");
        println("2. shutdown");
        println("3. time");
        println("4. version");
        return;
    }
    else{//the param string contains a argument
        if(strcmp(str,"help")==0){
            println("To use help, simply type help followed by the command you want help with.");
            println("Example: help time");
        }
        else if(strcmp(str,"shutdown")==0){
            println("To use shutdown, simply type shutdown, follow the confirmation, and the system will terminate.");
            println("Example: shutdown");
        }
        else if(strcmp(str,"version")==0){
            println("To use version, simply type version and the current version and compilation will display.");
            println("Example: version");
        }
        else if(strcmp(str,"time")==0){
            println("To use time, simply type time followed by get or set (followed by the hours, minutes, and seconds separated by colons).");
            println("Example: time get or time set 08:45:59");
        }
        else if(strcmp(str,"date")==0){
            println("To use date, simply type date followed by get or set (followed by the month, day, and year separated by colons).");
            println("Example: date get or date set 09:25:2001");
        }
        else{
            println("Incorrect parameter(s) for command: help. Try again.");
        }
        return;
    }    
}