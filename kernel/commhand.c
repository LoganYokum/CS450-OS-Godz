#include <sys_req.h>
#include <string.h>

void commhand() {
    char buffer[100] = {0};
    int len = 100;

    char prompt[] = "> ";

    sys_req(WRITE, COM1, prompt, sizeof(prompt));
    int nread = sys_req(READ, COM1, buffer, len);
    sys_req(WRITE, COM1, buffer, nread);

    // while (1) {
    //     sys_req(WRITE, COM1, prompt, sizeof(prompt));
    //     int nread = sys_req(READ, COM1, buffer, len);
    //     sys_req(WRITE, COM1, buffer, nread);

    //     if (strcmp(buffer, "shutdown") == 0) {
    //         shutdown();
    //     }
    // }
}