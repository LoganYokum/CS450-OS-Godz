#include <version.h>
#include <sys_req.h>
#include <string.h>
#include <date.h>

/**
    Manually update version function upon each module release.
*/
void version() {
    sys_req(WRITE, COM1, "R5: ", strlen("R5: "));
}
