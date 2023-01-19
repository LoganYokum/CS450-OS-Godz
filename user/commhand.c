#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <help.h>
#include <ctype.h>

void commhand() {
    char prompt[] = "> ";

    while (1) {
        char buffer[100] = {0};
        int len = 100;

        sys_req(WRITE, COM1, prompt, sizeof(prompt));
        //int nread = 
        sys_req(READ, COM1, buffer, len);
        //sys_req(WRITE, COM1, buffer, nread);

        //begin parsing
        int i = 0;
        char command_str[10] = {0};
        command_str[9] = '\0';
        while(isspace(buffer[i])==0){//while not a space
            command_str[i] = buffer[i];
            i++;
        }
        if(strcmp(command_str,"version")==0){
            version();
        }
        else if(strcmp(command_str,"help")==0){
            //should only print if only help is listed.
            help();
            const char delimeter[2] = {' '}; //parse by spaces to get each param of the command.
            char* token; 
            token = strtok(buffer,delimeter);//should capture "help"

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
        else if(strcmp(command_str,"shutdown")==0){

        }
        else if(strcmp(command_str,"time")==0){
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
