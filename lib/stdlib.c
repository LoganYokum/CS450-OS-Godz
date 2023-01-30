#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <mpx/io.h>
#include <mpx/device.h>
#include <string.h>

int atoi(const char *s){
	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

// still getting strange output for negative numbers but it works for positive numbers
char *itoa(int n) {
  	int tmp = n;
	int digits = 0;

  	while (tmp > 0) {
    	tmp /= 10;
   	 	digits++;
  	}

  	int negative = 0;
  	if (n < 0) {
   		negative = 1;
  	}

	char *res = sys_alloc_mem(digits + 1 + negative);
  	res[digits + negative] = '\0';

  	for (int i = digits - 1 + negative; i >= negative; i--) {
    	res[i] = (n % 10) + '0';
    	n /= 10;
  	}

  	return res;
}

int dtoh(int dec) {
    int hex = 0;
    int i = 1;
    int rem;
    while (dec > 0) {
        rem = dec % 16;
        dec /= 16;
        hex += rem * i;
        i *= 10;
    }
    return hex;
}

int htod(int hex) {
    int dec = 0;
	int i = 1; 
	int rem;
    while (hex > 0) {
        rem = hex % 10;
        hex /= 10;
        dec += rem * i;
        i *= 16;
    }
    return dec;
}

int validnum(const char *s) {
	for (size_t i = 0; i < strlen(s); i++) {
		if (s[i] < '0' || s[i] > '9') {
			return 0;
		}
	}
	return 1;
}

int println(const char* message){
        //TODO: possible that com1 will not always be the default device. may need to use a provided function to get the current
        //      device?
    int index = 0;
    while(*(index+message)!='\0'){ //pointer arithmetic to outb each byte in the message to COM1.
            outb(COM1,*(index+message));
            index++;
    }
    outb(COM1,'\r'); //carrage return
    outb(COM1,'\n'); //new line
    return (int)strlen(message);
}
