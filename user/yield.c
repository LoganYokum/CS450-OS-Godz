#include <yield.h>
#include <sys_req.h>
void yield(){
    sys_req(IDLE);
}