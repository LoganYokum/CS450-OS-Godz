#include <time.h>
#include <string.h>
#include <stdlib.h>

void date(char *args) {
    if (strcmp(args, "") == 0) {

        println("current date");
    } else {
        int i = 0;
        int date[3] = {0};
        char *token = strtok(args, ":");
        while (token != NULL) {
            // should resolve leading zeros
            if (*token == '0') {
                token++;
            }
            date[i] = atoi(token);
            token = strtok(NULL, ":");
            i++;
        }
        if (i != 3) {
            println("Invalid date format. Use mm:dd:yyyy");
            return;
        }
        if (date[0] < 1 || date[0] > 12) {
            println("Invalid month. Use 0-12");
            return;
        }
        if (date[1] < 1 || date[1] > 31) {
            println("Invalid day. Use 0-31");
            return;
        }
        if (date[0] == 2 && date[1] > 28) {
            println("Invalid date. Use 0-28 for days of February");
        }
        if ((date[0] == 4 || date[0] == 6 || date[0] == 9 || date[0] == 11) && date[1] > 30) {
            println("Invalid date. Use 0-30 for days of month given");
        }
        if ((date[0] == 1 || date[0] == 3 || date[0] == 5 || date[0] == 7 || date[0] == 8 || date[0] == 10 || date[0] == 12) && date[1] > 31) {
            println("Invalid date. Use 0-31 for days of month given");
        }
        if (date[2] < 1970 || date[2] > 2023) {
            println("Invalid year. Use 1970-2023");
            return;
        }
    }
}

void time(char *args) {
    if (strcmp(args, "") == 0) {

        println("current time");
    } else {
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