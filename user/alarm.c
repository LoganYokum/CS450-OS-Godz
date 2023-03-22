#include <alarm.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <mpx/call.h>
#include <mpx/pcb.h>
#include <processes.h>
#include <memory.h>

int hour, minute, second;
int cur_hour, cur_minute, cur_second;
char *time_dup;

alarm_t *alarm_list = NULL;

void alarm_insert(alarm_t *a) {
    if (alarm_list == NULL) {
        alarm_list = a;
        return;
    }
    alarm_t *cur = alarm_list;
    int adjusted_hour = (a->hour < cur_hour) ? a->hour + 24 : a->hour;
    while (cur->next != NULL) {
        int tmp_hour = (cur->hour < cur_hour) ? cur->hour + 24 : cur->hour;
        if (adjusted_hour < tmp_hour || (adjusted_hour == tmp_hour && (a->minute < cur->minute || (a->minute == cur->minute && a->second < cur->second)))) {
            alarm_t *rear = cur->next;
            cur->next = a;
            a->next = rear;
            return;
        }
        cur = cur->next;
    }
}

int alarm_remove() {
    if (alarm_list == NULL) {
        return 0;
    }
    alarm_t *tmp = alarm_list;
    alarm_list = alarm_list->next;
    sys_free_mem(tmp);
    return 1;
}

void alarm_setup(char *time, char *message) {
    if (strlen(time) != 8 || (time[2] != ':' || time[5] != ':')) {
        error("Invalid time format. Use hh:mm:ss");
        return;
    }
    time_dup = (char *) sys_alloc_mem(strlen(time) + 1);
    memcpy(time_dup, time, strlen(time) + 1);

    char *hour_str = strtok(time, ":");
    char *minute_str = strtok(NULL, ":");
    char *second_str = strtok(NULL, " ");

    if (!validnum(hour_str) || !validnum(minute_str) || !validnum(second_str)) {
        error("Invalid character format. Must use numbers only.");
        return;
    }

    hour = atoi(hour_str);
    minute = atoi(minute_str);
    second = atoi(second_str);

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

    pcb *alarm_pcb = pcb_setup(time_dup, 1, 0);
    context alarm_ctx = {
        .ds = 0x10, .es = 0x10, .fs = 0x10, .gs = 0x10, .ss = 0x10,
		.eax = 0, .ebx = 0, .ecx = 0, .edx = 0, .esi = 0, .edi = 0, .ebp = (uint32_t) (alarm_pcb->stack + STACK_SIZE - 1 - sizeof(void *)),
		.eip = (uint32_t) alarm_exec, .cs = 0x8, .eflags = 0x202
    };
    alarm_pcb->stack_ptr += sizeof(void *);
    alarm_pcb->stack_ptr -= sizeof(alarm_ctx);
    memcpy(alarm_pcb->stack_ptr, &alarm_ctx, sizeof(alarm_ctx));
    pcb_insert(alarm_pcb); // insert process into pcb list

    alarm_t *a = (alarm_t *) sys_alloc_mem(sizeof(alarm_t));
    if (a == NULL) {
        error("Failed to allocate memory for alarm");
        return;
    }
    a->hour = hour;
    a->minute = minute;
    a->second = second;
    a->message = message;
    a->next = NULL;
    alarm_insert(a);
    success("Alarm Set");
}

void alarm_exec() {
    char *curr_time = gettime();

    cur_hour = atoi(strtok(curr_time, ":"));
    cur_minute = atoi(strtok(NULL, ":"));
    cur_second = atoi(strtok(NULL, " "));

    // check if alarm at front of list is ready to go off
    if (alarm_list->hour < cur_hour || (alarm_list->hour == cur_hour && (alarm_list->minute < cur_minute || (alarm_list->minute == cur_minute && alarm_list->second < cur_second)))) {
        sys_req(WRITE, COM1, alarm_list->message, strlen(alarm_list->message));
        alarm_remove();
        sys_req(EXIT);
        return;
    }
    sys_req(IDLE);
}
