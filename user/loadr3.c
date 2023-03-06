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
    pcb *p1 = pcb_setup("proc1", 1, 0);
    // context *c1 = (context *) p1->stack_ptr;
    // memset(c1, 0, sizeof(context));
    // c1->fs = 0x10;
    // c1->gs = 0x10;
    // c1->ds = 0x10;
    // c1->es = 0x10;
    // c1->ss = 0x10;
    // c1->cs = 0x8;
    // c1->ebp = (uint32_t)(p1->stack + STACK_SIZE - 1);
    // c1->eip = (uint32_t) proc1;
    // c1->eflags = 0x202;
    // c1->eax = 0;
    // c1->ebx = 0;
    // c1->ecx = 0;
    // c1->edx = 0;
    // c1->esi = 0;
    // c1->edi = 0;
    // pcb_insert(p1);
    context c1 = {0x10, 0x10, 0x10, 0x10, 0x10, 
                  0, 0, 0, 0, 0, 0, (uint32_t) (p1->stack + STACK_SIZE - 1), 
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