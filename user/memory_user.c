#include <sys_req.h>
#include <stdlib.h>
#include <memory_user.h>
#include <memory.h>
#include <string.h>
#include <heap.h>

void allocate(char *mem_size)
{
    // Do some error checking to verify that the size is valid
    if (mem_size == NULL || !validnum(mem_size)) {
        error("Invalid memory size. Must use numbers only.");
        return;
    }
    void *mem = allocate_memory(atoi(mem_size));
    if (mem == NULL)
    {
        error("Memory allocation failed.");
    }
    else
    {
        char *alloc = sys_alloc_mem(100);
        strcpy(alloc, "Memory allocated at address: ");
        char *address = dtoh((int)mem);
        strcat(alloc, address);
        success(alloc);
        sys_free_mem(alloc);
        sys_free_mem(address);
    }
}
void free(char *address)
{
    if (address == NULL) {
        error("Invalid address.");
        return;
    }
    int mem_addr = htod(address);
    // Do some error checking to verify that the address is valid
    if (free_memory((void *)mem_addr) == 0)
    {
        char *free_msg = sys_alloc_mem(100);
        strcpy(free_msg, "Memory freed at: ");
        strcat(free_msg, address);
        success(free_msg);
        sys_free_mem(free_msg);
    }
    else
    {
        error("Memory free failed.");
    }
}

void show_allocated()
{
    if (alloc_list == NULL)
    {
        sys_req(WRITE, COM1, "No allocated memory\r\n", sizeof("No allocated memory\r\n"));
        return;
    }
    mcb *cur = alloc_list;
    while (cur != NULL) {
        char *address = dtoh((int)cur->start_addr);
        sys_req(WRITE, COM1, "Allocated memory at: ", sizeof("Allocated memory at: "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_req(WRITE, COM1, " with size ", sizeof(" with size "));
        sys_free_mem(address);

        char *size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        sys_free_mem(size);
        cur = cur->next;
    }
}

void show_free()
{
    if (free_list == NULL)
    {
        sys_req(WRITE, COM1, "No free memory\r\n", sizeof("No free memory\r\n"));
        return;
    }
    mcb *cur = free_list;
    while (cur != NULL) {
        char* address = dtoh((int)cur->start_addr);
        sys_req(WRITE, COM1, "Free memory at ", sizeof("Free memory at "));
        sys_req(WRITE, COM1, address, strlen(address));
        sys_free_mem(address);

        sys_req(WRITE, COM1, " with size ", sizeof(" with size "));
        char *size = itoa(cur->size);
        sys_req(WRITE, COM1, size, strlen(size));
        sys_req(WRITE, COM1, "\r\n", 2);
        sys_free_mem(size);
        cur = cur->next;
    }
}