#include <loadr3.h>
#include <processes.h>
#include <mpx/pcb.h>
#include <sys_req.h>
#include <mpx/call.h>
#include <stdint.h>
#include <stddef.h>

void loadr3(){
    // Create 5 system processes with different priorities.
    pcb *process1 = pcb_setup("proc1", 0, 0);
    context *p1 = (context*)(process1->stack_top);
    p1->eax = (uint32_t)process1->stack_ptr;
    p1->ebp = (uint32_t)process1->stack_ptr - (uint32_t)process1->stack_top;
    p1->eip = (uint32_t)proc1;
    p1->eflags = (uint32_t)0x0202;
    p1->cs = (uint32_t)0x08;
    p1->ds = (uint32_t)0x10;
    p1->es = (uint32_t)0x10;
    p1->fs = (uint32_t)0x10; 
    p1->gs = (uint32_t)0x10; 
    p1->ss = (uint32_t)0x10; 
    p1->ebx = (uint32_t)0; 
    p1->ecx = (uint32_t)0;
    p1->edx = (uint32_t)0;
    p1->esi = (uint32_t)0;
    p1->edi = (uint32_t)0;
    pcb_insert(process1);

    pcb *process2 = pcb_setup("proc2", 1, 1);
    context *p2 = (context*)(process2->stack_top);
    p2->eax = (uint32_t)process2->stack_ptr;
    p2->ebp = (uint32_t)process2->stack_ptr - (uint32_t)process2->stack_top;
    p2->eip = (uint32_t)proc2;
    p2->eflags = (uint32_t)0x0202;
    p2->cs = (uint32_t)0x08;
    p2->ds = (uint32_t)0x10;
    p2->es = (uint32_t)0x10;
    p2->fs = (uint32_t)0x10;
    p2->gs = (uint32_t)0x10;
    p2->ss = (uint32_t)0x10;
    p2->ebx = (uint32_t)0;
    p2->ecx = (uint32_t)0;
    p2->edx = (uint32_t)0;
    p2->esi = (uint32_t)0;
    p2->edi = (uint32_t)0;
    pcb_insert(process2);

    pcb *process3 = pcb_setup("proc3", 1, 2);
    context *p3 = (context*)(process3->stack_top);
    p3->eax = (uint32_t)process3->stack_ptr;
    p3->ebp = (uint32_t)process3->stack_ptr - (uint32_t)process3->stack_top;
    p3->eip = (uint32_t)proc3;
    p3->eflags = (uint32_t)0x0202;
    p3->cs = (uint32_t)0x08;
    p3->ds = (uint32_t)0x10;
    p3->es = (uint32_t)0x10;
    p3->fs = (uint32_t)0x10;
    p3->gs = (uint32_t)0x10;
    p3->ss = (uint32_t)0x10;
    p3->ebx = (uint32_t)0;
    p3->ecx = (uint32_t)0;
    p3->edx = (uint32_t)0;
    p3->esi = (uint32_t)0;
    p3->edi = (uint32_t)0;
    pcb_insert(process3);

    pcb *process4 = pcb_setup("proc4", 1, 3);
    context *p4 = (context*)(process4->stack_top);
    p4->eax = (uint32_t)process4->stack_ptr;
    p4->ebp = (uint32_t)process4->stack_ptr - (uint32_t)process4->stack_top;
    p4->eip = (uint32_t)proc4;
    p4->eflags = (uint32_t)0x0202;
    p4->cs = (uint32_t)0x08;
    p4->ds = (uint32_t)0x10;
    p4->es = (uint32_t)0x10;
    p4->fs = (uint32_t)0x10;
    p4->gs = (uint32_t)0x10;
    p4->ss = (uint32_t)0x10;
    p4->ebx = (uint32_t)0;
    p4->ecx = (uint32_t)0;
    p4->edx = (uint32_t)0;
    p4->esi = (uint32_t)0;
    p4->edi = (uint32_t)0;
    pcb_insert(process4);

    pcb *process5 = pcb_setup("proc5", 1, 4);
    context *p5 = (context*)(process5->stack_top);
    p5->eax = (uint32_t)process5->stack_ptr;
    p5->ebp = (uint32_t)process5->stack_ptr - (uint32_t)process5->stack_top;
    p5->eip = (uint32_t)proc5;
    p5->eflags = (uint32_t)0x0202;
    p5->cs = (uint32_t)0x08;
    p5->ds = (uint32_t)0x10;
    p5->es = (uint32_t)0x10;
    p5->fs = (uint32_t)0x10;
    p5->gs = (uint32_t)0x10;
    p5->ss = (uint32_t)0x10;
    p5->ebx = (uint32_t)0;
    p5->ecx = (uint32_t)0;
    p5->edx = (uint32_t)0;
    p5->esi = (uint32_t)0;
    p5->edi = (uint32_t)0;
    pcb_insert(process5);
}