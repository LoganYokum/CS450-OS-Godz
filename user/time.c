#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>
#include <memory.h>

#define SEC_INDEX 0x00
#define MIN_INDEX 0x02
#define HOUR_INDEX 0x04

char *gettime();

void time(char *args) {
    if (strcmp(args, "\n") == 0) { 
        char *date = gettime();
        println(date);
        
        sys_free_mem(date);
        
    }
    else {
        if (strlen(args) > 8 || (args[2] != ':' || args[5] != ':')) {
            error("Invalid time format. Use hh:mm:ss");
            return;
        }

        char *hour_str = strtok(args, ":");
        char *min_str = strtok(NULL, ":");
        char *sec_str = strtok(NULL, " ");

        if (!validnum(hour_str) || !validnum(min_str) || !validnum(sec_str)) {
            error("Invalid character format. Must use numbers only.");
            return;
        }

        int hour = atoi(hour_str);
        int min = atoi(min_str);
        int sec = atoi(sec_str);
        
        if (hour < 0 || hour > 23) {
            error("Invalid hour. Use 0-23");
            return;
        }
        if (min < 0 || min > 59) {
            error("Invalid minute. Use 0-59");
            return;
        }
        if (sec < 0 || sec > 59) {
            error("Invalid sec. Use 0-59");
            return;
        }
        success("Time Set.");
        //write sec
        cli();
        outb(0x70, SEC_INDEX);
        outb(0x71, (unsigned char)htod(sec)); //fill in 0x00 with write value
        
        // write min
        outb(0x70, MIN_INDEX);
        outb(0x71, (unsigned char)htod(min)); //fill in 0x00 with write value
        
        //write hour
        outb(0x70, HOUR_INDEX);
        outb(0x71, (unsigned char)htod(hour)); //fill in 0x00 with write value
        sti();
    }
}

char *gettime() {
    char *time = sys_alloc_mem(9);
    time[8] = '\0';

    // get hour
    outb(0x70, HOUR_INDEX);
    unsigned char hour = inb(0x71); 
    char *hour_str =  itoa(dtoh(hour));
    
    //get min
    outb(0x70, MIN_INDEX);
    unsigned char min = inb(0x71);
    char *min_str = itoa(dtoh(min));

    //get sec
    outb(0x70, SEC_INDEX);
    unsigned char sec = inb(0x71);
    char *sec_str = itoa(dtoh(sec));

    if (hour < 10){
        time[0] = '0';
        time[1] = hour_str[0];
    }else {
        time[0] = hour_str[0];
        time[1] = hour_str[1];
    }
    time[2] = ':';

    if (min < 10){
       time[3] = '0';
       time[4] = (strcmp(min_str, "") == 0) ? '0' : min_str[0];
    }else {
         time[3] = min_str[0];
         time[4] = min_str[1];
    }
    time[5] = ':';

    if (sec < 10){
       time[6] = '0';
       time[7] = (strcmp(sec_str, "") == 0) ? '0' : sec_str[0];
    }else {
        time[6] = sec_str[0];
        time[7] = sec_str[1];
    }

    sys_free_mem(hour_str);
    sys_free_mem(min_str);
    sys_free_mem(sec_str);

    return time;
}
