#include <mpx/io.h>
#include <mpx/serial.h>
#include <mpx/device.h>
#include <mpx/interrupts.h>
#include <mpx/call.h>
#include <memory.h>
#include <sys_req.h>
#include <stdlib.h>
#include <string.h>

enum uart_registers {
	RBR = 0,	// Receive Buffer
	THR = 0,	// Transmitter Holding
	DLL = 0,	// Divisor Latch LSB
	IER = 1,	// Interrupt Enable
	DLM = 1,	// Divisor Latch MSB
	IIR = 2,	// Interrupt Identification
	FCR = 2,	// FIFO Control
	LCR = 3,	// Line Control
	MCR = 4,	// Modem Control
	LSR = 5,	// Line Status
	MSR = 6,	// Modem Status
	SCR = 7,	// Scratch
};

static int initialized[4] = { 0 };
dcb dcb_table[4];

static int serial_devno(device dev)
{
	switch (dev) {
	case COM1: return 0;
	case COM2: return 1;
	case COM3: return 2;
	case COM4: return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	outb(dev + IER, 0x00);	//disable interrupts
	outb(dev + LCR, 0x80);	//set line control register
	outb(dev + DLL, 115200 / 9600);	//set bsd least sig bit
	outb(dev + DLM, 0x00);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);	//enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);	//enable interrupts, rts/dsr set
	(void)inb(dev);		//read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0) {
		return -1;
	}
	for (size_t i = 0; i < len; i++) {
		outb(dev, buffer[i]);
	}
	return (int)len;
}

#define BACKSPACE 0x08
#define DELETE 0x7F
#define CARRIAGE_RETURN 0x0D
#define NEWLINE 0x0A

int serial_poll(device dev, char *buffer, size_t len)
{
	size_t bufferIndex = 0;
	size_t cursorIndex = 0;

	while (bufferIndex < len-3) {
		if (!(inb(dev + LSR) & 0x01)) {
			continue;
		}
		char c = inb(dev);
		if (c == NEWLINE || c == CARRIAGE_RETURN) break;

		if (c == '\033' && inb(dev) == '[') {
			c = inb(dev);
			if (c == 'A' || c == 'B') {
				continue;
			}
			if (c == 'D') {
				if (cursorIndex == 0) {
					continue;
				}
				cursorIndex -= 1;
			}else if (c == 'C') {
				if (cursorIndex == bufferIndex) {
					continue;
				}
				cursorIndex += 1;
			}
			outb(dev, '\033');
			outb(dev, '[');
			outb(dev, c);
			continue;
		}else if (c == BACKSPACE || c == DELETE) {
			if (cursorIndex > 0) {
				outb(dev, BACKSPACE);
				outb(dev, ' ');
				outb(dev, BACKSPACE);
				cursorIndex--;
				bufferIndex--;

				for (size_t i = cursorIndex; i < bufferIndex; i++) {
					buffer[i] = buffer[i + 1];
					buffer[i + 1] = 0;
				}
				for (size_t i = cursorIndex; i < bufferIndex; i++) {
					outb(dev, buffer[i]);
				}
				outb(dev, ' ');
				outb(dev, BACKSPACE);
				for (size_t i = bufferIndex; i > cursorIndex; i--) {
					outb(dev, BACKSPACE);
				}
			}
			continue;
		}
		if (cursorIndex < bufferIndex) {
    		for (size_t i = bufferIndex; i > cursorIndex; i--) {
				buffer[i] = buffer[i - 1];
    		}
    		buffer[cursorIndex] = c;
			bufferIndex++;

			for (size_t i = cursorIndex; i < bufferIndex; i++) {
				outb(dev, buffer[i]);
			}
			for (size_t i = bufferIndex; i > cursorIndex + 1; i--) {
				outb(dev, BACKSPACE);
			}
  		}else {
			outb(dev, c);
			buffer[cursorIndex] = c;
			bufferIndex++;
		}
		cursorIndex++;
	}

	buffer[bufferIndex] = '\r';
	buffer[bufferIndex + 1] = '\n';
	buffer[bufferIndex + 2] = '\0';
	bufferIndex += 3;

	outb(dev, '\r');
	outb(dev, '\n');
	return bufferIndex;
}

void serial_input_interrupt(struct dcb *dcb) {
	device dev = 0;
	if (&dcb_table[0] == dcb) {
		dev = COM1;
	}else if (&dcb_table[1] == dcb) {
		dev = COM2;
	}else if (&dcb_table[2] == dcb) {
		dev = COM3;
	}else if (&dcb_table[3] == dcb) {
		dev = COM4;
	}else {
		return;
	}

	char c = inb(dev);	
	if (dcb->cur_op != READ) {
		if ((dcb->buf_end + 1) % dcb->buf_len == (size_t) dcb->buf_start) { // check if ring buffer is full
			return;
		}
		if (dcb->buf_start == -1) { // check if ring buffer is empty
			dcb->buf_start = 0;
			dcb->buf_end = 0;
		}
		// otherwise store the character at end of ring buffer
		dcb->buffer[dcb->buf_end] = c;
		dcb->buf_end = (dcb->buf_end + 1) % dcb->buf_len;
	}else {
		// store the character in the first IOCB in the queue
		iocb *io = dcb->iocb_queue;
		if (io == NULL) {
			return;
		}
		io->buffer[io->buf_idx++] = c;
		if (io->buf_idx == io->buf_len || c == '\n') { // buffer is now full or newline character
			dcb->cur_op = IDLE;
			dcb->event_flag = 1;
		}
	}
}

void serial_output_interrupt(struct dcb *dcb) {
	if (dcb->cur_op != WRITE) {
		return;
	}
	iocb *io = dcb->iocb_queue;
	if (io == NULL) {
		return;
	}

	device dev = 0;
	if (&dcb_table[0] == dcb) {
		dev = COM1;
	}else if (&dcb_table[1] == dcb) {
		dev = COM2;
	}else if (&dcb_table[2] == dcb) {
		dev = COM3;
	}else if (&dcb_table[3] == dcb) {
		dev = COM4;
	}else {
		return;
	}

	if (io->buffer[io->buf_idx] == '\0') { // end of buffer
		int ier = inb(dev + IER);
		ier &= ~0x02;
		outb(dev + IER, ier); // disable output interrupts

		dcb->cur_op = IDLE;
		dcb->event_flag = 1;
	}else {
		outb(dev, io->buffer[io->buf_idx++]); // write next character
	}
}

void serial_interrupt() {
	device dev = 0;
	// check which device caused the interrupt
	if ((inb(COM1 + IIR) & 0x01) == 0) {
		dev = COM1;
	}else if ((inb(COM2 + IIR) & 0x01) == 0) {
		dev = COM2;
	}else if ((inb(COM3 + IIR) & 0x01) == 0) {
		dev = COM3;
	}else if ((inb(COM4 + IIR) & 0x01) == 0) {
		dev = COM4;
	}
	dcb d = dcb_table[serial_devno(dev)];

	int iir = inb(dev + IIR);
	if (iir & 0x02) { // bit 1 (output)
		serial_output_interrupt(&d);
	}else if (iir & 0x04) { // bit 2 (input)
		serial_input_interrupt(&d);
	}
	outb(0x20, 0x20); // issue EOI to PIC to clear interrupt
}

int serial_open(device dev, int speed) {
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	if (dcb_table[dno].open_flag) { // check if device is already open
		return -103;
	}

	int dll = 115200 / speed;
	int dlm = 0;
	switch (dll) {
		case 0x00:
			dlm = 0x09;
			break;
		case 0x80:
			dlm = 0x01;
			break;
		case 0x60:
		case 0x30:
		case 0x18:
		case 0x0C:
		case 0x06:
		case 0x03:
		case 0x02:
		case 0x01:
			dlm = 0x00;
			break;
		default:
			return -102;
	}
	
	// initialize device
	dcb_table[dno] = (dcb) {
		.open_flag = 1,
		.event_flag = 0,
		.cur_op = IDLE,
		.buffer = NULL,
		.buf_len = 128,
		.buf_start = 0,
		.buf_end = 0,
		.iocb_queue = NULL
	};

	int vector = (dev == COM1 || dev == COM3) ? 0x24 : 0x23; // IRQ4 or IRQ3
	idt_install(vector, serial_interrupt);

	outb(dev + LCR, 0x80);	//set line control register	
	outb(dev + DLL, dll); 	//set bsd least sig bit
	outb(dev + DLM, dlm);	//brd most significant bit
	outb(dev + LCR, 0x03);	//lock divisor; 8bits, no parity, one stop
	outb(dev + MCR, 0x08);
	outb(dev + IER, 0x01);	//enable input ready interrupts

	cli();
	int mask = inb(0x21);
	int irq = (dev == COM1 || dev == COM3) ? 4 : 3; // IRQ4 or IRQ3
	mask &= (~1 << (irq - 1)); 
	outb(0x21, mask); // enable hardware IRQ4 or IRQ3
	sti();

	outb(dev + MCR, 0x08); // enable serial port interrupts
	outb(dev + IER, 0x01); // enable interrupts

	return 0;
}

int serial_close(device dev) {
	int dno = serial_devno(dev);
	if (dno == -1) {
		return -1;
	}
	dcb *d = &dcb_table[dno];
	if (d->open_flag == 0) { // device is already closed
		return -201;
	}
	d->open_flag = 0; // close device

	cli();
	int mask = inb(0x21);
	int irq = (dev == COM1 || dev == COM3) ? 4 : 3; // IRQ4 or IRQ3
	mask |= (1 << (irq - 1));
	outb(0x21, mask); // disable hardware IRQ4 or IRQ3
	sti();

	outb(dev + MSR, 0x00); // disable serial port interrupts
	outb(dev + IER, 0x00); // disable interrupts

	return 0;
}

int serial_read(device dev, char *buf, size_t len) {
	int dno = serial_devno(dev);
	dcb *d = &dcb_table[dno];
	
	if (d->open_flag == 0) { // device is closed
		return -301;
	}
	if (buf == NULL) {
		return -302;
	}
	if (len < 1) { 
		return -303;
	}
	if (device_state[dno]) { // device is busy
		return -304;
	}
	d->event_flag = 0;
	d->cur_op = READ;

	size_t i = 0, empty = 0; 
	while (!empty && d->buffer[d->buf_start] != '\n' && i < len) {
		empty = (d->buf_start == d->buf_end);

		buf[d->iocb_queue->buf_idx] = d->buffer[d->buf_start];
		d->buffer[d->buf_start] = 0;
		d->iocb_queue->buf_idx += 1;
		d->buf_start = (d->buf_start + 1) % d->buf_len;

		if (empty) {
			d->buf_start = -1; // maybe 0?
			d->buf_end = -1; // maybe 0?
			break;
		}
	}

	if (i < len) return i+1;
	
	d->cur_op = IDLE;
	d->event_flag = 1;
	return len;
}
//every read and write should create an iocb and add it to the queue
//not doing that here
int serial_write(device dev, char *buf, size_t len) {
	int dno = serial_devno(dev);
	dcb *d = &dcb_table[dno];

	if (d->open_flag == 0) { // device is closed
		return -401;
	}
	if (buf == NULL) {
		return -402;
	}
	if (len < 1) { 
		return -403;
	}
	if (device_state[dno]) { // device is busy
		return -404;
	}
	d->event_flag = 0;

	outb(dev, *buf); // write first character to output buffer
	d->iocb_queue->buf_idx += 1;

	int ier = inb(dev + IER);
	ier |= 0x02;
	outb(dev + IER, ier); // enable output interrupts
	
	return 0;
}

int iocb_dequeue(iocb **io_queue) {
	if (*io_queue == NULL) {
		return -1;
	}
	iocb *tmp = *io_queue;
	*io_queue = (*io_queue)->next;
	sys_free_mem(tmp);
	return 0;
}

void iocb_enqueue(iocb **io_queue, iocb *io) {
	if (*io_queue == NULL) {
		*io_queue = io;
		return;
	}
	iocb *cur = *io_queue;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	cur->next = io;
}