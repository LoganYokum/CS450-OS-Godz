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
    void *heap = kmalloc(size + sizeof(mcb_t), 0, NULL);
    mcb_t *m = (mcb_t *) heap;
    m->start_addr = heap + sizeof(mcb_t);
    m->size = size;
    m->next = NULL;
    m->prev = NULL;

    mcb_insert(&free_list, m);
    alloc_list = NULL;
}

void *allocate_memory(size_t size) {
    // free block is too small for mcb and requested size
    if (free_list->size < size + sizeof(mcb_t)) {
        return NULL;
    }
    // split the current free block into two blocks (one allocated, one remaining free)
    mcb_t *alloc_mcb = free_list;
    mcb_t *free_mcb = (mcb_t *) (free_list->start_addr + size);
    free_mcb->start_addr = free_mcb + sizeof(mcb_t);
    free_mcb->size = free_list->size - size - sizeof(mcb_t);

    alloc_mcb->size = size;
    alloc_mcb->prev = NULL;
    alloc_mcb->next = NULL;

    // remove the free block from the free list and add the allocated block to the allocated list
    mcb_remove(&free_list, free_list);
    mcb_insert(&alloc_list, alloc_mcb);

    // add the remaining free block back to the free list
    free_mcb->next = free_list;
    free_mcb->prev = NULL;
    free_list = free_mcb;

    return alloc_mcb->start_addr;
}

int free_memory(void *addr) {

}