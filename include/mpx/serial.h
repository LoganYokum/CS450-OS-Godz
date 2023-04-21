#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#include <stddef.h>
#include <mpx/device.h>
#include <mpx/pcb.h>
#include <sys_req.h>

typedef struct iocb iocb;
typedef struct dcb dcb;

struct dcb {
    int busy_flag;
    int open_flag;
    int event_flag;
    op_code cur_op;
    char *buffer;
    size_t buf_len;
    int buf_start;
    int buf_end;
    iocb *iocb_queue;
};

struct iocb {
    op_code cur_op;
    char *buffer;
    size_t buf_idx;
    size_t buf_len;
    pcb *process;
    iocb* next;
};

extern dcb dcb_table[4];

/**
 @file mpx/serial.h
 @brief Kernel functions and constants for handling serial I/O
*/

/**
 Initializes devices for user input and output
 @param device A serial port to initialize (COM1, COM2, COM3, or COM4)
 @return 0 on success, non-zero on failure
*/
int serial_init(device dev);

/**
 Writes a buffer to a serial port
 @param device The serial port to output to
 @param buffer A pointer to an array of characters to output
 @param len The number of bytes to write
 @return The number of bytes written
*/
int serial_out(device dev, const char *buffer, size_t len);

/**
 Reads a string from a serial port
 @param device The serial port to read data from
 @param buffer A buffer to write data into as it is read from the serial port
 @param count The maximum number of bytes to read
 @return The number of bytes read on success, a negative number on failure
*/   		   

int serial_poll(device dev, char *buffer, size_t len);

/**
 * Second level interrupt handler for serial input
 * @param dcb struct for the device that triggered the interrupt
 */
void serial_input_interrupt(struct dcb *dcb);

/**
 * Second level interrupt handler for serial output
 * @param dcb struct for the device that triggered the interrupt
 */
void serial_output_interrupt(struct dcb *dcb);

/**
 * First level interrupt handler for serial interrupts
 */
void serial_interrupt();

/**
 * Opens a serial port
 * @param dev The serial port to open
 * @param speed The baud rate to use
 * @return 0 on success, non-zero on failure
 */
int serial_open(device dev, int speed);

/**
 * Closes a serial port
 * @param dev The serial port to close
 * @return 0 on success, non-zero on failure
 */
int serial_close(device dev);

/**
 * Reads from a serial port
 * @param dev device to read from
 * @param buf buffer to read into
 * @param len number of bytes to read
 * @return len on success, 0 on failure 
 */
int serial_read(device dev, char *buf, size_t len);

/**
 * Writes to a serial port
 * @param dev device to write to
 * @param buf buffer to write from
 * @param len number of bytes to write
 * @return 0 on success, non-zero on failure
 */
int serial_write(device dev, char *buf, size_t len);

/** 
 * Dequeues an iocb from the given queue
 * @param iocb_queue the front of the queue
 * @return 0 on success, non-zero on failure
 */
int iocb_dequeue(iocb **iocb_queue);

/**
 * Enqueues an iocb to the given queue
 * @param iocb_queue the front of the queue
 * @param io the iocb to enqueue
 */
void iocb_enqueue(iocb **iocb_queue, iocb *io);

#endif
