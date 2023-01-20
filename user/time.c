#include <time.h>
#include <string.h>
#include <stdlib.h>

void time(const char *args) {
    if (strcmp(args, "") == 0) {
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