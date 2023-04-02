#include <loadr3.h>
#include <processes.h>
#include <string.h>
#include <mpx/pcb.h>
#include <sys_req.h>
#include <mpx/call.h>
#include <stdint.h>
#include <stddef.h>
#include <pcb_user.h>
//save stack_top into esp
void loadr3(){
    // Create 5 system processes with different priorities.

    //process 1
    //setup process
    pcb *p1 = pcb_setup("proc1", 1, 0);
    //setup context for process 1
    context c1 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p1->stack + STACK_SIZE - 1 - sizeof(void *)), 
                  (uint32_t) proc1, 0x8, 0x202
                 };
    //move stack pointer to match for new context space
    p1->stack_ptr += sizeof(void *);
    p1->stack_ptr -= sizeof(c1);
    //copy context to stack
    memcpy(p1->stack_ptr, &c1, sizeof(c1));
    //insert process into pcb list
    pcb_insert(p1);

    //process 2
    //setup process
    pcb *p2 = pcb_setup("proc2", 1, 0);
    //setup context for process 2
    context c2 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p2->stack + STACK_SIZE - 1 - sizeof(void *)), 
                  (uint32_t) proc2, 0x8, 0x202
                 };
    //move stack pointer to match for new context space
    p2->stack_ptr += sizeof(void *);
    p2->stack_ptr -= sizeof(c2);
    //copy context to stack
    memcpy(p2->stack_ptr, &c2, sizeof(c2));
    //insert process into pcb list
    pcb_insert(p2);

    //process 3
    //setup process
    pcb *p3 = pcb_setup("proc3", 1, 0);
    //setup context for process 3
    context c3 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p3->stack + STACK_SIZE - 1 - sizeof(void *)), 
                  (uint32_t) proc3, 0x8, 0x202
                 };
    //move stack pointer to match for new context space
    p3->stack_ptr += sizeof(void *);
    p3->stack_ptr -= sizeof(c3);
    //copy context to stack
    memcpy(p3->stack_ptr, &c3, sizeof(c3));
    //insert process into pcb list
    pcb_insert(p3);

    //process 4
    //setup process
    pcb *p4 = pcb_setup("proc4", 1, 0);
    //setup context for process 4
    context c4 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p4->stack + STACK_SIZE - 1 - sizeof(void *)), 
                  (uint32_t) proc4, 0x8, 0x202
                 };
    //move stack pointer to match for new context space
    p4->stack_ptr += sizeof(void *);
    p4->stack_ptr -= sizeof(c4);
    //copy context to stack
    memcpy(p4->stack_ptr, &c4, sizeof(c4));
    //insert process into pcb list
    pcb_insert(p4);

    //process 5
    //setup process
    pcb *p5 = pcb_setup("proc5", 1, 0);
    //setup context for process 5
    context c5 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p5->stack + STACK_SIZE - 1 - sizeof(void *)), 
                  (uint32_t) proc5, 0x8, 0x202
                 };
    //move stack pointer to match for new context space
    p5->stack_ptr += sizeof(void *);
    p5->stack_ptr -= sizeof(c1);
    //copy context to stack
    memcpy(p5->stack_ptr, &c5, sizeof(c5));
    //insert process into pcb list
    pcb_insert(p5);

    //The five process are removed and freed from after their are dispatched and exited.
}