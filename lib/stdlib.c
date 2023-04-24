#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <mpx/io.h>
#include <mpx/device.h>
#include <string.h>
#include <sys_req.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

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

	if (n == 0) {
		char *res = sys_alloc_mem(2);
		res[0] = '0';
		res[1] = '\0';
		return res;
	}

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

int dtoBCD(int dec) {
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

int BCDtod(int bcd) {
    int dec = 0;
	int i = 1; 
	int rem;
    while (bcd > 0) {
        rem = bcd % 10;
        bcd /= 10;
        dec += rem * i;
        i *= 16;
    }
    return dec;
}

char* dtoh(int dec) {
    char* hex = sys_alloc_mem(sizeof(char) * 11);

    if (!hex) {
        error("Failed to allocate int to hex memory");
    }

    if (dec == 0) {
        strcpy(hex, "0x00000000");
        return hex;
    }

    hex[10] = '\0';
    int i = 9;
    int rem;
    while (i > 1) {
        rem = dec % 16;
        dec /= 16;
        hex[i] = (rem > 9) ? rem - 10 + 'a' : rem + '0';
        i--;
    }
    hex[0] = '0';
    hex[1] = 'x';
    return hex;
}

int htod(const char *hex) {
    int dec = 0;
    int len = strlen(hex);
    int i = 2;

    for (; i < len; i++) {
        int rem;

        if (hex[i] >= '0' && hex[i] <= '9') {
            rem = hex[i] - '0';
        } else {
            rem = hex[i] - 'a' + 10;
        }
        dec = dec * 16 + rem;
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

void error(const char *message) {
	sys_req(WRITE, COM1, RED, strlen(RED));
	sys_req(WRITE, COM1, message, strlen(message));
	sys_req(WRITE, COM1, RESET, strlen(RESET));
	sys_req(WRITE, COM1, "\n", 1);
}

void success(const char *message) {
	sys_req(WRITE, COM1, GREEN, strlen(GREEN));
	sys_req(WRITE, COM1, message, strlen(message));
	sys_req(WRITE, COM1, RESET, strlen(RESET));
	sys_req(WRITE, COM1, "\n", 1);
}

void alarm_output(const char *message) {
	sys_req(WRITE, COM1, YELLOW, strlen(YELLOW));
	sys_req(WRITE, COM1, message, strlen(message));
	sys_req(WRITE, COM1, RESET, strlen(RESET));
	sys_req(WRITE, COM1, "\n", 1);
}