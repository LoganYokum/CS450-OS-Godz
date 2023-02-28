#include <processes.h>
#include <mpx/pcb.h>
#include <stdlib.h>
#include <sys_req.h>
#include <yield.h>

void proc1(void){
    pcb *p = pcb_setup("proc1", 1, 1);
    pcb_insert(p);
    for(int i = 0; i < 1; i++){
        sys_req(WRITE, COM1, "proc1 is running! ", sizeof("proc1 is running! "));
        sys_req(WRITE,COM1,itoa(i),sizeof(itoa(i)));
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        yield();
    }
    sys_req(EXIT); //exit the process when done
}
void proc2(void){
    pcb *p = pcb_setup("proc2", 1, 2);
    pcb_insert(p);
    for(int i = 0; i < 2;i++){
        sys_req(WRITE, COM1, "proc2 is running! ", sizeof("proc2 is running! "));
        sys_req(WRITE,COM1,itoa(i),sizeof(itoa(i)));
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        yield();
    }
    sys_req(EXIT);
}
void proc3(void){
    pcb *p = pcb_setup("proc3", 1, 3);
    pcb_insert(p);
    for(int i = 0; i < 3; i++){
        sys_req(WRITE, COM1, "proc3 is running! ", sizeof("proc3 is running! "));
        sys_req(WRITE,COM1,itoa(i),sizeof(itoa(i)));
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        yield();
    }
    sys_req(EXIT);
}
void proc4(void){
    pcb *p = pcb_setup("proc4", 1, 4);
    pcb_insert(p);
    for(int i = 0; i < 4; i++){
        sys_req(WRITE, COM1, "proc4 is running! ", sizeof("proc4 is running! "));
        sys_req(WRITE,COM1,itoa(i),sizeof(itoa(i)));
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        yield();
    }
    sys_req(EXIT);
}
void proc5(void){
    pcb *p = pcb_setup("proc5", 1, 5);
    pcb_insert(p);
    for(int i = 0; i < 5; i++){
        sys_req(WRITE, COM1, "proc5 is running! ", sizeof("proc5 is running! "));
        sys_req(WRITE,COM1,itoa(i),sizeof(itoa(i)));
        sys_req(WRITE, COM1, "\n", sizeof("\n"));
        yield();
    }
    sys_req(EXIT);
}
