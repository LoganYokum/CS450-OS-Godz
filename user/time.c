#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>

void time(char *args) {
    if (strcmp(args, "\n") == 0) { 
        println("Curent time: ");
        /**
            READ CLOCK
        */
        // get seconds
        outb(0x70,0x00);
        unsigned char sec = inb(0x71);
        char *sec_str = itoa(sec);
        sys_req(WRITE,COM1,sec_str,sizeof(sec_str));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));

        //get minutes
        outb(0x70,0x02);
        unsigned char min = inb(0x71);
        char *min_str = itoa(min);
        sys_req(WRITE,COM1,min_str,sizeof(min_str));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));

        //get hours
        outb(0x70,0x04);
        unsigned char hour = inb(0x71);
        char *hour_str = itoa(hour);
        sys_req(WRITE,COM1,hour_str,sizeof(hour_str));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));
    }
    else {
        int hour = atoi(strtok(args,":"));
        int min = atoi(strtok(NULL,":"));
        int second = atoi(strtok(NULL," "));
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

            //CONVERT VALUES INTO BCD


            /**
                WRITE CLOCK
            */
            //write sec
            cli();
            outb(0x70,0x00);
            outb(0x71,0x00); //fill in 0x00 with write value
            
            // write min
            outb(0x70,0x02);
            outb(0x71,0x00); //fill in 0x00 with write value
            
            //write hour
            outb(0x70,0x04);
            outb(0x71,0x80); //fill in 0x00 with write value
            sti();
        }
    }
}