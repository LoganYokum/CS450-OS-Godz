#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>

void time(char *args) {
    /**
        READ CLOCK
    */
    // get seconds
    outb(0x70,0x00);
    unsigned char sec = inb(0x71);
    sys_req(WRITE,COM1,sec,sizeof(sec));
    sys_req(WRITE,COM1,"\n",sizeof("\n"));
    // int sec1 = sec;
    // int a = sec1%16;
    // int b = sec1/16;
    // char *a = atoi(a);
    // char *b = atoi(b);
    // println(a);

    //get minutes
    outb(0x70,0x02);
    unsigned char min = inb(0x71);
    sys_req(WRITE,COM1,min,sizeof(min));
    sys_req(WRITE,COM1,"\n",sizeof("\n"));

    //get hours
    outb(0x70,0x04);
    unsigned char hour = inb(0x71);
    sys_req(WRITE,COM1,hour,sizeof(hour));
    sys_req(WRITE,COM1,"\n",sizeof("\n"));

    /**
        WRITE CLOCK
    */
    //write sec
    cli();
    outb(0x70,0x00);
    outb(0x71,0x00);
    
    // write min
    outb(0x70,0x02);
    outb(0x71,0x00);
    
    //write hour
    outb(0x70,0x04);
    outb(0x71,0x80);
    sti();

    if (strcmp(args, "\n") == 0) { 
        // need to implement getting time from RTC
        println("current time");
    }else {
        int i = 0;
        int time[3] = {0};
        char *token = strtok(args, ":");
        while (token != NULL) {
            // should resolve leading zeros
            if (*token == '0') {
                token++;
            }
            time[i] = atoi(token);
            token = strtok(NULL, ":");
            i++;
        }
        if (i != 3) {
            println("Invalid time format. Use hh:mm:ss");
            return;
        }
        if (time[0] < 0 || time[0] > 23) {
            println("Invalid hour. Use 0-23");
            return;
        }
        if (time[1] < 0 || time[1] > 59) {
            println("Invalid minute. Use 0-59");
            return;
        }
        if (time[2] < 0 || time[2] > 59) {
            println("Invalid second. Use 0-59");
            return;
        }
    }
}