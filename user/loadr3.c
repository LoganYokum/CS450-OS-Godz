#include <loadr3.h>
#include <processes.h>
#include <string.h>
#include <mpx/pcb.h>
#include <sys_req.h>
#include <mpx/call.h>
#include <stdint.h>
#include <stddef.h>

void loadr3(){
    // Create 5 system processes with different priorities.
    pcb *p1 = pcb_setup("proc1", 1, 0);
    context c1 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p1->stack + STACK_SIZE - 1- sizeof(void *)), 
                  (uint32_t) proc1, 0x8, 0x202
                 };
    p1->stack_ptr += 4;
    p1->stack_ptr -= sizeof(c1);
    memcpy(p1->stack_ptr, &c1, sizeof(c1));
    pcb_insert(p1);

    // pcb *p2 = pcb_setup("proc2", 1, 1);
    
    // pcb_insert(p2);

    // pcb *p3 = pcb_setup("proc3", 1, 2);
    
    // pcb_insert(p3);

    // pcb *p4 = pcb_setup("proc4", 1, 3);
    
    // pcb_insert(p4);

    // pcb *p5 = pcb_setup("proc5", 1, 4);
    
    // pcb_insert(p5);
}