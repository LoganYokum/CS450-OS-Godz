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
        while(isspace(buffer[i])==0){
            command_str[i] = buffer[i];
            i++;
        }
        if(strcmp(command_str,"version")==0){
            version();
        }
        else if(strcmp(command_str,"help")==0){
            help();
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
