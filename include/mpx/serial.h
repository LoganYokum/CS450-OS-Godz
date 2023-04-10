#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#include <stddef.h>
#include <mpx/device.h>
#include <mpx/pcb.h>
#include <sys_req.h>

typedef struct iocb iocb;
typedef struct dcb dcb;

struct dcb {
    int open_flag;
    int event_flag;
    op_code cur_op;
    char buffer[128];
    size_t buf_len;
    size_t buf_start;
    size_t buf_end;
    iocb *iocb_queue;
};

struct iocb {
    op_code cur_op;
    char buffer[128];
    size_t len;
    pcb *process;
};

extern dcb devices[4];

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


void serial_input_interrupt(struct dcb *dcb);

void serial_output_interrupt(struct dcb *dcb);

void serial_interrupt();

int serial_open(device dev, int speed);

int serial_close(device dev);

#endif
