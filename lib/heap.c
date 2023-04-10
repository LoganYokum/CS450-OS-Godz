#include <mpx/vm.h>
#include <heap.h>
#include <stdlib.h>
#include <stddef.h>

mcb *free_list;
mcb *alloc_list;

void mcb_insert(mcb **head, mcb *m) {
    mcb *prev = NULL;
    mcb *cur = *head;

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

int mcb_remove(mcb **head, mcb *m) {
    mcb *cur = *head;
    mcb *prev = NULL;
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
    void *heap = kmalloc(size + sizeof(mcb), 0, NULL);
    mcb *m = (mcb *) heap;
    m->start_addr = heap + sizeof(mcb);
    m->size = size;
    m->next = NULL;
    m->prev = NULL;

    mcb_insert(&free_list, m);
    alloc_list = NULL;
}

void *allocate_memory(size_t size) {
    // check if the size is valid
    if (size < 1 || size > 50000) {
        return NULL;
    }
    // check if there is a free block that is large enough
    mcb *cur = free_list;
    while (cur != NULL) {
        if (cur->size >= size + sizeof(mcb)) {
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL) return NULL; // no free block large enough to allocate
    
    // split the current free block into two blocks (one allocated, one remaining free)
    mcb *alloc_mcb = cur;
    mcb *free_mcb = (mcb *) ((char *)cur->start_addr + size);
    free_mcb->start_addr = free_mcb + 1; // start addr is greater by the size of the mcb
    free_mcb->size = cur->size - size - sizeof(mcb);

    alloc_mcb->start_addr = alloc_mcb + 1; // start addr is greater by the size of the mcb
    alloc_mcb->size = size;

    mcb_remove(&free_list, alloc_mcb); // remove allocated block from free list
    mcb_insert(&alloc_list, alloc_mcb); // add allocated block to the allocated list
    mcb_insert(&free_list, free_mcb); // add remaining free block to the free list

    return alloc_mcb->start_addr;
}

int free_memory(void *addr) {
    // find the mcb for the given address
    mcb *m = (mcb *) ((char *)addr - sizeof(mcb));
    if (!mcb_remove(&alloc_list, m)) {
        return 1;
    }
    // add the mcb to the free list
    mcb_insert(&free_list, m);

    // merge any adjacent free blocks
    mcb *cur = free_list;
    while (cur != NULL) {
         // check if the next block is adjacent to the current block
        if (cur->next != NULL && ((char *)cur->start_addr == (char *)cur->next->start_addr + cur->next->size + sizeof(mcb))) {
            cur->next->size += cur->size + sizeof(mcb);
            cur = cur->next;
            mcb_remove(&free_list, cur->prev);
        }else {
            cur = cur->next;
        }
    }
    return 0;
}