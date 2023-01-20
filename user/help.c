#include <help.h>
#include <stdlib.h>
#include <string.h>

//Note: For easier readability, help() is split into 4 println statements.
void help(char *args){
    if (strcmp(args, "version") == 0) {
        println("version: Displays the current version of the OS");
    }else if (strcmp(args, "shutdown") == 0) {
        println("shutdown: Shuts down the OS");
    }else if (strcmp(args, "time") == 0) {
        println("time: Displays the current time");
    }else if (strcmp(args, "") == 0) {
        println("help: Displays all the commands");
        println("version: Displays the current version of the OS");
        println("shutdown: Shuts down the OS");
        println("time: Displays the current time");
    }
}