typedef struct mcb_t {
    void *start_addr;
    size_t size;
    struct mcb_t *prev;
    struct mcb_t *next;
} mcb_t;

extern mcb_t *free_list;
extern mcb_t *alloc_list;

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