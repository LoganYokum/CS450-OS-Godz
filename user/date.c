#include <date.h>
#include <string.h>
#include <stdlib.h>

void date(char *args) {
    if (strcmp(args, "\n") == 0) {
        // need to implement getting date from RTC
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
            println("Invalid month. Use 1-12");
            return;
        }
        if (date[1] < 1 || date[1] > 31) {
            println("Invalid day. Use 1-31");
            return;
        }
        if (date[0] == 2 && date[1] > 28) {
            println("Invalid date. Use 1-28 for days of February");
        }
        if ((date[0] == 4 || date[0] == 6 || date[0] == 9 || date[0] == 11) && date[1] > 30) {
            println("Invalid date. Use 1-30 for days of month given");
        }
        if ((date[0] == 1 || date[0] == 3 || date[0] == 5 || date[0] == 7 || date[0] == 8 || date[0] == 10 || date[0] == 12) && date[1] > 31) {
            println("Invalid date. Use 1-31 for days of month given");
        }
        if (date[2] < 1970 || date[2] > 2023) {
            println("Invalid year. Use 1970-2023");
            return;
        }
    }
}