#include <version.h>
#include <stdlib.h>
#include <sys_req.h>
#include <string.h>
#include <time.h>

/**
    Manually update version function upon each module release.
*/
void version(){
    sys_req(WRITE, COM1, "R1: ", sizeof("R1: "));
    time("\n");
}