#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

int atoi(const char *s)
{
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

char *iota(int n) 
{
	int tmp = n;
	int digits = 0;

	while (tmp < 0) {
		tmp /= 10;
		digits++;
	}

	char *res = sys_alloc_mem(digits + 1);
	res[digits] = '\0';

	for (int i = digits - 1; i >= 0; i--) {
		res[i] = (n % 10) + '0';
		n /= 10;
	}

	return res;
}
