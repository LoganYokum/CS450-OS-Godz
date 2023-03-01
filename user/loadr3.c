#include <loadr3.h>
#include <processes.h>
#include <mpx/pcb.h>
#include <sys_req.h>
#include <call.h>

void loadr3(){
    // Create 5 system processes with different priorities.
    pcb *process1 = pcb_setup("proc1", 0, 0);
    context *p1 = pcb_allocate(sizeof(context)); //allocate on stack
    p1 = {&stack_ptr, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0, 0, &stack_ptr - &stack_top, 0, 0, 0, 0, 0, 0, 0, &proc1, 0x08, 0x0202 }
    sys_call(p1);
    pcb_insert(process1);
    pcb *process2 = pcb_setup("proc2", 1, 1);
    context *p2 = pcb_allocate(sizeof(context)); //allocate on stack
    p2 = {&stack_ptr, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0, 0, &stack_ptr - &stack_top, 0, 0, 0, 0, 0, 0, 0, &proc2, 0x08, 0x0202 }
    sys_call(p2);
    pcb_insert(process2);
    pcb *process3 = pcb_setup("proc3", 1, 2);
    context *p3 = pcb_allocate(sizeof(context)); //allocate on stack
    p3 = {&stack_ptr, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0, 0, &stack_ptr - &stack_top, 0, 0, 0, 0, 0, 0, 0, &proc3, 0x08, 0x0202 }
    sys_call(p3);
    pcb_insert(process3);
    pcb *process4 = pcb_setup("proc4", 1, 3);
    context *p4 = pcb_allocate(sizeof(context)); //allocate on stack
    p4 = {&stack_ptr, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0, 0, &stack_ptr - &stack_top, 0, 0, 0, 0, 0, 0, 0, &proc4, 0x08, 0x0202 }
    sys_call(p4);
    pcb_insert(process4);
    pcb *process5 = pcb_setup("proc5", 1, 4);
    context *p5 = pcb_allocate(sizeof(context)); //allocate on stack
    p5 = {&stack_ptr, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0, 0, 0, 0, 0, &stack_ptr - &stack_top, 0, 0, 0, 0, 0, 0, 0, &proc5, 0x08, 0x0202 }
    sys_call(p5);
    pcb_insert(process5);
    // Call the 5 processes. From the system.
    proc1();
    proc2();
    proc3();
    proc4();
    proc5();
    // Remove the 5 processes.
    pcb_remove(process1);
    pcb_remove(process2);
    pcb_remove(process3);
    pcb_remove(process4);
    pcb_remove(process5);
    // Free the 5 processes.
    pcb_free(process1);
    pcb_free(process2);
    pcb_free(process3);
    pcb_free(process4);
    pcb_free(process5);
}