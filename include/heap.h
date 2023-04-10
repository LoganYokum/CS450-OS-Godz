typedef struct mcb {
    void *start_addr;
    size_t size;
    struct mcb  *prev;
    struct mcb  *next;
} mcb;

extern mcb  *free_list;
extern mcb  *alloc_list;

/**
 * Allocates all memory available to heap manager
 * @param size Size of memory to allocate
 */
void initialize_heap(size_t size);

/**
 * Allocates memory from the heap
 * @param size Size of memory to allocate
 */
void *allocate_memory(size_t size);

/**
 * Frees memory from the heap
 * @param addr Address of memory to free
 * @return 0 on success, non-zero on failure
 */
int free_memory(void *addr);