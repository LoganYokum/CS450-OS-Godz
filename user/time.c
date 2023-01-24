#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>

void time(char *args) {
    /**
        READ CLOCK
    */
    // get seconds
    outb(0x70,0x00);
    unsigned char sec = inb(0x71);
    println(itoa(sec));

    //get minutes
    outb(0x70,0x02);
    unsigned char min = inb(0x71);
    println(itoa(min));

    //get hours
    outb(0x70,0x04);
    unsigned char hour = inb(0x71);
    println(itoa(hour));

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