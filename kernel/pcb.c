#include <mpx/pcb.h>
#include <memory.h>
#include <string.h>

pcb *ready_head;
pcb *blocked_head;
pcb *suspended_ready_head;
pcb *suspended_blocked_head;

pcb *pcb_allocate() {
    pcb *p = (pcb *) sys_alloc_mem(sizeof(pcb));
    if (p == NULL) {
        return NULL;
    }

    p->stack = sys_alloc_mem(STACK_SIZE);
    if (p->stack == NULL) {
        sys_free_mem(p);
        return NULL;
    }
    memset(p->stack, 0, STACK_SIZE);
    p->stack_ptr = p->stack + STACK_SIZE - 1 - sizeof(void *);

    return p;
}

int pcb_free(pcb *p) {
    if (p == NULL) {
        return -1;
    }
    sys_free_mem(p->stack);

    return sys_free_mem(p);
}

int list_free(pcb *head) {
    pcb *cur = head;
    while (cur != NULL) {
        pcb *tmp = cur;
        cur = cur->next;
        pcb_remove(tmp);
        if (pcb_free(tmp) != 0) return -1;
    }
    return 0;
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

pcb *pcb_setup(const char *name, int class, int priority) {
    if (strlen(name) > 16 || pcb_find(name) != NULL || class < 0 || class > 2 || priority < 0 || priority > 9) {
        return NULL;
    }
    pcb *p = pcb_allocate();
    if (p == NULL) {
        return NULL;
    }
    memcpy(p->name, name, strlen(name));
    p->class = class;
    p->priority = priority;
    p->state = 1; //ready, not suspended

    return p;
}

void list_insert_ready(pcb **head, pcb *p) {
    pcb *cur = *head;
    if (cur == NULL) {
        *head = p;
        return;
    }
    if (cur->priority > p->priority) {
        p->next = cur;
        *head = p;
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

void list_insert_blocked(pcb **head, pcb *p) {
    pcb *cur = *head;
    if (cur == NULL) {
        *head = p;
        return;
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = p;
    p->next = NULL; // interesting little change
}

void pcb_insert(pcb *p) {
    if (p->state == 1) {  // pcb state is ready, not suspended (0x01)
        list_insert_ready(&ready_head, p);
    }else if (p->state == 2) { // pcb state is blocked, not suspended (0x02)
        list_insert_blocked(&blocked_head, p);
    }else if (p->state == 17) { // pcb state is ready, suspended (0x11)
        list_insert_ready(&suspended_ready_head, p);
    }else if (p->state == 18) { // pcb state is blocked, suspended (0x12)
        list_insert_blocked(&suspended_blocked_head, p);
    }
}

int list_remove(pcb **head, pcb *p) {
    pcb *cur = *head;
    if (cur == NULL) {
        return 0;
    }
    if (cur == p) {
        *head = cur->next;
        cur->next = NULL;
        return 1;
    }
    while (cur->next != NULL && cur->next != p) {
        cur = cur->next;
    }
    if (cur->next == NULL) {
        return 0;
    }
    pcb *mid = cur->next;
    pcb *rear = mid->next;
    cur->next = rear;
    mid->next = NULL;
    return 1;
}

int pcb_remove(pcb *p) {
    if (p->state == 1) {
        return list_remove(&ready_head, p);
    }
    if (p->state == 2) {
        return list_remove(&blocked_head, p);
    }
    if (p->state == 17) {
        return list_remove(&suspended_ready_head, p);
    }
    if (p->state == 18) {
        return list_remove(&suspended_blocked_head, p);
    }
    return -1;
}