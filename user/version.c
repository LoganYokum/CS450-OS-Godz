#include <version.h>
#include <stdlib.h>
#include <sys_req.h>
#include <string.h>
#include <date.h>

/**
    Manually update version function upon each module release.
*/
void version(char *comp_date) {
    sys_req(WRITE, COM1, "R1: ", sizeof("R1: "));
    sys_req(WRITE, COM1, comp_date, strlen(comp_date));
    sys_req(WRITE, COM1, "\r\n", 2);
}