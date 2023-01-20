#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <help.h>
#include <ctype.h>
#include <shutdown.h>
#include <time.h>
#include <mpx/io.h>

void commhand() {
    char prompt[] = "> ";

    while (1) {
        char buffer[100] = {0};
        int len = 100;
        buffer[99] = '\0';

        sys_req(WRITE, COM1, prompt, sizeof(prompt));
        //int nread = 
        sys_req(READ, COM1, buffer, len);
        //sys_req(WRITE, COM1, buffer, nread);

        //begin parsing
        int i = 0;
        char command_str[20] = {0}; //init char array
        command_str[19] = '\0'; //null terminator at end of string
        while(isspace(buffer[i])==0){ //while not a space
            command_str[i] = buffer[i];
            i++;
        }
        if(strcmp(command_str,"version")==0){
            version();
        }
        else if(strcmp(command_str,"help")==0){
            strtok(buffer," ");//capture parameter args
            char *param_str = strtok(NULL," ");//capture argument after help
            char args[strlen(param_str)]; //set argument
            memcpy(args,param_str,strlen(param_str)); //copy param char* to args array
            args[(int)sizeof(args)-3] = '\0';//set null terminator to cut of \r,\n,enter key
            help(args);
        }
        else if(strcmp(command_str,"shutdown")==0){
           int code = shutdown();
           if(code==0){
                break;
           }
           else{
                continue;
           }
        }
        else if(strcmp(command_str,"time")==0){
            const char delimeter[2] = {' '}; //parse by spaces to get each param of the command.
            char* token; 
            token = strtok(buffer,delimeter);
            while(token!=NULL){
                token = strtok(NULL,delimeter);

            }
        }
        else if(strcmp(command_str,"date")==0){
            const char delimeter[2] = {' '}; //parse by spaces to get each param of the command.
            char* token; 
            token = strtok(buffer,delimeter);
            while(token!=NULL){
                token = strtok(NULL,delimeter);

            }
        }
        else{
            println("The command you entered is not recognized. Try again.");
        }
    }
}
