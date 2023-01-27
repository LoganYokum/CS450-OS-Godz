#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>

#define SEC_INDEX 0x00
#define MIN_INDEX 0x02
#define HOUR_INDEX 0x04

void time(char *args) {
    if (strcmp(args, "\n") == 0) { 
        // get seconds
        outb(0x70, SEC_INDEX);
        int sec = dtoh(inb(0x71));
        char *sec_str = itoa(sec);

        //get minutes
        outb(0x70, MIN_INDEX);
        int min = dtoh(inb(0x71));
        char *min_str = itoa(min);

        //get hours
        outb(0x70, HOUR_INDEX);
        int hour = dtoh(inb(0x71));
        char *hour_str = itoa(hour);

        sys_req(WRITE,COM1,"Current Time: ",strlen("Current Time: "));
        if (hour < 10) {
            outb(COM1, '0');
        }
        sys_req(WRITE, COM1, hour_str, strlen(hour_str));
        outb(COM1, ':');

        if (min < 10) {
            outb(COM1, '0');
        }
        sys_req(WRITE, COM1, min_str, strlen(min_str));
        outb(COM1, ':');

        if (sec < 10) {
            outb(COM1, '0');
        }
        sys_req(WRITE, COM1, sec_str, strlen(sec_str));
        sys_req(WRITE, COM1, "\r\n", 2);
    }
    else {
        unsigned char hour = atoi(strtok(args,":"));
        unsigned char min = atoi(strtok(NULL,":"));
        unsigned char second = atoi(strtok(NULL," "));
        if (strlen(args) > 8) {
            println("Invalid time format. Use hh:mm:ss");
            return;
        }
        else if (hour < 0 || hour > 23) {
            println("Invalid hour. Use 0-23");
            return;
        }
        else if (min < 0 || min > 59) {
            println("Invalid minute. Use 0-59");
            return;
        }
        else if (second < 0 || second > 59) {
            println("Invalid second. Use 0-59");
            return;
        }
        else{
            println("Time Set.");
            //write sec
            cli();
            outb(0x70, SEC_INDEX);
            outb(0x71, (unsigned char)htod(second)); //fill in 0x00 with write value
            
            // write min
            outb(0x70, MIN_INDEX);
            outb(0x71, (unsigned char)htod(min)); //fill in 0x00 with write value
            
            //write hour
            outb(0x70, HOUR_INDEX);
            outb(0x71, (unsigned char)htod(hour)); //fill in 0x00 with write value
            sti();
        }
    }
}

char* compilation_time(){
    // get seconds
    outb(0x70, SEC_INDEX);
    int sec = dtoh(inb(0x71));
    char *sec_str = itoa(sec);

    // //get minutes
    outb(0x70, MIN_INDEX);
    int min = dtoh(inb(0x71));
    char *min_str = itoa(min);

    //get hours
    outb(0x70, HOUR_INDEX);
    int hour = dtoh(inb(0x71));
    char *hour_str = itoa(hour);

    //concat all char* string together into one using memcpy
    char *compilation_time_str = NULL;
    memcpy(compilation_time_str,hour_str,strlen(hour_str));
    memcpy(compilation_time_str+strlen(hour_str),":",strlen(":")+1);
    memcpy(compilation_time_str+strlen(hour_str)+strlen(":"),min_str,strlen(min_str)+2);
    memcpy(compilation_time_str+strlen(hour_str)+strlen(":")+strlen(min_str),":",strlen(":")+3);
    memcpy(compilation_time_str+strlen(hour_str)+strlen(":")+strlen(min_str)+strlen(":"),sec_str,strlen(sec_str)+4);
    return compilation_time_str;
}