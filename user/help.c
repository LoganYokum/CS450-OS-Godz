#include <help.h>
#include <stdlib.h>
#include <string.h>

//Note: For easier readability, help() is split into 4 println statements.
void help(char *str){
    if(str==NULL){
        println("The list of commands you can recieve help on include:");
        println("1. help");
        println("2. shutdown");
        println("3. time");
        println("4. version");
    }
    const char delimeter[2] = {' '}; //parse by spaces to get each param of the command.
    char* token; 
    token = strtok(str,delimeter);//should capture "help"

    while(token!=NULL){
        token = strtok(NULL,delimeter);//should capture the commands they want help with
        if(strcmp(token,"help")==0){
            println("To use help, simply type help followed by the command you want help with.");
            println("Example: help time");
        }
        else if(strcmp(token,"shutdown")==0){
            println("To use shutdown, simply type shutdown and the system will terminate.");
        }
        else if(strcmp(token,"version")==0){
            println("To use version, simply type version and the current version will display.");
        }
        else if(strcmp(token,"time")==0){
            println("To use time, simply type time followed by the month, day, and year separated by spaces.");
        }
        else{
            println("Incorrect parameter(s) for command: help. Try again.");
        }
    }
}