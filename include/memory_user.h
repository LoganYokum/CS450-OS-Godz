/**
 * @brief Allocate memory on the heap
 * 
 * @param mem_size number of bytes to allocate
 */
void allocate(char* mem_size);
/**
 * @brief Free memory on the heap
 * 
 */
void free(char* address);
/**
 * @brief Show the memory block start address(s) and the size of each block for all allocated blocks
 * 
 * @param address start address of the memory block to free
 */
void show_allocated();
/**
 * @brief 
 * Show the memory block start address(s) and the size of each block for all free blocks
 */
void show_free();