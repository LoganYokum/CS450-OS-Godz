#include <mpx/vm.h>
#include <heap.h>
#include <stdlib.h>
#include <stddef.h>

mcb_t *free_list;
mcb_t *alloc_list;

void mcb_insert(mcb_t **head, mcb_t *m) {
    mcb_t *prev = NULL;
    mcb_t *cur = *head;

    // insert the mcb in order of start address (descending) so that the large initial block is always first
    while (cur != NULL && cur->start_addr > m->start_addr) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        *head = m;
        m->next = cur;
        if (cur != NULL) {
            cur->prev = m;
        }
    }else {
        prev->next = m;
        m->prev = prev;
        m->next = cur;
        if (cur != NULL) {
            cur->prev = m;
        }
    }
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
    m->next = NULL;
    m->prev = NULL;
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
    // check if there is a free block that is large enough
    mcb_t *cur = free_list;
    while (cur != NULL) {
        if (cur->size >= size + sizeof(mcb_t)) {
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL) {
        return NULL;
    }
    // split the current free block into two blocks (one allocated, one remaining free)
    mcb_t *alloc_mcb = cur;
    mcb_t *free_mcb = (mcb_t *) ((char *)cur->start_addr + size);
    free_mcb->start_addr = free_mcb + 1; // start addr is greater by the size of the mcb
    free_mcb->size = cur->size - size - sizeof(mcb_t);

    alloc_mcb->start_addr = alloc_mcb + 1; // start addr is greater by the size of the mcb
    alloc_mcb->size = size;

    mcb_remove(&free_list, alloc_mcb); // remove allocated block from free list
    mcb_insert(&alloc_list, alloc_mcb); // add allocated block to the allocated list
    mcb_insert(&free_list, free_mcb); // add remaining free block to the free list

    return alloc_mcb->start_addr;
}

int free_memory(void *addr) {
    // find the mcb for the given address
    mcb_t *m = (mcb_t *) ((char *)addr - sizeof(mcb_t));
    if (!mcb_remove(&alloc_list, m)) {
        return 1;
    }
    // add the mcb to the free list
    mcb_insert(&free_list, m);

    // merge any adjacent free blocks
    mcb_t *cur = free_list;
    while (cur != NULL) {
         // check if the next block is adjacent to the current block
        if (cur->next != NULL && ((char *)cur->start_addr == (char *)cur->next->start_addr + cur->next->size + sizeof(mcb_t))) {
            cur->start_addr = cur->next->start_addr;
            cur->size += cur->next->size + sizeof(mcb_t);
            mcb_remove(&free_list, cur->next);
            continue;
        }
        cur = cur->next;
    }
    return 0;
}