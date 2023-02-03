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
 * Initializes a pcb struct with the given parameters
 * @param name the name of the process
 * @param type the type of the process
 * @param priority the priority of the process
 * @return pointer to the initialized pcb struct
 */
pcb *pcb_setup(const char *name, int type, int priority);
