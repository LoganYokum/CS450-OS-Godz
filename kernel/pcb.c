#include <pcb.h>
#include <memory.h>
#include <string.h>

#define STACK_SIZE 1024

pcb *pcb_allocate() {
    pcb *p = (pcb *) sys_alloc_mem(sizeof(pcb));
    if (p == NULL) {
        return NULL;
    }

    p->stack_top = sys_alloc_mem(STACK_SIZE);
    if (p->stack_top == NULL) {
        sys_free_mem(p);
        return NULL;
    }
    memset(p->stack_top, 0, STACK_SIZE);
    p->stack_ptr = p->stack_top + STACK_SIZE - 1;

    return p;
}

int pcb_free(pcb *p) {
    return sys_free_mem(p);
}

pcb *pcb_setup(const char *name, int type, int priority) {
    pcb *p = pcb_allocate();
    if (p == NULL) {
        return NULL;
    }

    memcpy(p->name, name, sizeof(name));
    p->type = type;
    p->priority = priority;
    p->state = 1;

    return p;
}

