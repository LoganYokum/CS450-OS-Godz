#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <help.h>
#include <ctype.h>
#include <shutdown.h>
#include <time.h>
#include <date.h>

void commhand() {
    char prompt[] = "> ";
    
    while (1) {
        char buffer[100] = {0};
        buffer[99] = '\0';

        sys_req(WRITE, COM1, prompt, sizeof(prompt));
        sys_req(READ, COM1, buffer, sizeof(buffer));

        buffer[strlen(buffer)-2] = ' ';
        //begin parsing
        char command_str[20] = {0}; //init char array
        command_str[19] = '\0'; //null terminator at end of string
        for(int i = 0;isspace(buffer[i])==0;i++){//while not a space
            command_str[i] = buffer[i];
        }
        if(strcmp(command_str,"version")==0){
            version();
        }
        else if(strcmp(command_str,"help")==0){
            strtok(buffer," ");//capture parameter args
            char *param_str = strtok(NULL," ");//capture argument after help
            char *extra_arg = strtok(NULL," ");//test for extra args
            help(param_str,extra_arg);
        }
        else if(strcmp(command_str,"shutdown")==0){
           int code = shutdown();
           if(code==0){
                break;
           }
           continue;
        }
        else if(strcmp(command_str,"time")==0){
            strtok(buffer," ");//capture parameter args
            char *param_str = strtok(NULL," ");//capture argument after help
            char *extra_arg = strtok(NULL," ");//test for extra args
            time(param_str,extra_arg);
        }
        else if(strcmp(command_str,"date")==0){
            strtok(buffer," ");//capture parameter args
            char *param_str = strtok(NULL," ");//capture argument after help
            char *extra_arg = strtok(NULL," ");//test for extra args
            date(param_str,extra_arg);
        }
        else{
            println("The command you entered is not recognized. Try again.");
        }
    }
}
