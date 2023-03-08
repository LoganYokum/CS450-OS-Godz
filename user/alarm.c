#include <alarm.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

char *gettime(); 

void alarm(char* time, char* message){
 if (strlen(time) != 8 || (time[2] != ':' || time[5] != ':')) {
            error("Invalid time format. Use HH:MM:SS");
            return;
        }
        char *hour_str = strtok(time, ":");
        char *minute_str = strtok(NULL, ":");
        char *second_str = strtok(NULL, " ");

          if (!validnum(hour_str) || !validnum(minute_str) || !validnum(second_str)) {
            error("Invalid character format. Must use numbers only.");
            return;
        }

        int hour = atoi(hour_str);
        int minute = atoi(minute_str);
        int second = atoi(second_str);

        if (hour < 0 || hour > 23) {
            error("Invalid hour. Use 0-23");
            return;
        }
        if (minute < 0 || minute > 59) {
            error("Invalid minute. Use 0-59");
            return;
        }
        if (second < 0 || second > 59) {
            error("Invalid second. Use 0-59");
            return;
        }
   
        char *curr_time = gettime();
        char *curr_hour_str = strtok(curr_time, ":");
        char *curr_minute_str = strtok(NULL, ":");
        char *curr_second_str = strtok(NULL, " ");

        int curr_hour = atoi(curr_hour_str);
        int curr_minute = atoi(curr_minute_str);
        int curr_second = atoi(curr_second_str);

        // decided that the set alarm time must be in the future, if not 
        if (curr_hour > hour){
            error("Invalid time. Alarm time must be in the future.");
            return;
        } 
        if (curr_minute > minute){
            error("Invalid time. Alarm time must be in the future.");
            return;
        }
        if (curr_second > second){
            error("Invalid time. Alarm time must be in the future.");
            return;
        }

        // after input checking, we can create a new alarm process and send it the time and message
        
        sys_req(WRITE,COM1,time,strlen(time));
        sys_req(WRITE,COM1,message,strlen(message));
    }
