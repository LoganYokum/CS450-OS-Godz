#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <mpx/interrupts.h>
#include <string.h>
#include <sys_req.h>
#include <memory.h>

#define MONTH_INDEX 0x08
#define DAY_INDEX 0x07
#define YEAR_INDEX 0x09

char *getdate();

void date(char *args) {
    if (strcmp(args, "\n") == 0) {
        char *date = getdate();
        println(date);
        
        sys_free_mem(date);
    } else {
        if (strlen(args) > 8 || (args[2] != '/' || args[5] != '/')) {
            println("Invalid date format. Use mm/dd/yy");
            return;
        }

        char *month_str = strtok(args, "/");
        char *day_str = strtok(NULL, "/");
        char *year_str = strtok(NULL, " ");

        if (!validnum(month_str) || !validnum(day_str) || !validnum(year_str)) {
            println("Invalid character format. Must use numbers only.");
            return;
        }

        int month = atoi(month_str);
        int day = atoi(day_str);
        int year = atoi(year_str);

        if (month < 1 || month > 12) {
            println("Invalid month. Use 1-12");
            return;
        }
        if (day < 1 || day > 31) {
            println("Invalid day. Use 1-31");
            return;
        }
        if (month == 2 && day > 28) {
            println("Invalid date. Use 1-28 for days of February");
            return;
        }
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            println("Invalid date. Use 1-30 for days of month given");
            return;
        }
        if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) {
            println("Invalid date. Use 1-31 for days of month given");
            return;
        }
        if (year > 99 || year < 0) {
            println("Invalid year. Use 1970-2023");
            return;
        }
        println("Date Set.");
        //write month
        cli();
        outb(0x70, MONTH_INDEX);
        outb(0x71,(unsigned char)htod(month)); 
            
        // write day
        outb(0x70, DAY_INDEX);
        outb(0x71,(unsigned char)htod(day)); 
            
        //write year
        outb(0x70, YEAR_INDEX);
        outb(0x71,(unsigned char)htod(year)); 
        sti();
    }
}

char *getdate() {
    char *date = sys_alloc_mem(9);
    date[8] = '\0';

    // get month
    outb(0x70, MONTH_INDEX);
    unsigned char month = inb(0x71); 
    char *month_str =  itoa(dtoh(month));
    
    //get day
    outb(0x70, DAY_INDEX);
    unsigned char day = inb(0x71);
    char *day_str = itoa(dtoh(day));

    //get year
    outb(0x70, YEAR_INDEX);
    unsigned char year = inb(0x71);
    char *year_str = itoa(dtoh(year));

    if (month < 10){
        date[0] = '0';
        date[1] = month_str[0];
    }else {
        date[0] = month_str[0];
        date[1] = month_str[1];
    }
    date[2] = '/';

    if (day < 10){
       date[3] = '0';
       date[4] = day_str[0];
    }else {
         date[3] = day_str[0];
         date[4] = day_str[1];
    }
    date[5] = '/';

    if (year < 10){
       date[6] = '0';
       date[7] = year_str[0];
    }else {
        date[6] = year_str[0];
        date[7] = year_str[1];
    }

    sys_free_mem(month_str);
    sys_free_mem(day_str);
    sys_free_mem(year_str);

    return date;
}