#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <mpx/io.h>
#include <mpx/device.h>
#include <string.h>
#include <sys_req.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
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
	sys_req(WRITE, COM1, message, strlen(message));
	sys_req(WRITE, COM1, "\r\n", 2);
    return (int)strlen(message);
}

void error(const char *message){
	sys_req(WRITE, COM1, RED, strlen(RED));
	println(message);
	sys_req(WRITE, COM1, RESET, strlen(RESET));
}

void success(const char *message){
	sys_req(WRITE, COM1, GREEN, strlen(GREEN));
	println(message);
	sys_req(WRITE, COM1, RESET, strlen(RESET));
}