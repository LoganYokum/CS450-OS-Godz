/**
 * Different states of process (col 1 is dispatch state, col 2 is execution state)
 * 0x00: running, not suspended
 * 0x01: ready, not suspended
 * 0x02: blocked, not suspended
 * 
 * 0x10: running, suspended
 * 0x11: ready, suspended
 * 0x12: blocked, suspended
 */

typedef struct pcb {
    char name[16];
    int type;
    int priority;
    int state;
    void *stack_top;
    void *stack_ptr;
    struct pcb *next;
} pcb;

typedef struct queue {
    pcb *head;
    pcb *tail;
} queue;

/**
 * Allocates a new instance of pcb struct
 * @return pointer to the new pcb struct
 */
pcb *pcb_allocate();

/**
 * Frees the memory associated with a pcb struct
 * @param p the pcb struct to free
 * @return 0 on success, non-zero on failure
 */
int pcb_free(pcb *p);

/**
 * Finds an element in a specified queue by name
 * @param q the queue to search
 * @param name the name of the pcb to find
 * @return pointer to the pcb struct if found, NULL otherwise
 */
pcb *queue_find(queue *q, const char *name);

/**
 * Finds a pcb struct by name
 * @param name the name of the pcb to find
 * @return pointer to the pcb struct if found, NULL otherwise 
 */
pcb *pcb_find(const char *name);

/**
 * Initializes a pcb struct with the given parameters
 * @param name the name of the process
 * @param type the type of the process
 * @param priority the priority of the process
 * @return pointer to the initialized pcb struct
 */
pcb *pcb_setup(const char *name, int type, int priority);


/**
 * Inserts a pcb struct into a specified queue
 * @param q the struct to insert into
 * @param p the pcb to insert
 */
void queue_insert(queue *q, pcb *p);

/**
 * Inserts a pcb struct into the appropriate queue
 * @param p the pcb to insert
 */
void pcb_insert(pcb *p);

/**
 * Removes a pcb struct from a specified queue
 * @param q the queue to remove from
 * @param p the pcb to remove
 * @return 1 on success, 0 on failure 
 */
int queue_remove(queue *q, pcb *p);

/**
 * Removes a pcb struct from the appropriate queue
 * @param p the pcb to remove
 * @return 1 on success, 0 on failure
 */
int pcb_remove(pcb *p);
