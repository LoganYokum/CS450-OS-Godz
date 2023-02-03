#include <pcb.h>
#include <memory.h>
#include <string.h>

#define STACK_SIZE 1024

queue *ready;
queue *blocked;
queue *suspended_ready;
queue *suspended_blocked;

pcb *pcb_find(const char *name);

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
    if (p == NULL) {
        return -1;
    }
    sys_free_mem(p->stack_top);

    return sys_free_mem(p);
}

pcb *pcb_setup(const char *name, int type, int priority) {
    pcb *p = pcb_allocate();
    if (p == NULL || strlen(name) > 16 || pcb_find(name) != NULL) {
        return NULL;
    }
    if (type < 0 || type > 2 || priority < 0 || priority > 10) {
        return NULL;
    }

    memcpy(p->name, name, sizeof(name));
    p->type = type;
    p->priority = priority;
    p->state = 1;

    return p;
}

// may shorten later by putting the queues in an array
pcb *pcb_find(const char *name) {
    pcb *cur = ready->head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    cur = blocked->head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    cur = suspended_ready->head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    cur = suspended_blocked->head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }

    return NULL;
}

void queue_insert(queue *q, pcb *p) {
    pcb *cur = q->head;
    if (cur == NULL) {
        q->head = p;
        q->tail = p;
        return;
    }
    while (cur->next != NULL && cur->next->priority <= p->priority) {
        cur = cur->next;
    }
    if (cur->next == NULL) {
        cur->next = p;
        ready->tail = p;
    }else {
        pcb *tmp = cur->next;
        cur->next = p;
        p->next = tmp;
    }
}

void pcb_insert(pcb *p) {
    if (p->state == 1) {  // pcb state is ready, not suspended (0x01)
        queue_insert(ready, p);
    }else if (p->state == 2) { // pcb state is blocked, not suspended (0x02)
        queue_insert(blocked, p);
    }else if (p->state == 17) { // pcb state is ready, suspended (0x11)
        queue_insert(suspended_ready, p);
    }else if (p->state == 18) { // pcb state is blocked, suspended (0x12)
        queue_insert(suspended_blocked, p);
    }
}

int pcb_remove(pcb *p) {

}