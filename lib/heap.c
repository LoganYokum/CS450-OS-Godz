#include <mpx/vm.h>
#include <heap.h>
#include <stdlib.h>
#include <stddef.h>

mcb_t *free_list;
mcb_t *alloc_list;

void mcb_insert(mcb_t **head, mcb_t *m) {
    mcb_t *cur = *head;
    mcb_t *prev = NULL;
    while (cur != NULL) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        *head = m;
        return;
    }
    prev->next = m;
    m->prev = prev;
}

int mcb_remove(mcb_t **head, mcb_t *m) {
    mcb_t *cur = *head;
    mcb_t *prev = NULL;
    while (cur != NULL && cur != m) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) {
        return 0;
    }
    if (prev == NULL) {
        *head = cur->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
    }else {
        prev->next = cur->next;
        if (cur->next != NULL) {
            cur->next->prev = prev;
        }
    }
    return 1;
}

void initialize_heap(size_t size) {
    void *heap = kmalloc(size + sizeof(mcb_t));
    mcb_t *m = (mcb_t *) heap;
    m->start_addr = heap + sizeof(mcb_t);
    m->size = size;
    m->next = NULL;
    m->prev = NULL;

    mcb_insert(&free_list, m);
    alloc_list = NULL;
}

void *allocate_memory(size_t size) {
    
}