typedef struct mcb_t {
    void *start_addr;
    void *size;
    struct mcb *prev;
    struct mcb *next;
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
 */
void free_memory(void *addr);