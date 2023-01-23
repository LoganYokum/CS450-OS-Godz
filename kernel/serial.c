#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdlib.h>

#define BACKSPACE 0x08
#define DELETE 0x7F
#define CARRIAGE_RETURN 0x0D
#define NEWLINE 0x0A

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
				bufferIndex--;
				cursorIndex--;
				buffer[cursorIndex] = 0;
			}
			continue;
		}
		if (cursorIndex < bufferIndex) {
    		for (size_t i = bufferIndex; i > cursorIndex; i--) {
      			char tmp = buffer[i - 1];
      			buffer[i] = tmp;
      			buffer[i - 1] = 0;
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