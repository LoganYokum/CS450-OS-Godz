#include <alarm.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <mpx/call.h>
#include <mpx/pcb.h>
#include <pcb_user.h>
#include <processes.h>

//TODO: add functionality to allow full string as a message, allow for multiple alarms (queue), check uniqueness in alarms and messages (no duplicates)

int hour, minute, second;

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

    hour = atoi(hour_str);
    minute = atoi(minute_str);
    second = atoi(second_str);

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
    pcb *a = pcb_setup("Alarm", 0, 0);
    // setup context for the alarm process
    context alarm_ctx = {.ds = 0x10, .es = 0x10, .fs = 0x10, .gs = 0x10, .ss = 0x10,
		.eax = 0, .ebx = 0, .ecx = 0, .edx = 0, .esi = 0, .edi = 0, .ebp = (uint32_t) (a->stack + STACK_SIZE - 1 - sizeof(void *)),
		.eip = (uint32_t) alarmmsg, .cs = 0x8, .eflags = 0x202};
    // move stack pointer to match for new context space
    a->stack_ptr += sizeof(void *);
    a->stack_ptr -= sizeof(alarm_ctx);
    // copy context to stack
    memcpy(a->stack_ptr, &alarm_ctx, sizeof(alarm_ctx));
    // insert process into pcb list
    pcb_insert(a);
    success("Alarm Set");
    sys_req(WRITE, COM1, message, strlen(message)); //ASSIGN EACH ALARM A MESSAGE TO BE PRINTED
}

void alarmmsg()
{
    char *curr_time = gettime();
    char *curr_hour_str = strtok(curr_time, ":");
    char *curr_minute_str = strtok(NULL, ":");
    char *curr_second_str = strtok(NULL, " ");

    int curr_hour = atoi(curr_hour_str);
    int curr_minute = atoi(curr_minute_str);
    int curr_second = atoi(curr_second_str);
    while(1){
        curr_time = gettime();
        curr_hour_str = strtok(curr_time, ":");
        curr_minute_str = strtok(NULL, ":");
        curr_second_str = strtok(NULL, " ");

        curr_hour = atoi(curr_hour_str);
        curr_minute = atoi(curr_minute_str);
        curr_second = atoi(curr_second_str);
        if(curr_hour >= hour && curr_minute >= minute && curr_second >= second){
            sys_req(WRITE, COM1, "Alarm Went off \n", strlen("Alarm Went off \n")); //PUT MESSAGE HERE FOR ALARM
            sys_req(EXIT);
        }
        else{
            sys_req(IDLE);
        }
    }
}
