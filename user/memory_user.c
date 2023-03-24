#include <sys_req.h>
#include <stdlib.h>
#include <memory_user.h>
#include <string.h>
#include <memory.h>
void allocate(char* mem_size){
    int int_size = atoi(mem_size);
    char* mem = (char*)sys_alloc_mem(int_size); //CHANGE TO allocate_memory()
    if(mem == NULL){
        error("Memory allocation failed.");
    }
    else{
        sys_req(WRITE, COM1, "Memory allocated at address: 0x", 28);
        char* address = itoa(dtoh(atoi(mem)));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);
    }
}
void free(char* address){
    int return_val = sys_free_mem(address); //CHANGE TO free_memory()
    if(return_val == 0){
        sys_req(WRITE, COM1, "Memory freed at: 0x", 19);
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, "\r\n", 2);
    }
    else{
        error("Memory free failed.");
    }
}

void show_allocated(){
    // while(allocated_list->next != NULL){
        // char* address = itoa(allocated_list->address);
        // sys_req(WRITE, COM1, "Allocated memory at 0x", 22);
        // sys_req(WRITE, COM1, address, strlen(address));
        // sys_req(WRITE, COM1, " with size ", 11);
        // char* size = itoa(allocated_list->size);
        // sys_req(WRITE, COM1, size, strlen(size));
        // sys_req(WRITE, COM1, "\r\n", 2);
        // allocated_list = allocated_list->next;
    // }
}

void show_free(){
    // while(free_list->next != NULL){
        // char* address = itoa(free_list->address);
        // sys_req(WRITE, COM1, "Free memory at 0x", 17);
        // sys_req(WRITE, COM1, address, strlen(address));
        // sys_req(WRITE, COM1, " with size ", 11);
        // char* size = itoa(free_list->size);
        // sys_req(WRITE, COM1, size, strlen(size));
        // sys_req(WRITE, COM1, "\r\n", 2);
        // free_list = free_list->next;
    // }
}