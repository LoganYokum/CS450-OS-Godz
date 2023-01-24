#include <shutdown.h>
#include <stdlib.h>
#include <commhand.h>
#include <sys_req.h>
#include <string.h>
#include <ctype.h>

int shutdown(){

    char shutdown_buf[12] = {0};    // init shutdown buffer char array
    shutdown_buf[11] = '\0';        // null terminator at end of string
    char compare_str[10] = {0};     // init char compare array
    compare_str[9] = '\0';          // null terminator at end of string

    char prompt[] = "> ";
    println("You selected shutdown. Retype shutdown to confirm.");
    sys_req(WRITE, COM1, prompt, sizeof(prompt));           // add prompt to output    
    sys_req(READ,COM1,shutdown_buf,strlen(shutdown_buf));   // read in buffer for confirmation

    for(int i = 0;isspace(shutdown_buf[i])==0;i++){         // while not a space
        compare_str[i] = shutdown_buf[i];
    }

    if(strcmp(compare_str,"shutdown")==0){                  // compare string for shutdown
        return 0;
    }                  
    else{                                                   // shutdown not confirmed
        println("You did not confirm shutdown.");
        return 1;
    }
}