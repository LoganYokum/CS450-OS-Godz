#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <version.h>
#include <help.h>
#include <ctype.h>
#include <shutdown.h>

void commhand()
{
    char prompt[] = "> ";

    while (1) {
        char buffer[100] = {0};
        int len = 100;

        sys_req(WRITE, COM1, prompt, sizeof(prompt));
        sys_req(READ, COM1, buffer, len);

        char *token = strtok(buffer, " ");
        char command[10];
        char args[10];
        int count = 0;

        while (token != NULL) {
            if (strcmp(token, "help") == 0) {
                if (count == 0) {
                    strcpy(command, token);
                    count++;
                }else{
                    println("Invalid command");
                }
            }else if (strcmp(token, "version") == 0) {
                if (count == 0) {
                    strcpy(command, token);
                    count++;
                }else {
                    strcpy(args, token);
                }
            }else if (strcmp(token, "shutdown") == 0){
                if (count == 0) {
                    strcpy(command, token);
                    count++;
                }else {
                    strcpy(args, token);
                }
            }
            sys_req(WRITE, COM1, command, sizeof(command));
            sys_req(WRITE, COM1, args, sizeof(args));
            token = strtok(NULL, " ");
        }

        if (strcmp(command, "help") == 0) {
            help(args);
        }else if (strcmp(command, "version") == 0) {
            if (strlen(args) > 0) {
                println("Invalid command");
            }else {
                version();
            }
        }else if (strcmp(command, "shutdown") == 0) {
            if (strlen(args) > 0) {
                println("Invalid command");
            }else {
                // shutdown();
            }
        }else if (strcmp(command, "time") == 0) {
            // TODO: Implement time
        }
    }
}
