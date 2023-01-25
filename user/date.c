#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>

void date(char *args) {
    if (strcmp(args, "\n") == 0) {
        println("Current Date: ");
        /**
            READ CLOCK
        */
        // get month
        outb(0x70,0x07);
        unsigned char month = inb(0x71);
        int ascii = month;
        sys_req(WRITE,COM1,ascii,sizeof(ascii));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));

        //get day
        outb(0x70,0x06);
        unsigned char day = inb(0x71);
        sys_req(WRITE,COM1,day,sizeof(day));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));

        //get year
        outb(0x70,0x09);
        unsigned char year = inb(0x71);
        sys_req(WRITE,COM1,year,sizeof(year));
        sys_req(WRITE,COM1,"\n",sizeof("\n"));
    } else {
        int month = atoi(strtok(args,":"));
        int day = atoi(strtok(NULL,":"));
        int year = atoi(strtok(NULL," "));
        if (strlen(args) > 10) {
            println("Invalid date format. Use mm:dd:yyyy");
            return;
        }
        else if (month < 1 || month > 12) {
            println("Invalid month. Use 1-12");
            return;
        }
        else if (day < 1 || day > 31) {
            println("Invalid day. Use 1-31");
            return;
        }
        else if (month == 2 && day > 28) {
            println("Invalid date. Use 1-28 for days of February");
        }
        else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            println("Invalid date. Use 1-30 for days of month given");
        }
        else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) {
            println("Invalid date. Use 1-31 for days of month given");
        }
        else if (year < 1970 || year > 2023) {
            println("Invalid year. Use 1970-2023");
            return;
        }
        else{
            println("here");
            //CONVERT VALUES INTO BCD


            /**
                WRITE CLOCK
            */
            //write month
            cli();
            outb(0x70,0x07);
            outb(0x71,0x00); //fill in 0x00 with write value
            
            // write day
            outb(0x70,0x06);
            outb(0x71,0x00); //fill in 0x00 with write value
            
            //write year
            outb(0x70,0x09);
            outb(0x71,0x80); //fill in 0x00 with write value
            sti();
        }
    }
}