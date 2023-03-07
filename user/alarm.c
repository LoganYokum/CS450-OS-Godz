#include <alarm.h>
#include <sys_req.h>
#include <string.h>

void alarm(char* time, char* message){
    sys_req(WRITE,COM1,time,strlen(time));
    sys_req(WRITE,COM1,message,strlen(message));
}