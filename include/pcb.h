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
