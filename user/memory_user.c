#include <sys_req.h>
#include <stdlib.h>
#include <memory_user.h>
#include <string.h>
#include <heap.h>

void allocate(char* mem_size){
    //Do some error checking to verify that the size is valid
    void *mem = allocate_memory(atoi(mem_size));
    if(mem == NULL){
        error("Memory allocation failed.");
    }
    else{
        sys_req(WRITE, COM1, "Memory allocated at address: ", sizeof("Memory allocated at address: "));
        char* address = dtoh((int) mem);
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);   
    }
}
void free(char* address){
    int mem_addr = htod(address);
    //Do some error checking to verify that the address is valid
    if(free_memory((void *) mem_addr)==0){
        sys_req(WRITE, COM1, "Memory freed at: ", sizeof("Memory freed at: "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);
    }
    else{
        error("Memory free failed.");
    }
}

void show_allocated() {
    if (alloc_list == NULL) {
        sys_req(WRITE, COM1, "No allocated memory\r\n", sizeof("No allocated memory\r\n"));
        return;
    }
    mcb_t *cur = alloc_list;
    while (cur != NULL) {
        char* address = dtoh((int)cur->start_addr);
        sys_req(WRITE, COM1, "Allocated memory at: ", sizeof("Allocated memory at: "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, " with size ", sizeof(" with size "));
        char* size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        cur = cur->next;
    }
}

void show_free() {
    if (free_list == NULL) {
        sys_req(WRITE, COM1, "No free memory\r\n", sizeof("No free memory\r\n"));
        return;
    }
    mcb_t *cur = free_list;
    while (cur != NULL) {
         char* address = dtoh((int)cur->start_addr);
        sys_req(WRITE, COM1, "Free memory at ", sizeof("Free memory at "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, " with size ", sizeof(" with size "));
        char* size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        cur = cur->next;
    }
}