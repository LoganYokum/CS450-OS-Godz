#include <pcb.h>
#include <memory.h>
#include <string.h>

#define STACK_SIZE 1024

pcb *ready_head;
pcb *blocked_head;
pcb *suspended_ready_head;
pcb *suspended_blocked_head;

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

pcb *list_find(pcb *head, const char *name) {
    pcb *cur = head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

pcb *pcb_find(const char *name) {
    pcb *p = list_find(ready_head, name);
    if (p != NULL) {
        return p;
    }

    p = list_find(blocked_head, name);
    if (p != NULL) {
        return p;
    }

    p = list_find(suspended_ready_head, name);
    if (p != NULL) {
        return p;
    }

    p = list_find(suspended_blocked_head, name);
    if (p != NULL) {
        return p;
    }

    return NULL;
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
    p->state = 1; //ready, not suspended

    return p;
}

void list_insert(pcb *head, pcb *p) {
    pcb *cur = head;
    if (cur == NULL) {
        head = p;
        return;
    }
    while (cur->next != NULL && cur->next->priority <= p->priority) {
        cur = cur->next;
    }
    if (cur->next == NULL) {
        cur->next = p;
    }else {
        pcb *tmp = cur->next;
        cur->next = p;
        p->next = tmp;
    }
}

void pcb_insert(pcb *p) {
    if (p->state == 1) {  // pcb state is ready, not suspended (0x01)
        list_insert(ready_head, p);
    }else if (p->state == 2) { // pcb state is blocked, not suspended (0x02)
        list_insert(blocked_head, p);
    }else if (p->state == 17) { // pcb state is ready, suspended (0x11)
        list_insert(suspended_ready_head, p);
    }else if (p->state == 18) { // pcb state is blocked, suspended (0x12)
        list_insert(suspended_blocked_head, p);
    }
}

int list_remove(pcb *head, pcb *p) {
    pcb *cur = head;
    if (cur == NULL) {
        return 0;
    }
    while (cur->next != NULL && cur->next != p) {
        cur = cur->next;
    }
    if (cur->next == NULL) {
        return 0;
    }else {
        pcb *mid = cur->next;
        pcb *rear = mid->next;
        cur->next = rear;
        mid->next = NULL;
    }
    return 1;
}

int pcb_remove(pcb *p) {
    if (p->state == 1) {
        return list_remove(ready_head, p);
    }
    if (p->state == 2) {
        return list_remove(blocked_head, p);
    }
    if (p->state == 17) {
        return list_remove(suspended_ready_head, p);
    }
    if (p->state == 18) {
        return list_remove(suspended_blocked_head, p);
    }
}