#include <sys_req.h>
#include <stdlib.h>
#include <memory_user.h>
#include <string.h>
#include <heap.h>

void allocate(char* mem_size){
    void *mem = allocate_memory(atoi(mem_size));
    if(mem == NULL){
        error("Memory allocation failed.");
    }
    else{
        sys_req(WRITE, COM1, "Memory allocated at address: ", sizeof("Memory allocated at address: "));
        char* address = strcat("0x", itoa(dtoh((int) mem)));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);
        
    }
}
void free(char* address){
    int mem_addr = atoi(strsub(address, 2, strlen(address))); // may need to change this as strsub dynamically allocates memory
    if(free_memory((void *) mem_addr) == 0){
        sys_req(WRITE, COM1, "Memory freed at: ", sizeof("Memory freed at: "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);
    }
    else{
        error("Memory free failed.");
    }
}

void show_allocated() {
    mcb_t *cur = alloc_list;
    while (cur->next != NULL) {
        char* address = itoa((int)cur->start_addr);
        sys_req(WRITE, COM1, "Allocated memory at 0x", 22);
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, " with size ", 11);
        char* size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        cur = cur->next;
    }
}

void show_free() {
    mcb_t *cur = free_list;
    while (cur->next != NULL) {
        char* address = itoa((int)cur->start_addr);
        sys_req(WRITE, COM1, "Free memory at 0x", 17);
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, " with size ", 11);
        char* size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        cur = cur->next;
    }
}