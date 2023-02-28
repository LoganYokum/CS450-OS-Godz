#include <loadr3.h>
#include <processes.h>
#include <mpx/pcb.h>
void loadr3(){
    pcb *process1 = pcb_setup("proc1", 1, 0);
    pcb_insert(process1);
    pcb *process2 = pcb_setup("proc2", 1, 1);
    pcb_insert(process2);
    pcb *process3 = pcb_setup("proc3", 1, 2);
    pcb_insert(process3);
    pcb *process4 = pcb_setup("proc4", 1, 3);
    pcb_insert(process4);
    pcb *process5 = pcb_setup("proc5", 1, 4);
    pcb_insert(process5);
    proc1();
    proc2();
    proc3();
    proc4();
    proc5();
    pcb_free(process1);
    pcb_free(process2);
    pcb_free(process3);
    pcb_free(process4);
    pcb_free(process5);
}