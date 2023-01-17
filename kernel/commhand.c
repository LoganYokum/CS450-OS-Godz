#include <sys_req.h>

void commhand() {
    char buffer[100] = {0};
    int len = 100;

    char prompt[] = "> ";

    sys_req(WRITE, COM1, prompt, sizeof(prompt));
    int nread = sys_req(READ, COM1, buffer, len);
    sys_req(WRITE, COM1, buffer, nread);
}