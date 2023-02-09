#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <help.h>
#include <ctype.h>
#include <shutdown.h>
#include <time.h>
#include <date.h>
#include <memory.h>
#include <mpx/io.h>
#include <pcb_user.h>

#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

void commhand()
{
char* line1="    ███████     █████████              █████████               █████            \n";           
char* line2="  ███░░░░░███  ███░░░░░███            ███░░░░░███             ░░███             \n"; 
char* line3=" ███     ░░███░███    ░░░            ███     ░░░   ██████   ███████   █████████ \n"; 
char* line4="░███      ░███░░█████████           ░███          ███░░███ ███░░███  ░█░░░░███  \n"; 
char* line5="░███      ░███ ░░░░░░░░███          ░███    █████░███ ░███░███ ░███  ░   ███░   \n"; 
char* line6="░░███     ███  ███    ░███          ░░███  ░░███ ░███ ░███░███ ░███    ███░   █ \n"; 
char* line7=" ░░░███████░  ░░█████████  █████████ ░░█████████ ░░██████ ░░████████  █████████ \n"; 
char* line8="   ░░░░░░░     ░░░░░░░░░  ░░░░░░░░░   ░░░░░░░░░   ░░░░░░   ░░░░░░░░  ░░░░░░░░░  \n";
                                                                                
outb(COM1, '\n');
sys_req(WRITE,COM1,YELLOW, strlen(YELLOW));
sys_req(WRITE,COM1,line1, strlen(line1));
sys_req(WRITE,COM1,line2, strlen(line2));
sys_req(WRITE,COM1,line3, strlen(line3));
sys_req(WRITE,COM1,line4, strlen(line4));
sys_req(WRITE,COM1,line5, strlen(line5));
sys_req(WRITE,COM1,line6, strlen(line6));
sys_req(WRITE,COM1,line7, strlen(line7));
sys_req(WRITE,COM1,line8, strlen(line8));
sys_req(WRITE,COM1,RESET, strlen(RESET));
outb(COM1, '\n');
outb(COM1, '\n');
                                                                               
    char prompt[] = "> ";
    char *comp_date = getdate();

    while (1) {
        char buffer[100] = {0};
        buffer[99] = '\0';

        sys_req(WRITE, COM1, prompt, sizeof(prompt));
        sys_req(READ, COM1, buffer, sizeof(buffer));

        // begin parsing buffer
        buffer[strlen(buffer) - 2] = ' ';
        char command_str[20] = {0}; // init char array
        command_str[19] = '\0'; // null terminator at end of string

        int spaces = 0;
        while (isspace(buffer[spaces])) { // count leading spaces infront of command in the buffer
            spaces++;
        }
        
        for (int i = 0; !isspace(buffer[spaces + i]); i++) { // capture the command on buffer
            command_str[i] = buffer[spaces + i];
        }

        if(strcmp(command_str,"pcb") == 0){ //command is PCB
            char pcb_str[50] = {0};
            pcb_str[49] = '\0';
            for(int i = 0;(i+spaces+strlen(command_str))<strlen(buffer);i++){
                pcb_str[i] = buffer[(i+spaces+strlen(command_str))];
            }
            pcb(pcb_str, strlen(pcb_str));
        }
        else{
            strtok(buffer, " ");                // capture parameter args
            char *param_str = strtok(NULL, " ");// capture argument after help
            char *extra_arg = strtok(NULL, " ");// test for extra args
            if (strcmp(extra_arg, NULL) != 0 && strcmp(extra_arg, "\n") != 0) { // check for extra arguments in buffer
                error("The command you entered is not recognized. Too many arguments. Try again.");
                continue;
            }
            if(strcmp(command_str, "version") == 0 && strcmp(param_str, "\n") == 0) { // buffer command is version
                version();
                sys_req(WRITE, COM1, comp_date, strlen(comp_date));
                sys_req(WRITE, COM1, "\r\n", 2);
            }else if(strcmp(command_str, "help") == 0) { // buffer command is help
                help(param_str);
            }else if(strcmp(command_str, "shutdown") == 0) { // buffer command is shutdown
                if (shutdown() == 0) break;
            }else if(strcmp(command_str, "time") == 0) { // buffer command is time
                time(param_str);
            }else if(strcmp(command_str, "date") == 0) { // buffer command is date
                date(param_str);
            }else{                                   // not a command
                error("The command you entered is not recognized. Try again.");
            }
        }
    }
    sys_free_mem(comp_date);
}
