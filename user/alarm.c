#include <alarm.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <mpx/call.h>
#include <mpx/pcb.h>
#include <processes.h>

char *gettime();

void alarm(char *time, char *message)
{

    if (strlen(time) != 8 || (time[2] != ':' || time[5] != ':'))
    {
        error("Invalid time format. Use HH:MM:SS");
        return;
    }
    char *hour_str = strtok(time, ":");
    char *minute_str = strtok(NULL, ":");
    char *second_str = strtok(NULL, " ");

    if (!validnum(hour_str) || !validnum(minute_str) || !validnum(second_str))
    {
        error("Invalid character format. Must use numbers only.");
        return;
    }

    int hour = atoi(hour_str);
    int minute = atoi(minute_str);
    int second = atoi(second_str);

    if (hour < 0 || hour > 23)
    {
        error("Invalid hour. Use 0-23");
        return;
    }
    if (minute < 0 || minute > 59)
    {
        error("Invalid minute. Use 0-59");
        return;
    }
    if (second < 0 || second > 59)
    {
        error("Invalid second. Use 0-59");
        return;
    }
      // setup process
    pcb *a = pcb_setup(message, 0, 0);
    // setup context for the alarm process
    context c1 = {0x10, 0x10, 0x10, 0x10, 0x10,
                  0, 0, 0, 0, 0, 0, (uint32_t)(a->stack + STACK_SIZE - 1 - sizeof(void *)),
                  (uint32_t)message, 0x8, 0x202};
    // move stack pointer to match for new context space
    a->stack_ptr += sizeof(void *);
    a->stack_ptr -= sizeof(c1);
    // copy context to stack
    memcpy(a->stack_ptr, &c1, sizeof(c1));
    // insert process into pcb list
    pcb_insert(a);
    success("Alarm Set");

    char *curr_time = gettime();
    char *curr_hour_str = strtok(curr_time, ":");
    char *curr_minute_str = strtok(NULL, ":");
    char *curr_second_str = strtok(NULL, " ");

    int curr_hour = atoi(curr_hour_str);
    int curr_minute = atoi(curr_minute_str);
    int curr_second = atoi(curr_second_str);

    while ((curr_hour < hour) || (curr_hour == hour && curr_minute < minute) || (curr_hour == hour && curr_minute == minute && curr_second < second))
    {
        curr_time = gettime();
        curr_hour_str = strtok(curr_time, ":");
        curr_minute_str = strtok(NULL, ":");
        curr_second_str = strtok(NULL, " ");

        curr_hour = atoi(curr_hour_str);
        curr_minute = atoi(curr_minute_str);
        curr_second = atoi(curr_second_str);
       // sys_req(IDLE);

    }
  //  sys_req(WRITE, COM1, time, strlen(time));
    sys_req(WRITE, COM1, message, strlen(message));
}
