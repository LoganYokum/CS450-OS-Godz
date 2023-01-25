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

        // get month
        outb(0x70,0x08);
        unsigned char month = inb(0x71);//BCD 8bit 0x01
        char* month_str =  itoa(dtoh(month));
        
        //get day
        outb(0x70,0x07);
        unsigned char day = inb(0x71);
        char* day_str = itoa(dtoh(day));

        //get year
        outb(0x70,0x09);
        unsigned char year = inb(0x71);
        char* year_str = itoa(dtoh(year));

        sys_req(WRITE,COM1,month_str,strlen(month_str));
        sys_req(WRITE,COM1,":",strlen(":"));
        sys_req(WRITE,COM1,day_str,strlen(day_str));
        sys_req(WRITE,COM1,":",strlen(":"));
        sys_req(WRITE,COM1,year_str,strlen(year_str));
        outb(COM1,'\r');
        outb(COM1,'\n');

    } else {
        unsigned char month = atoi(strtok(args,":"));
        unsigned char day = atoi(strtok(NULL,":"));
        unsigned char year = atoi(strtok(NULL," "));
        if (strlen(args) > 10) {
            println("Invalid date format. Use mm:dd:yy");
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
        else if (year > 99 || year < 0) {
            println("Invalid year. Use 1970-2023");
            return;
        }
        else{
            println("Setting Date.");
            //write month
            cli();
            outb(0x70,0x08);
            outb(0x71,(unsigned char)htod(month)); 
            
            // write day
            outb(0x70,0x07);
            outb(0x71,(unsigned char)htod(day)); 
            
            //write year
            outb(0x70,0x09);
            outb(0x71,(unsigned char)htod(year)); 
            sti();
        }
    }
}