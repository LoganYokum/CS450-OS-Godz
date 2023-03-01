#include <loadr3.h>
#include <processes.h>
#include <mpx/pcb.h>
#include <sys_req.h>

void loadr3(){
    // Create 5 system processes with different priorities.
    pcb *process1 = pcb_setup("proc1", 1, 0);
    context *p1 = pcb_allocate(sizeof(context));
    p1->esp = &stack_ptr; // This is the stack pointer. top of stack
    p1->ds = 0x10;
    p1->es = 0x10;
    p1->fs = 0x10;
    p1->gs = 0x10;
    p1->ss = 0x10;
    p1->edi = 0;
    p1->esi = 0;
    p1->ebp = &stack_ptr - &stack_top; //bottom of stack
    p1->ebx = 0;
    p1->edx = 0;
    p1->ecx = 0;
    p1->eax = 0;
    p1->eip = &proc1;
    p1->cs = 0x08;
    p1->eflags = 0x0202;
    sys_call(p1);
    pcb_insert(process1);
    pcb *process2 = pcb_setup("proc2", 1, 1);
    context *p2 = sys_alloc_mem(sizeof(context));
    p2->esp = 0;
    p2->ds = 0;
    p2->es = 0;
    p2->fs = 0;
    p2->gs = 0;
    p2->ss = 0;
    p2->edi = 0;
    p2->esi = 0;
    p2->ebp = 0;
    p2->ebx = 0;
    p2->edx = 0;
    p2->ecx = 0;
    p2->eax = 0;
    p2->eip = 0;
    p2->cs = 0;
    p2->eflags = 0;
    sys_call(p2);
    pcb_insert(process2);
    pcb *process3 = pcb_setup("proc3", 1, 2);
    context *p3 = sys_alloc_mem(sizeof(context));
    p3->esp = 0;
    p3->ds = 0;
    p3->es = 0;
    p3->fs = 0;
    p3->gs = 0;
    p3->ss = 0;
    p3->edi = 0;
    p3->esi = 0;
    p3->ebp = 0;
    p3->ebx = 0;
    p3->edx = 0;
    p3->ecx = 0;
    p3->eax = 0;
    p3->eip = 0;
    p3->cs = 0;
    p3->eflags = 0;
    sys_call(p3);
    pcb_insert(process3);
    pcb *process4 = pcb_setup("proc4", 1, 3);
    pcb_insert(process4);
    pcb *process5 = pcb_setup("proc5", 1, 4);
    pcb_insert(process5);
    // Call the 5 processes. From the system.
    proc1();
    proc2();
    proc3();
    proc4();
    proc5();
    // Free the 5 processes.
    pcb_free(process1);
    pcb_free(process2);
    pcb_free(process3);
    pcb_free(process4);
    pcb_free(process5);
}